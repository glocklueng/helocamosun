# Introduction #

This is a discussion about how the helicopter's stability algorithms will work.
A good starting point for reference is [this](http://en.wikipedia.org/wiki/PID_controller) wikipedia article

# Cyclic Stability #

This aspect must be broken down into 2 parts.
  * forward/reverse pitch
  * side to side roll

## Forward/Reverse ##

Proportional: This is the absolute pitch angle of the helocopter which is read in for every iteration of the PID loop.

Integral: Each loop of the control adds the pitch angle multiplied by the time elapsed since last loop execution.  An accumulated error is built up.

Derivative: A gyroscope that reads the rate of tilt can be sampled.

Solution: The solution is a cyclic correction in the front/back direction

### C code: ###

```
//constants that will need to be empirically derived or mathematically calculated
#define PropConst = 1
#define IntConst = 1
#define DerConst = 1

//variable declarations:
int Proportion;
int Integral = 0;
int Derivative;
int Correction;
long LastTime;

//Loop:
//ReadForwardBackTilt() returns the tilt forward or backward as a scaled integer
//ReadGyro() returns an int with the scaled rate of tilt forward or backward
//Time() returns a long int counter that it incremented at some interval in an interrupt
while(1)
{
  Proportion = ReadForwardBackTilt() * PropConst;
  Integral += ReadForwardBackTilt() * (Time() - LastTime) * IntConst;
  Derivative = ReadGyro() * DerConst;
  Correction = Proportion + Integral + Derivative;
  ApplyCorrection(Correction);
  LastTime = Time();
}

```