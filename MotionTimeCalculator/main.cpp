#include <iostream>
#include <cmath>

// Function to calculate motion time using a cubic motion profile
double calculateMotionTime(double distance, double maxVelocity, double maxAcceleration, double jerk) 
{
/*
   // Calculate the time to reach max acceleration
   double t1 = maxAcceleration / jerk;
   double t2 = maxVelocity / maxAcceleration;
   double t3 = maxDeceleration / jerk;

   // Calculate the distance covered during acceleration and deceleration
   double d1 = 0.5 * jerk * std::pow(t1, 3);
   double d2 = maxVelocity * t2;
   double d3 = 0.5 * jerk * std::pow(t3, 3);

   // Check if the distance can be covered within the max velocity
   double totalDistance = d1 + d2 + d3;
   if ( distance < totalDistance ) 
   {
      // Recalculate the times if the distance is less than the total distance
      t1 = std::cbrt(2 * distance / jerk);
      t2 = 0;
      t3 = t1;
   }

   // Calculate the total time required
   double totalTime = t1 + t2 + t3;

   // Return the total time
   return totalTime;
   */

   float ta = maxAcceleration / jerk;
   float tb = ( maxVelocity - maxAcceleration * ta ) / maxAcceleration;
   float tc = distance / maxVelocity - maxAcceleration / jerk - maxVelocity / maxAcceleration;

   //!< Cases that don't reach the constant velocity
   if ( tc < 0.0 )
   {
      float a = maxAcceleration;
      float b = 3.0 * maxAcceleration * ta;
      float c = 2.0 * maxAcceleration * ta * ta - distance;

      float discriminant = b * b - 4 * a * c;
      float x1 = (-b + std::sqrt(discriminant)) / (2 * a);
      float x2 = (-b - std::sqrt(discriminant)) / (2 * a);

      float tb = x1;
      float totalTime = 4.0 * ta + 2.0 * tb;

      return totalTime;
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

   auto squareToMeter = [] ( int square ) { return square * 0.725; };

   MotionProfile profileReacingConstVel  = { squareToMeter(5), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 3.11 [sec], validated
   MotionProfile profileReacingConstAcc  = { squareToMeter(4), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.83 [sec],
   MotionProfile profileReacingConstAcc2 = { squareToMeter(3), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.51 [sec],
   MotionProfile profileReacingConstAcc3 = { squareToMeter(2), 2.23, 2.0, 2.0, 5.0 };  // total motion time: 2.14 [sec],
   MotionProfile profileTriangularAcc    = { squareToMeter(1), 2.23, 2.0, 2.0, 5.0 };  // total motion time: [sec],

   MotionProfile &profile = profileReacingConstAcc3;

   double motionTime = calculateMotionTime(profile.distance, profile.maxVelocity, profile.maxAcceleration, profile.jerk);

   printf( "Motion time: %.2f\n", motionTime);

   return 0;
}
