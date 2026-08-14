/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cinttypes>
#include <log/log.h>
//#include <qemud.h>
#include <utils/SystemClock.h>
#include "multihal_sensors.h"
#include "sensor_list.h"

#include <android-base/properties.h>

namespace cells {
using ahs21::SensorType;
using ahs10::SensorFlagBits;
using ahs10::SensorStatus;
using ahs10::MetaDataEventType;

MultihalSensors::MultihalSensors()
        : m_qemuSensorsFd(0)
        , m_batchInfo(getSensorNumber()) {

    std::string sensors = android::base::GetProperty("ro.custommade.deviceinfo.sensors", 
        "acceleration,gyroscope,orientation,temperature,light,pressure");

    /*std::vector<int> types;
    size_t start = 0, end;
    while ((end = sensors.find(',', start)) != std::string::npos) {
        types.push_back(std::stoi(sensors.substr(start, end - start)));
        start = end + 1;
    }
    types.push_back(std::stoi(sensors.substr(start)));

    for(int t : types){
        if (t == (int)SensorType::ACCELEROMETER)
            m_availableSensorsHandles.push_back(kSensorHandleAccelerometer);
        else if (t == (int)SensorType::GYROSCOPE)
            m_availableSensorsHandles.push_back(kSensorHandleGyroscope);
        else if (t == (int)SensorType::MAGNETIC_FIELD)
            m_availableSensorsHandles.push_back(kSensorHandleMagneticField);
        else if (t == (int)SensorType::ORIENTATION)
            m_availableSensorsHandles.push_back(kSensorHandleOrientation);
        else if (t == (int)SensorType::AMBIENT_TEMPERATURE)
            m_availableSensorsHandles.push_back(kSensorHandleAmbientTemperature);
        else if (t == (int)SensorType::PROXIMITY)
            m_availableSensorsHandles.push_back(kSensorHandleProximity);
        else if (t == (int)SensorType::LIGHT)
            m_availableSensorsHandles.push_back(kSensorHandleLight);
        else if (t == (int)SensorType::PRESSURE)
            m_availableSensorsHandles.push_back(kSensorHandlePressure);
        else if (t == (int)SensorType::RELATIVE_HUMIDITY)
            m_availableSensorsHandles.push_back(kSensorHandleRelativeHumidity);
        else if (t == (int)SensorType::MAGNETIC_FIELD_UNCALIBRATED)
            m_availableSensorsHandles.push_back(kSensorHandleMagneticFieldUncalibrated);
        else if (t == (int)SensorType::GYROSCOPE_UNCALIBRATED)
            m_availableSensorsHandles.push_back(kSensorHandleGyroscopeFieldUncalibrated);
        else if (t == (int)SensorType::HINGE_ANGLE)
            m_availableSensorsHandles.push_back(kSensorHandleHingeAngle0);
        else if (t == (int)SensorType::HINGE_ANGLE)
            m_availableSensorsHandles.push_back(kSensorHandleHingeAngle1);
        else if (t == (int)SensorType::HINGE_ANGLE)
            m_availableSensorsHandles.push_back(kSensorHandleHingeAngle2);
        else if (t == (int)SensorType::HEART_RATE)
            m_availableSensorsHandles.push_back(kSensorHandleHeartRate);
    }*/

    m_availableSensorsMask = 0;
    for (int i=0; i < getSensorNumber(); i++){
        if (sensors.find(getQemuSensorNameByHandle(i)) != std::string::npos){
            m_availableSensorsMask |= (1u << i);
            ALOGD("%s:%d: name=%s available sensors mask=%x", __func__, __LINE__, getQemuSensorNameByHandle(i), m_availableSensorsMask);
        }
    }

    m_IsActive = true;
    m_sensorThread = std::thread(&MultihalSensors::qemuSensorListenerThread, this);
    m_batchThread = std::thread(&MultihalSensors::batchThread, this);
}

MultihalSensors::~MultihalSensors() {
    setAllQemuSensors(false);
    m_availableSensorsMask = 0;
    //m_availableSensorsHandles.clear();

    m_batchRunning = false;
    m_batchUpdated.notify_one();
    m_batchThread.join();

    m_IsActive = false;
    m_sensorThread.join();
}

const std::string MultihalSensors::getName() {
    return "hal_sensors_2_1_impl_ranchu";
}

Return<void> MultihalSensors::debug(const hidl_handle& fd, const hidl_vec<hidl_string>& args) {
    (void)fd;
    (void)args;
    return {};
}

Return<void> MultihalSensors::getSensorsList_2_1(getSensorsList_2_1_cb _hidl_cb) {
    std::vector<SensorInfo> sensors;

    uint32_t mask = m_availableSensorsMask;
    for (int i = 0; mask; ++i, mask >>= 1) {
        if (mask & 1) {
            sensors.push_back(*getSensorInfoByHandle(i));
        }
    }

    /*for (int t : m_availableSensorsHandles) {
        sensors.push_back(*getSensorInfoByHandle(t));
    }*/

    _hidl_cb(sensors);
    return {};
}

Return<Result> MultihalSensors::setOperationMode(const OperationMode mode) {
    std::unique_lock<std::mutex> lock(m_mtx);

    if (m_activeSensorsMask) {
        return Result::INVALID_OPERATION;
    } else {
        m_opMode = mode;
        return Result::OK;
    }
}

Return<Result> MultihalSensors::activate(const int32_t sensorHandle,
                                         const bool enabled) {
    if (!isSensorHandleValid(sensorHandle)) {
        return Result::BAD_VALUE;
    }

    std::unique_lock<std::mutex> lock(m_mtx);
    BatchInfo& batchInfo = m_batchInfo[sensorHandle];

    if (enabled) {
        const SensorInfo* sensor = getSensorInfoByHandle(sensorHandle);
        LOG_ALWAYS_FATAL_IF(!sensor);
        if (!(sensor->flags & static_cast<uint32_t>(SensorFlagBits::ON_CHANGE_MODE))) {
            if (batchInfo.samplingPeriodNs <= 0) {
                return Result::BAD_VALUE;
            }

            BatchEventRef batchEventRef;
            batchEventRef.timestamp =
                ::android::elapsedRealtimeNano() + batchInfo.samplingPeriodNs;
            batchEventRef.sensorHandle = sensorHandle;
            batchEventRef.generation = ++batchInfo.generation;

            m_batchQueue.push(batchEventRef);
            m_batchUpdated.notify_one();
        } else if (sensor->type == SensorType::HEART_RATE){
            // Heart rate sensor's first data after activation should be
            // SENSOR_STATUS_UNRELIABLE.
            Event event;
            event.u.heartRate.status = SensorStatus::UNRELIABLE;
            event.u.heartRate.bpm = 0;
            event.timestamp = ::android::elapsedRealtimeNano();
            event.sensorHandle = sensorHandle;
            event.sensorType = SensorType::HEART_RATE;
            doPostSensorEventLocked(*sensor, event);
        }

        m_activeSensorsMask = m_activeSensorsMask | (1u << sensorHandle);
    } else {
        m_activeSensorsMask = m_activeSensorsMask & ~(1u << sensorHandle);
    }
    return Result::OK;
}

Return<Result> MultihalSensors::batch(const int32_t sensorHandle,
                                      const int64_t samplingPeriodNs,
                                      const int64_t maxReportLatencyNs) {
    (void)maxReportLatencyNs;

    if (!isSensorHandleValid(sensorHandle)) {
        return Result::BAD_VALUE;
    }

    const SensorInfo* sensor = getSensorInfoByHandle(sensorHandle);
    LOG_ALWAYS_FATAL_IF(!sensor);

    if (samplingPeriodNs < sensor->minDelay) {
        return Result::BAD_VALUE;
    }

    std::unique_lock<std::mutex> lock(m_mtx);
    if (m_opMode == OperationMode::NORMAL) {
        m_batchInfo[sensorHandle].samplingPeriodNs = samplingPeriodNs;
    }

    return Result::OK;
}

Return<Result> MultihalSensors::flush(const int32_t sensorHandle) {
    if (!isSensorHandleValid(sensorHandle)) {
        return Result::BAD_VALUE;
    }

    const SensorInfo* sensor = getSensorInfoByHandle(sensorHandle);
    LOG_ALWAYS_FATAL_IF(!sensor);

    std::unique_lock<std::mutex> lock(m_mtx);
    if (!isSensorActive(sensorHandle)) {
        return Result::BAD_VALUE;
    }

    Event event;
    event.sensorHandle = sensorHandle;
    event.sensorType = SensorType::META_DATA;
    event.u.meta.what = MetaDataEventType::META_DATA_FLUSH_COMPLETE;

    doPostSensorEventLocked(*sensor, event);
    return Result::OK;
}

Return<Result> MultihalSensors::injectSensorData_2_1(const Event& event) {
    if (!isSensorHandleValid(event.sensorHandle)) {
        return Result::BAD_VALUE;
    }
    if (event.sensorType == SensorType::ADDITIONAL_INFO) {
        return Result::OK;
    }

    std::unique_lock<std::mutex> lock(m_mtx);
    if (m_opMode != OperationMode::DATA_INJECTION) {
        return Result::INVALID_OPERATION;
    }
    const SensorInfo* sensor = getSensorInfoByHandle(event.sensorHandle);
    LOG_ALWAYS_FATAL_IF(!sensor);
    if (sensor->type != event.sensorType) {
        return Result::BAD_VALUE;
    }

    doPostSensorEventLocked(*sensor, event);
    return Result::OK;
}

Return<Result> MultihalSensors::initialize(const sp<IHalProxyCallback>& halProxyCallback) {
    std::unique_lock<std::mutex> lock(m_mtx);
    setAllQemuSensors(true);   // we need to start sampling sensors for batching
    m_opMode = OperationMode::NORMAL;
    m_halProxyCallback = halProxyCallback;
    return Result::OK;
}

void MultihalSensors::postSensorEvent(const Event& event) {
    const SensorInfo* sensor = getSensorInfoByHandle(event.sensorHandle);
    LOG_ALWAYS_FATAL_IF(!sensor);

    std::unique_lock<std::mutex> lock(m_mtx);
    if (sensor->flags & static_cast<uint32_t>(SensorFlagBits::ON_CHANGE_MODE)) {
        if (isSensorActive(event.sensorHandle)) {
            doPostSensorEventLocked(*sensor, event);
        }
    } else {    // CONTINUOUS_MODE
        m_batchInfo[event.sensorHandle].event = event;
    }
}

void MultihalSensors::doPostSensorEventLocked(const SensorInfo& sensor,
                                              const Event& event) {
    const bool isWakeupEvent =
        sensor.flags & static_cast<uint32_t>(SensorFlagBits::WAKE_UP);

    m_halProxyCallback->postEvents(
        {event},
        m_halProxyCallback->createScopedWakelock(isWakeupEvent));
}

bool MultihalSensors::qemuSensorThreadSendCommand(const char /*cmd*/) const {
    return true;
}

bool MultihalSensors::isSensorHandleValid(int sensorHandle) const {
    if (!cells::isSensorHandleValid(sensorHandle)) {
        return false;
    }

    if (!(m_availableSensorsMask & (1u << sensorHandle))) {
        return false;
    }

    /*if (std::find(m_availableSensorsHandles.begin(), m_availableSensorsHandles.end(), sensorHandle) 
        == m_availableSensorsHandles.end()) {
        return false;
    }*/

    return true;
}

void MultihalSensors::batchThread() {
    while (m_batchRunning) {
        std::unique_lock<std::mutex> lock(m_mtx);
        if (m_batchQueue.empty()) {
            m_batchUpdated.wait(lock);
        } else {
            const int64_t d =
                m_batchQueue.top().timestamp - ::android::elapsedRealtimeNano();
            m_batchUpdated.wait_for(lock, std::chrono::nanoseconds(d));
        }

        const int64_t nowNs = ::android::elapsedRealtimeNano();
        while (!m_batchQueue.empty() && (nowNs >= m_batchQueue.top().timestamp)) {
            BatchEventRef evRef = m_batchQueue.top();
            m_batchQueue.pop();

            const int sensorHandle = evRef.sensorHandle;
            LOG_ALWAYS_FATAL_IF(!cells::isSensorHandleValid(sensorHandle));
            if (!isSensorActive(sensorHandle)) {
                continue;
            }

            BatchInfo &batchInfo = m_batchInfo[sensorHandle];
            if (batchInfo.event.sensorType == SensorType::META_DATA) {
                ALOGW("%s:%d the host has not provided value yet for sensorHandle=%d", __func__, __LINE__, sensorHandle);
            } else {
                batchInfo.event.timestamp = evRef.timestamp;
                const SensorInfo* sensor = getSensorInfoByHandle(sensorHandle);
                LOG_ALWAYS_FATAL_IF(!sensor);
                doPostSensorEventLocked(*sensor, batchInfo.event);
            }

            if (evRef.generation == batchInfo.generation) {
                const int64_t samplingPeriodNs = batchInfo.samplingPeriodNs;
                LOG_ALWAYS_FATAL_IF(samplingPeriodNs <= 0);

                evRef.timestamp += samplingPeriodNs;
                m_batchQueue.push(evRef);
            }
        }
    }
}

/// not supported //////////////////////////////////////////////////////////////
Return<void> MultihalSensors::registerDirectChannel(const SharedMemInfo& mem,
                                                    registerDirectChannel_cb _hidl_cb) {
    (void)mem;
    _hidl_cb(Result::INVALID_OPERATION, -1);
    return {};
}

Return<Result> MultihalSensors::unregisterDirectChannel(int32_t channelHandle) {
    (void)channelHandle;
    return Result::INVALID_OPERATION;
}

Return<void> MultihalSensors::configDirectReport(int32_t sensorHandle,
                                                 int32_t channelHandle,
                                                 RateLevel rate,
                                                 configDirectReport_cb _hidl_cb) {
    (void)sensorHandle;
    (void)channelHandle;
    (void)rate;
    _hidl_cb(Result::INVALID_OPERATION, 0 /* reportToken */);
    return {};
}

}  // namespace
