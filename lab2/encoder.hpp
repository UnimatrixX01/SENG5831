#ifndef _ENCODER_HPP
#define _ENCODER_HPP

#include "typedef.h"

/* This class reads the encoder values from the motor encoders and allows  */
/* the application access to the variables.                                */

class CEncoder {
   private:
      static CEncoder m_encoder;
      uns8 m_last_pin1;
      uns8 m_last_pin2;

      int32   m_position;
      boolean m_error;

   private:
      CEncoder();

   public:
      ~CEncoder();

      static CEncoder &get();

      void init();
      int32 getPosition();
      boolean getError();
      void clearError();

      void update();

      static int32 ticksToDegrees(int32 ticks);
      static int32 degreesToTicks(int32 deg);
};

#endif
