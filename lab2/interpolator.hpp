#ifndef _INTERPOLATOR_HPP
#define _INTERPOLATOR_HPP

#include "typedef.h"

/* This class interpolates between the final position and the current PID  */
/* destination so the PID controller can be tuned correctly.               */

class CInterpolator {
   private:
      static CInterpolator m_interpolator;

      int32 m_finalPr;
      int32 m_step;

      uns32 m_lastMove;
      uns8  m_state;

   private:
      CInterpolator();

   public:
      ~CInterpolator();

      static CInterpolator &get();

      void init(int32 step);
      void update();

      void setFinalPrDeg(int32 newFinalPrDeg);
      void setFinalPr(int32 newPr);
      void setStep(int32 newStep);

      int32 getFinalPr();
      int32 getStep();

      int32 getFinalPrDeg();
};

#endif
