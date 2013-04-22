#include <pololu/orangutan.h>  
#include <avr/interrupt.h>
#include "pid.hpp"
#include "logger.hpp"
#include "encoder.hpp"

CPID CPID::m_pid;

CPID::CPID() {
   m_Pr = 1;
   m_Pm = 0;
   m_Kp = 32;
   m_Kd = 4;
   m_Vm = 0;
   m_T = 0;
   m_PmVm = 0;
   m_vel_upd_cnt = 0;
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
   /* Configure the PID controller to use timer 3 at 1000 Hz               */
   /************************************************************************/

   /* Using CTC mode with OCR3A for TOP.                                   */

   TCCR3B &= ~(1 << WGM33);
   TCCR3B |=  (1 << WGM32);
   TCCR3A &= ~(1 << WGM31);
   TCCR3A &= ~(1 << WGM30);

   /* Using pre-scaler 1.                                                  */

   TCCR3B &= ~(1 << CS32);
   TCCR3B &= ~(1 << CS31);
   TCCR3B |=  (1 << CS30);

   /* Interrupt Frequency: 1000 = f_IO / (PreScale*OCR3A)                  */
   /* 20000000/(1000*1) = OCR3A                                            */
   // 20e6/(1*50) = ?

   OCR3A = 20000;

   // 50 Hz PID Controller
   // prescaler 8
   //TCCR3B &= ~(1 << CS32);
   //TCCR3B |=  (1 << CS31);
   //TCCR3B &= ~(1 << CS30);
   //OCR3A = 50000;

   // 5 Hz PID Controller
   // prescaler 64
   //TCCR3B &= ~(1 << CS32);
   //TCCR3B |=  (1 << CS31);
   //TCCR3B |=  (1 << CS30);
   //OCR3A = 62500;


   /* Enable output compare match interrupt on timer 1B if we have a valid */
   /* motor and encoder to control.                                        */

   if (m_motor != NULL && m_encoder != NULL) {
      TIMSK3 |= (1 << OCIE3A);
   }
   else {
      TIMSK3 &= ~(1 << OCIE3A);
   }
}

void CPID::setPr(int32 newPr) {
   // Automatically start logging when the Pr is changed
   if(newPr != m_Pm) {
      CLogger::get().setEnabled(true);
   }
   m_Pr = newPr;
}

void CPID::setPrDeg(int32 newPrDeg) {
   newPrDeg = CEncoder::degreesToTicks(newPrDeg);

   // Automatically start logging when the Pr is changed
   if(newPrDeg != m_Pm) {
      CLogger::get().setEnabled(true);
   }
   m_Pr = newPrDeg;
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

int32 CPID::getPrDeg() {
   return CEncoder::ticksToDegrees(m_Pr);
}

int32 CPID::getPmDeg() {
   return CEncoder::ticksToDegrees(m_Pm);
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

int32 CPID::getVmDeg() {
   return CEncoder::ticksToDegrees(m_Vm);
}

int32 CPID::getT() {
   return m_T;
}


void CPID::update() {
   m_Pm = m_encoder->getPosition();

   // Velocity is in ticks/100ms

   /*
   // 5 Hz PID Loop
   if (++m_vel_upd_cnt) {
      m_Vm = (m_Pm - m_PmVm)/2;
      m_PmVm = m_Pm;
      m_vel_upd_cnt = 0;
   }

   // 50 Hz PID Loop
   if (++m_vel_upd_cnt >= 5) {
      m_Vm = (m_Pm - m_PmVm);
      m_PmVm = m_Pm;
      m_vel_upd_cnt = 0;
   }
   */

   // 1000 Hz PID Loop
   if (++m_vel_upd_cnt >= 50) {
      m_Vm = (m_Pm - m_PmVm)*2;
      m_PmVm = m_Pm;
      m_vel_upd_cnt = 0;
   }


   /* Calculate the torque                                                 */

   m_T = m_Kp*(m_Pr - m_Pm) - m_Kd*m_Vm;

   /* Stop logging if we reach our destination                             */

   if (m_Pr == m_Pm && m_Vm == 0 && m_T == 0) {
      if (CLogger::get().isEnabled()) {
         CLogger::get().release();
      }
      CLogger::get().setEnabled(false);
   }

   /* Set the motor torque                                                 */

   m_motor->setSpeed(m_T);
}

ISR(TIMER3_COMPA_vect) {
   CPID::get().update();
}
