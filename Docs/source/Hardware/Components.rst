*************
Components
*************

Fault Indicator/Strobe Light
=============================
Description
    The strobelight contains 4, 3 watt LED's and is powered by a 12V line. It has 4 wires for setup
    although we only need to use 3. Outside of power and ground, the other 2 wires are for sync and
    pattern. 
Usage    
    The sync wire is if we are using multiple strobelights that must strobe at the same
    time. The pattern wire must be shorted with power if the pattern is to be changed. Once it is 
    set to a pattern, it remembers the pattern with on board flash memory.
Reason
    The strobelight is used to alert other vehicles on the road that the car is dangerous and to 
    avoid it. It is also required under the ASC regulations(9.4.G).

Fuse
====
Description
    This fuse is meant to protect the battery in the event of a short circuit in case the BPS cannot
    react fast enough. This `fuse <https://www.allfuses.com/pub/media/documents/Ferraz%20A15QS.pdf>`__
    is rated for 60A and 150VDC and is what is used in the BPS. 
Usage
    It is placed on the high voltage line between the contactor and battery pack.
Reason
    According to the ASC regulations(8.5.A), the fuse must be rated for less than 200% of the maximum
    expected current draw(which for this system is around 60A). The rating must also be for more than
    75% of the maximum current capacity of the wire(tbd).

`Contactors <https://www.waytekwire.com/datasheet/77101.pdf>`__
===============================================================
Description
    Turn car on and off. 
Usage
    Control power delivered to load (HV+), power delivered to battery (ARR+), GNDPWR(HV-) connections by
    powering contactors. They are normally open and have a turn on current around 4A (when tested). They
    have a sense pin that returns whether the contactors are on or off. If the kill switch is pressed, 
    the Contactors power is removed and the sense line is pulled low. The sense line must be read to 
    turn on the strobe light of the car.

`Fans <https://noctua.at/en/nf-f12-industrialppc-2000-pwm/specification>`__
===========================================================================
Description
    Cool down battery pack.
Usage
    Use PWM to change speed of fans and save power.

Horn
====
Description 
    Makes Noise. Also draws around 3.6A at 12V
