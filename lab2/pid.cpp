#include <avr/interrupt.h>
#include "pid.hpp"

CPID CPID::m_pid;

CPID::CPID() {
   m_Pr = 320;
   m_Pm = 0;
   m_Kd = 10;
   m_Kp = 100;
   m_Vm = 0;
   m_T = 0;
}

CPID::~CPID() {
}

CPID &CPID::get() {
   return m_pid;
}

void CPID::init(CMotor *motor, CEncoder *encoder) {
   m_motor = motor;
   m_encoder = encoder;

   /************************************************************************/
   /* Configure the PID controller to use timer 1 at 1000 Hz               */
   /************************************************************************/

   /* Using CTC mode with OCR1 for TOP. WGMn3/2/1/0 to 0/1/0/0             */

   TCCR1B &= ~(1 << WGM13);
   TCCR1B |=  (1 << WGM12);
   TCCR1A &= ~(1 << WGM11);
   TCCR1A &= ~(1 << WGM10);

   /* Using pre-scaler 1.                                                  */

   TCCR1B &= ~(1 << CS12);
   TCCR1B &= ~(1 << CS11);
   TCCR1B |=  (1 << CS10);

   /* Interrupt Frequency: 1000 = f_IO / (PreScale*OCR1A)                  */
   /* 20000000/(1000*1) = OCR1A                                            */

   OCR1A = 20000;

   /* Enable output compare match interrupt on timer 1B if we have a valid */
   /* motor and encoder to control.                                        */

   if (m_motor != NULL && m_encoder != NULL) {
      TIMSK1 |= (1 << OCIE1A);
   }
   else {
      TIMSK1 &= ~(1 << OCIE1A);
   }
}

void CPID::setPr(int32 newPr) {
   m_Pr = newPr;
}

void CPID::setKd(int32 newKd) {
   m_Kd = newKd;
}

void CPID::setKp(int32 newKp) {
   m_Kp = newKp;
}


int32 CPID::getPr() {
   return m_Pr;
}

int32 CPID::getPm() {
   return m_Pm;
}

int32 CPID::getKd() {
   return m_Kd;
}

int32 CPID::getKp() {
   return m_Kp;
}

int32 CPID::getVm() {
   return m_Vm;
}

int32 CPID::getT() {
   return m_T;
}


void CPID::update() {
   int32 Lm = m_Pm;

   m_Pm = m_encoder->getPosition();
   m_Vm = (m_Pm - Lm) * 1000;

   m_T = m_Kp*(m_Pr - m_Pm) - m_Kd*m_Vm;

   m_motor->setSpeed(m_T);
}

ISR(TIMER1_COMPA_vect) {
   CPID::get().update();
}
