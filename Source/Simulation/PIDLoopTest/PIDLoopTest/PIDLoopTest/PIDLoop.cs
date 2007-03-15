using System;
using System.Collections.Generic;
using System.Text;

namespace PIDLoopTest
{
    class PIDLoop
    {
        Kinematics k;
        double Integral = 0;//integral needs to persist
        public PIDLoop(Kinematics k)
        {
            this.k = k;
        }
        public double calculateCorrection(double CorrectPosition)
        {
            //constants that will need to be empirically derived or mathematically calculated
            double PropConst = 1000;
            double IntConst = 50;
            double DerConst = 10;

            //variable declarations:
            double Proportion;

            double Derivative;
            double Correction;


            Proportion = -(k.XPosition - CorrectPosition) * PropConst;
            Integral += -(k.XPosition - CorrectPosition) * (k.interval) * IntConst;
            Derivative = -k.XVelocity * DerConst;
            Correction = Proportion + Integral + Derivative;

            return Correction;
        }
    }
}
