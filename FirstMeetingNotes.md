# Proposed microprocessor layout for control/feedback #

On Saturday Feb 17 2007 we met to discuss various issues for the design of this helicopter.


# Details #

We decided a good approach is to use a central [CPU](cpu.md) for the high level processing, and several low end PICs for specialized tasks.

Jobs done by the main CPU:
  * Communication with base station
  * assigning tasks to the specialized MCUs
  * non-flight control sensing: battery level, GPS, etc.
  * photography
  * collision detection

Specialized MCUs (12fxx or 16fxx PIC):

In the meeting we discussed using specialized MCUs to control flight stability. We will need to address the issue of in-circuit programming for these MCUs.
  * Pitch/Roll MCU
    * Controls 2 servos connected to main rotor
    * requires feedback from gyroscopes, level sensor
  * Collective MCU
    * Controls collective blade pitch via 1 servo connected to main rotor
    * requires feedback from GPS, elevation sensors,
  * Yaw MCU
    * Controls servo connected to rear rotor
    * requires feedback from compass, gyroscope
  * Motor Control MCU
    * Controls motor power
    * requires feedback from GPS, elevation sensors, possibly motor RPM
  * Base Station MCU
    * A ground based radio to Laptop bridge





