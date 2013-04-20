#ifndef _MOTOR_HPP
#define _MOTOR_HPP

#include "typedef.h"

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
