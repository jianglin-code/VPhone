#!/vendor/bin/sh

if [ ! -f /data/vendor/gps/gnss ]; then
    latitude=`getprop persist.ro.custommade.lo.latitude`
    if [ ! -n "$latitude" ]; then latitude=1.30; fi
    echo "LatitudeDegrees=$latitude" > /data/vendor/gps/gnss

    longitude=`getprop persist.ro.custommade.lo.longitude`
    if [ ! -n "$longitude" ]; then longitude=103.82; fi
    echo "LongitudeDegrees=$longitude" >> /data/vendor/gps/gnss

    altitude=`getprop persist.ro.custommade.lo.altitude`
    if [ ! -n "$altitude" ]; then altitude=0.0; fi
    echo "AltitudeMeters=$altitude" >> /data/vendor/gps/gnss

    bearing=`getprop persist.ro.custommade.lo.bearing`
    if [ ! -n "$bearing" ]; then bearing=0.0; fi
    echo "BearingDegrees=$bearing" >> /data/vendor/gps/gnss

    speedmeters=`getprop persist.ro.custommade.lo.speedmeters`
    if [ ! -n "$speedmeters" ]; then speedmeters=0.0; fi
    echo "SpeedMetersPerSec=$speedmeters" >> /data/vendor/gps/gnss

    chmod 777 /data/vendor/gps/gnss
fi
