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

#include <log/log.h>
#include <utils/SystemClock.h>
#include <math.h>
//#include <qemud.h>
#include "multihal_sensors.h"
#include "sensor_list.h"

namespace cells {
using ahs10::EventPayload;
using ahs21::SensorType;
using ahs10::SensorStatus;

namespace {

bool approximatelyEqual(double a, double b, double eps) {
    return fabs(a - b) <= std::max(fabs(a), fabs(b)) * eps;
}

/*int64_t weigthedAverage(const int64_t a, int64_t aw, int64_t b, int64_t bw) {
    return (a * aw + b * bw) / (aw + bw);
}*/

}  // namespace

bool MultihalSensors::activateQemuSensorImpl(const int /*pipe*/,
                                             const int /*sensorHandle*/,
                                             const bool /*enabled*/) {
    return true;
}

bool MultihalSensors::setAllQemuSensors(const bool enabled) {
    uint32_t mask = m_availableSensorsMask;
    for (int i = 0; mask; ++i, mask >>= 1) {
        if (mask & 1) {
            if (!activateQemuSensorImpl(0, i, enabled)) {
                return false;
            }
        }
    }

    return true;
}

void MultihalSensors::parseQemuSensorEvent(const int /*pipe*/,
                                           QemuSensorsProtocolState* state) {
    const int64_t nowNs = ::android::elapsedRealtimeNano();
    Event event;
    EventPayload* payload = &event.u;
    ahs10::Vec3* vec3 = &payload->vec3;
    ahs10::Uncal* uncal = &payload->uncal;
    int64_t timestamp = static_cast<int64_t>(time(NULL));
    std::srand(static_cast<unsigned int>(timestamp));
    float r0 = (std::rand() % 10) * 1.1f;
    float r1 = (std::rand() % 10) * 0.1f;
    float r2 = (std::rand() % 10) * 0.01f;

    if ((m_availableSensorsMask >> kSensorHandleAccelerometer) & 1) {
        vec3->x = vec3->z = r2;
        vec3->y = 9.78f + r2;
        vec3->status = SensorStatus::ACCURACY_MEDIUM;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandleAccelerometer;
        event.sensorType = SensorType::ACCELEROMETER;
        postSensorEvent(event);
    } 
    if ((m_availableSensorsMask >> kSensorHandleGyroscope) & 1) {
        vec3->x = r1 + r2;
        vec3->y = -r2;
        vec3->z = r1 - r2;
        vec3->status = SensorStatus::ACCURACY_MEDIUM;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandleGyroscope;
        event.sensorType = SensorType::GYROSCOPE;
        postSensorEvent(event);
    }
    if ((m_availableSensorsMask >> kSensorHandleGyroscopeFieldUncalibrated) & 1) {
        uncal->x = r1 - r2;
        uncal->y = r1 + r2;
        uncal->z = r2;
        uncal->x_bias = 0.0;
        uncal->y_bias = 0.0;
        uncal->z_bias = 0.0;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandleGyroscopeFieldUncalibrated;
        event.sensorType = SensorType::GYROSCOPE_UNCALIBRATED;
        postSensorEvent(event);
    } 
    if ((m_availableSensorsMask >> kSensorHandleOrientation) & 1) {
        vec3->x = r2;
        vec3->y = -90.0f - r2;
        vec3->z = r1 * 2.0f;
        vec3->status = SensorStatus::ACCURACY_HIGH;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandleOrientation;
        event.sensorType = SensorType::ORIENTATION;
        postSensorEvent(event);
    } 
    if ((m_availableSensorsMask >> kSensorHandleMagneticField) & 1) {
        vec3->x = 35.74f + r0 + r1;
        vec3->y = 28.82f + r1 + r2;
        vec3->z = -45.72f + r0 + r2 * 5.2f;
        vec3->status = SensorStatus::ACCURACY_HIGH;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandleMagneticField;
        event.sensorType = SensorType::MAGNETIC_FIELD;
        postSensorEvent(event);
    } 
    if ((m_availableSensorsMask >> kSensorHandleMagneticFieldUncalibrated) & 1) {
        uncal->x = 94.95f + r1 + r2;
        uncal->y = 100.58f + r2 * 2.6f;
        uncal->z = -47.19f + r1 + r2;
        uncal->x_bias = 0.0;
        uncal->y_bias = 0.0;
        uncal->z_bias = 0.0;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandleMagneticFieldUncalibrated;
        event.sensorType = SensorType::MAGNETIC_FIELD_UNCALIBRATED;
        postSensorEvent(event);
    } 
    if ((m_availableSensorsMask >> kSensorHandleAmbientTemperature) & 1) {
        payload->scalar = 25.0f + r0 + r2;
        if (!approximatelyEqual(state->lastAmbientTemperatureValue,
                                payload->scalar, 0.001)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleAmbientTemperature;
            event.sensorType = SensorType::AMBIENT_TEMPERATURE;
            postSensorEvent(event);
            state->lastAmbientTemperatureValue = payload->scalar;
        }
    } 
    if ((m_availableSensorsMask >> kSensorHandleProximity) & 1) {
        payload->scalar = (std::rand() % 2) * 5.0f;
        if (!approximatelyEqual(state->lastProximityValue,
                                payload->scalar, 0.001)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleProximity;
            event.sensorType = SensorType::PROXIMITY;
            postSensorEvent(event);
            state->lastProximityValue = payload->scalar;
        }
    } 
    if ((m_availableSensorsMask >> kSensorHandleLight) & 1) {
        payload->scalar = 240.67f + r0 * 3.4f + r1 + r2;
        if (!approximatelyEqual(state->lastLightValue,
                                payload->scalar, 0.001)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleLight;
            event.sensorType = SensorType::LIGHT;
            postSensorEvent(event);
            state->lastLightValue = payload->scalar;
        }
    } 
    if ((m_availableSensorsMask >> kSensorHandlePressure) & 1) {
        payload->scalar = r2;
        event.timestamp = nowNs + state->timeBiasNs;
        event.sensorHandle = kSensorHandlePressure;
        event.sensorType = SensorType::PRESSURE;
        postSensorEvent(event);
    } 
    if ((m_availableSensorsMask >> kSensorHandleRelativeHumidity) & 1) {
        payload->scalar = r1;
        if (!approximatelyEqual(state->lastRelativeHumidityValue,
                                payload->scalar, 0.001)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleRelativeHumidity;
            event.sensorType = SensorType::RELATIVE_HUMIDITY;
            postSensorEvent(event);
            state->lastRelativeHumidityValue = payload->scalar;
        }
    } 
    if ((m_availableSensorsMask >> kSensorHandleHingeAngle0) & 1) {
        payload->scalar = r1;
        if (!approximatelyEqual(state->lastHingeAngle0Value,
                                payload->scalar, 0.001) &&
            // b/197586273, ignore the state tracking if system sensor
            // service has not enabled hinge sensor
            isSensorActive(kSensorHandleHingeAngle0)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleHingeAngle0;
            event.sensorType = SensorType::HINGE_ANGLE;
            postSensorEvent(event);
            state->lastHingeAngle0Value = payload->scalar;
        }
    } 
    if ((m_availableSensorsMask >> kSensorHandleHingeAngle1) & 1) {
        payload->scalar = r1;
        if (!approximatelyEqual(state->lastHingeAngle1Value,
                                payload->scalar, 0.001) &&
            isSensorActive(kSensorHandleHingeAngle1)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleHingeAngle1;
            event.sensorType = SensorType::HINGE_ANGLE;
            postSensorEvent(event);
            state->lastHingeAngle1Value = payload->scalar;
        }
    }
    if ((m_availableSensorsMask >> kSensorHandleHingeAngle2) & 1) {
        payload->scalar = r1;
        if (!approximatelyEqual(state->lastHingeAngle2Value,
                                payload->scalar, 0.001) &&
            isSensorActive(kSensorHandleHingeAngle2)) {
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleHingeAngle2;
            event.sensorType = SensorType::HINGE_ANGLE;
            postSensorEvent(event);
            state->lastHingeAngle2Value = payload->scalar;
        }
    }
    if ((m_availableSensorsMask >> kSensorHandleHeartRate) & 1) {
        payload->heartRate.bpm = r0 * 30.5f;
        if (!approximatelyEqual(state->lastHeartRateValue,
                                payload->heartRate.bpm, 0.001)) {
            payload->heartRate.status = SensorStatus::ACCURACY_HIGH;
            event.timestamp = nowNs + state->timeBiasNs;
            event.sensorHandle = kSensorHandleHeartRate;
            event.sensorType = SensorType::HEART_RATE;
            postSensorEvent(event);
            state->lastHeartRateValue = payload->heartRate.bpm;
        }
    }
}

}  // namespace
