#include <iostream>
#include <cmath>

struct MotionTime
{
   double accDecTime;
   double constVeltime;
   double totalTime;
};

struct MotionProfile
{
   double distance;
   double maxVelocity;
   double maxAcceleration;
   double maxDeceleration;
   double jerk;
};

// Function to calculate motion time using a cubic motion profile
MotionTime calculateMotionTime(double P, double V, double A, double J) 
{
   double ta = A / J;
   double tb = ( V - A * ta ) / A;
   double tc = P / V - A / J - V / A;

   MotionTime time;

   //!< Cases that don't reach the constant velocity; recalculate tb
   if ( tc < 0.0 )
   {
      double a = A;
      double b = 3.0 * A * ta;
      double c = 2.0 * A * ta * ta - P;

      double discriminant = b * b - 4 * a * c;
      double tb = (-b + std::sqrt(discriminant)) / (2 * a);

      //!< Cases that don't reach the constant acceleration; recalculate ta
      if ( tb < 0.0 )
      {
         double ta = sqrt( P / ( 2.0 * A ) );
         double totalTime = 4.0 * ta;

         time.accDecTime = 2.0 * ta;
      }
      else
      {
         double totalTime = 4.0 * ta + 2.0 * tb;
         time.totalTime = totalTime;

         time.accDecTime = 2.0 * ta + tb;
      }

      time.constVeltime = 0.0;
   }
   //!< Cases that reach the constant velocity
   else
   {
      double totalTime = 4.0 * ta + 2.0 * tb + tc;      
      double tAcc = 2.0 * ta + tb;

      time.accDecTime = tAcc;
      time.constVeltime = tc;
   }

   time.totalTime = time.accDecTime * 2. + time.constVeltime;
   return time;
}

MotionTime calculateMotionTime( MotionProfile profile ) 
{
   return calculateMotionTime( profile.distance, profile.maxVelocity, profile.maxAcceleration, profile.jerk );
}

void printMotionTime( MotionTime time ) 
{
   printf( "Motion time[s]: total=%.3f, acc=%.3f, dec=%.3f, constVel=%.3f\r\n", time.totalTime, time.accDecTime, time.accDecTime, time.constVeltime );
}

int main() 
{
   // struct MotionProfile
   // {
   //    double distance;
   //    double maxVelocity;
   //    double maxAcceleration;
   //    double maxDeceleration;
   //    double jerk;
   // };

   auto squareToMeter = [] ( double square ) -> double { return square * 0.725; };

   MotionProfile profileReachingConstVel   = { squareToMeter(5),  2.23, 2.0, 2.0, 5.0 }; 
   MotionProfile profile1                  = { squareToMeter(5),  3.90, 2.5, 2.5, 5.0 }; 
   MotionProfile profile2                  = { squareToMeter(12), 3.90, 2.5, 2.5, 5.0 }; 
   MotionProfile profile3                  = { squareToMeter(5),  2.23, 2.5, 2.5, 5.0 }; 

   printMotionTime( calculateMotionTime( profileReachingConstVel ) );
   printMotionTime( calculateMotionTime( profile1 ) );
   printMotionTime( calculateMotionTime( profile2 ) );   
   printMotionTime( calculateMotionTime( profile3 ) );

   MotionProfile profileReachingConstAcc  = { squareToMeter(4), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.83 [sec],
   MotionProfile profileReachingConstAcc2 = { squareToMeter(3), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.51 [sec],
   MotionProfile profileReachingConstAcc3 = { squareToMeter(2), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.14 [sec],
   MotionProfile profileReachingConstAcc4 = { squareToMeter(1), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 1.65 [sec],
   MotionProfile profileTriangular        = { squareToMeter(0.1), 2.23, 2.0, 2.0, 5.0 }; // total motion time: 0.54 [sec],

   return 0;
}
