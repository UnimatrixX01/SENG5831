#include <avr/interrupt.h>
#include "pid.hpp"
#include "interpolator.hpp"
#include "encoder.hpp"
#include "timer.hpp"

CInterpolator CInterpolator::m_interpolator;

CInterpolator::CInterpolator() {
   m_finalPr = 0;
   m_step = 64;

   m_lastMove = 0;
   m_state = 0;
}

CInterpolator::~CInterpolator() {
}

CInterpolator &CInterpolator::get() {
   return m_interpolator;
}

void CInterpolator::init(int32 step) {
   m_step = step;

   /************************************************************************/
   /* Configure the interpolator to use timer 0 at 100 Hz                  */
   /************************************************************************/

   /* Using CTC mode with OCR0A for TOP.                                   */

   TCCR0B &= ~(1 << WGM02);
   TCCR0A |=  (1 << WGM01);
   TCCR0A &= ~(1 << WGM00);

   /* Using pre-scaler 1024.                                               */

   TCCR0B |=  (1 << CS02);
   TCCR0B &= ~(1 << CS01);
   TCCR0B |=  (1 << CS00);

   /* Interrupt Frequency: 100 = f_IO / (PreScale*OCR1A)                   */
   /* 20000000/(100*1024) = OCR1A                                          */

   OCR0A = 193;

   /* Enable output compare match interrupt on timer 0A if we have a valid */
   /* motor and encoder to control.                                        */

   TIMSK0 |= (1 << OCIE0A);
}

void CInterpolator::setStep(int32 newStep) {
   m_step = newStep;
}

int32 CInterpolator::getStep() {
   return m_step;
}

void CInterpolator::setFinalPr(int32 newFinalPr) {
   m_finalPr = newFinalPr;
}

void CInterpolator::setFinalPrDeg(int32 newFinalPrDeg) {
   m_finalPr = CEncoder::degreesToTicks(newFinalPrDeg);
}

int32 CInterpolator::getFinalPr() {
   return m_finalPr;
}

int32 CInterpolator::getFinalPrDeg() {
   return CEncoder::ticksToDegrees(m_finalPr);
}

void CInterpolator::update() {
   int32 Pm = CEncoder::get().getPosition();
   int32 delta;

   /* The following code was used for the last excersize questions, to     */
   /* have the motor behave a certain way                                  */
   /*
   uns32 ct = CTimer::get().getTime();
   if (m_finalPr == Pm && m_state == 0) {
      m_state = 1;
      m_lastMove = ct;
      // Rotate forward 360 degrees
      m_finalPr = 64;
   }
   else if (m_finalPr != Pm && m_state == 1) {
      m_state = 0;
   }
   if (m_state == 1 && (ct - m_lastMove) >= 500) {
      m_state = 2;
   }


   if (m_finalPr == Pm && m_state == 2) {
      m_state = 3;
      m_lastMove = ct;
      // Rotate backwards 360 degrees
      m_finalPr = 0;
   }
   else if (m_finalPr != Pm && m_state == 3) {
      m_state = 2;
   }
   if (m_state == 3 && (ct - m_lastMove) >= 500) {
      m_state = 4;
   }

   if (m_finalPr == Pm && m_state == 4) {
      m_state = 5;
      m_lastMove = ct;
      // Rotate forward 5 degrees
      m_finalPr = 1;
   }
   else if (m_finalPr != Pm && m_state == 5) {
      m_state = 4;
   }
   if (m_state == 5 && (ct - m_lastMove) >= 500) {
      m_state = 6;
   }
   */

   delta = m_finalPr - Pm;
   if (delta < -m_step) {
      delta = -m_step;
   }
   else if (delta > m_step) {
      delta = m_step;
   }

   /* Update the PID controller with the new destination position          */

   CPID::get().setPr(Pm + delta);
}

ISR(TIMER0_COMPA_vect) {
   CInterpolator::get().update();
}

