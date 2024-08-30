#include <iostream>
#include <cmath>

// Function to calculate motion time using a cubic motion profile
double calculateMotionTime(double distance, double maxVelocity, double maxAcceleration, double jerk) 
{
   float ta = maxAcceleration / jerk;
   float tb = ( maxVelocity - maxAcceleration * ta ) / maxAcceleration;
   float tc = distance / maxVelocity - maxAcceleration / jerk - maxVelocity / maxAcceleration;

   //!< Cases that don't reach the constant velocity; recalculate tb
   if ( tc < 0.0 )
   {
      float a = maxAcceleration;
      float b = 3.0 * maxAcceleration * ta;
      float c = 2.0 * maxAcceleration * ta * ta - distance;

      float discriminant = b * b - 4 * a * c;
      float tb = (-b + std::sqrt(discriminant)) / (2 * a);

      //!< Cases that don't reach the constant acceleration; recalculate ta
      if ( tb < 0.0 )
      {
         float ta = sqrt( distance / ( 2.0 * maxAcceleration ) );
         float totalTime = 4.0 * ta;
         return totalTime;
      }
      else
      {
         float totalTime = 4.0 * ta + 2.0 * tb;
         return totalTime;
      }
   }
   //!< Cases that reach the constant velocity
   else
   {
      float totalTime = 4.0 * ta + 2.0 * tb + tc;      
      float tAcc = 2.0 * ta + tb;

      return totalTime;
   }
}

int main() {

   struct MotionProfile
   {
      double distance;
      double maxVelocity;
      double maxAcceleration;
      double maxDeceleration;
      double jerk;
   };

   auto squareToMeter = [] ( float square ) { return square * 0.725; };

   MotionProfile profileReachingConstVel  = { squareToMeter(5), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 3.11 [sec], validated
   MotionProfile profileReachingConstAcc  = { squareToMeter(4), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.83 [sec],
   MotionProfile profileReachingConstAcc2 = { squareToMeter(3), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.51 [sec],
   MotionProfile profileReachingConstAcc3 = { squareToMeter(2), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.14 [sec],
   MotionProfile profileReachingConstAcc4 = { squareToMeter(1), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 1.65 [sec],
   MotionProfile profileTriangular        = { squareToMeter(0.1), 2.23, 2.0, 2.0, 5.0 }; // total motion time: 0.54 [sec],

   MotionProfile &profile = profileTriangular;

   double motionTime = calculateMotionTime(profile.distance, profile.maxVelocity, profile.maxAcceleration, profile.jerk);

   printf( "Motion time: %.2f\n", motionTime);

   return 0;
}
