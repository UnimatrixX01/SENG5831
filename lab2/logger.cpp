#include <avr/interrupt.h>
#include "logger.hpp"
#include "pid.hpp"

CLogger CLogger::m_logger;

CLogger::CLogger() {
   m_serial = NULL;
   m_logger_enabled = true;
   m_logger_release = false;
}

CLogger::~CLogger() {
}

CLogger &CLogger::get() {
   return m_logger;
}

void CLogger::setEnabled(boolean isEnabled) {
   m_logger_enabled = isEnabled;
}

void CLogger::init(CSerial *serial) {

   /* Save the serial object so the logger knows where to send messages    */

   m_serial = serial;

   /************************************************************************/
   /* Configure the logger to use timer 3 with a 1 second period           */
   /************************************************************************/

   /* Using CTC mode with OCR3 for TOP. WGMn3/2/1/0 to 0/1/0/0             */

   TCCR3B &= ~(1 << WGM13);
   TCCR3B |=  (1 << WGM12);
   TCCR3A &= ~(1 << WGM11);
   TCCR3A &= ~(1 << WGM10);

   /* Using pre-scaler 1024.                                               */

   TCCR3B |=  (1 << CS12);
   TCCR3B &= ~(1 << CS11);
   TCCR3B |=  (1 << CS10);

   /* Interrupt Frequency: 1 = f_IO / (x*OCR1A)                            */
   /* 20000000/(1*x) = OCR3A                                               */

   OCR3A = 19531;

   /* Enable output compare match interrupt on timer 1B                    */

   TIMSK3 |= (1 << OCIE3A);
};

void CLogger::update() {
   if (m_logger_release) {
      if ((m_serial != NULL) && m_logger_enabled) {
         CPID pid = CPID::get();
         m_serial->printf("Pr: %ld, Pm: %ld, Kd: %ld, Kp: %ld, Vm: %ld, T: %ld\r\n", 
                 pid.getPr(), pid.getPm(), pid.getKd(), pid.getKp(), pid.getVm(), pid.getT());
      }
      m_logger_release = false;
   }
}

void CLogger::release() {
   m_logger_release = true;
}

/* -------------------------  LOGGER ISR --------------------------------- */

ISR(TIMER3_COMPA_vect) {
   CLogger::get().release();
}
/*
      if (log_release) {
         sprintf(buffer, "Pr: %ld, Pm: %ld, Kd: %ld, Kp: %ld, Vm: %ld, T: %ld\r\n", 
                 Pr, Pm, Kd, Kp, Vm, T);
         print_usb(buffer, 0);
         log_release=0;
      }

   */
