#ifndef _MOTOR_HPP
#define _MOTOR_HPP

#include "typedef.h"

/* This class is responsible for running the motor.  It will clamp any     */
/* input motor speeds to the maximum allowed by the timer, as well as stop */
/* the motor if the speed is below a certain threshold (below which the    */
/* motor doesn't have enough power to actually move).                      */

class CMotor {
   private:
      uns8 m_still_thresh;
      int32 m_max_speed;

   public:
      CMotor();
      ~CMotor();

      void init();
      void setStillThreshold(uns8 thresh);
      void setMaxSpeed(uns32 newMaxSpeed);
      void setSpeed(int32 newSpeed);
};

#endif
