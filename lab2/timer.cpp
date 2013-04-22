#include "timer.hpp"

CTimer CTimer::m_timer;


CTimer::CTimer() {
}


CTimer::~CTimer() {
}


CTimer &CTimer::get() {
   return m_timer;
}


void CTimer::update(uns32 period) {

   /* Update the current time with the number of ellapsed milliseconds.    */

   m_time += period;
}


uns32 CTimer::getTime() {
   return m_time;
}

