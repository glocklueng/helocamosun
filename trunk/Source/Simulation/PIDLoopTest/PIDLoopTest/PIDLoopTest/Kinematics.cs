using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections;

namespace PIDLoopTest
{

    class Force
    {
        public double X;
        public double Y;
        public double Z;
        
    }
    class Kinematics
    {
        public double time = 0;
        public ArrayList Forces;
        Stopwatch s;

        public double XAcceleration = 0;

        public double YAcceleration = 0;
        public double ZAcceleration = 0;

        public double XVelocity = 0;
        public double YVelocity = 0;
        public double ZVelocity = 0;

        public double XVelocityPrev = 0;
        public double YVelocityPrev = 0;
        public double ZVelocityPrev = 0;

        public double XPosition = 0;
        public double YPosition = 0;
        public double ZPosition = 0;

        public double XDisplacement = 0;
        public double YDisplacement = 0;
        public double ZDisplacement = 0;

        public double mass = 1;

        public double interval = 0.001;

        public Kinematics()
        {
            Forces = new ArrayList();
            s = new Stopwatch();
            s.Calibrate();
        }
        private Force SumForces()
        {
            Force Sum = new Force();
            foreach (Force F in Forces)
            {
                Sum.X += F.X;
                Sum.Y += F.Y;
                Sum.Z += F.Z;
            }
            return Sum;
        }
        public void CalculateKinematics()
        {

            Force Sum = SumForces();

            s.Stop();
            time = interval;
            s.Reset();
            s.Start();

            XAcceleration = Sum.X / mass;
            YAcceleration = Sum.Y / mass;
            ZAcceleration = Sum.Z / mass;

            XVelocity = XVelocityPrev + XAcceleration * time;
            YVelocity = YVelocityPrev + YAcceleration * time;
            ZVelocity = ZVelocityPrev + ZAcceleration * time;

            XDisplacement = (XVelocityPrev + XVelocity) / 2 * time;
            YDisplacement = (YVelocityPrev + YVelocity) / 2 * time;
            ZDisplacement = (ZVelocityPrev + ZVelocity) / 2 * time;

            XPosition += XDisplacement;
            YPosition += YDisplacement;
            ZPosition += ZDisplacement;

            //the initial velocity for the next cycle is the final velocity on this cycle
            XVelocityPrev = XVelocity;
            YVelocityPrev = YVelocity;
            ZVelocityPrev = ZVelocity;

            

        }
    }
}
