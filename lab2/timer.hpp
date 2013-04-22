#ifndef _CTIMER_HPP
#define _CTIMER_HPP

#include "typedef.h"

/* This class is responsible for keeping time on the system.               */

class CTimer {
   private:
      static CTimer m_timer;

      uns32 m_time;

   private:
      CTimer();

   public:
      ~CTimer();

      static CTimer &get();

      void update(uns32 period);

      uns32 getTime();

};

#endif

