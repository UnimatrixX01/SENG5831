#include <avr/interrupt.h>
#include "logger.hpp"
#include "pid.hpp"
#include "interpolator.hpp"
#include "timer.hpp"

CLogger CLogger::m_logger;

CLogger::CLogger() {
   m_serial = NULL;
   m_logger_enabled = true;
   m_logger_release = false;
   m_csv = true;
}

CLogger::~CLogger() {
}

CLogger &CLogger::get() {
   return m_logger;
}

void CLogger::setEnabled(boolean isEnabled) {
   m_logger_enabled = isEnabled;
}

boolean CLogger::isEnabled() {
   return m_logger_enabled;
}

boolean CLogger::isCSV() {
   return m_csv;
}

void CLogger::toggleCSV() {
   m_csv = !m_csv;
   if (m_csv) {
      printHeader();
   }
}

void CLogger::printHeader() {
   if (m_serial != NULL) {
      m_serial->printf("Time,Final Dest Ticks,Final Dest Deg,Pr Ticks,Pr Deg,Pm Ticks,Pm Deg,Kp,Kd,Vm,T\r\n");
   }
}

void CLogger::init(CSerial *serial) {

   /* Save the serial object so the logger knows where to send messages    */

   m_serial = serial;

   if (m_csv) {
      printHeader();
   }

   /************************************************************************/
   /* Configure the logger to use timer 1 with a 10 milli-second period    */
   /************************************************************************/

   /* Using CTC mode with OCR1 for TOP.                                    */

   TCCR1B &= ~(1 << WGM13);
   TCCR1B |=  (1 << WGM12);
   TCCR1A &= ~(1 << WGM11);
   TCCR1A &= ~(1 << WGM10);

   /* Using pre-scaler 1024.                                               */

   TCCR1B |=  (1 << CS12);
   TCCR1B &= ~(1 << CS11);
   TCCR1B |=  (1 << CS10);

   /* Interrupt Frequency: 100 = f_IO / (x*OCR1A)                          */
   /* 20000000/(100*x) = OCR3A                                             */

   OCR1A = 195;

   /* Enable output compare match interrupt on timer 1A                    */

   TIMSK1 |= (1 << OCIE1A);
};

void CLogger::update() {
   char *format;

   /* If it's time to print something to the user, then for the love of    */
   /* god we better print it.                                              */

   if (m_logger_release) {
      if (m_serial != NULL) {
         CPID pid = CPID::get();

         /* Print in CSV if preferred, otherwise print non-CSV.            */

         if (m_csv) {
            format = "%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\r\n";
         } else {
            format = "%ld: Final: %ld/%ld, Pr: %ld/%ld, Pm: %ld/%ld, Kp: %ld, Kd: %ld, Vm: %ld, T: %ld\r\n";
         }

         m_serial->printf(format, 
                 CTimer::get().getTime(), 
                 CInterpolator::get().getFinalPr(), CInterpolator::get().getFinalPrDeg(),
                 pid.getPr(), pid.getPrDeg(),
                 pid.getPm(), pid.getPmDeg(),
                 pid.getKp(), pid.getKd(), pid.getVmDeg(), pid.getT());
      }
      m_logger_release = false;
   }
}

void CLogger::release() {
   m_logger_release = true;
}

/* -------------------------  LOGGER ISR --------------------------------- */

ISR(TIMER1_COMPA_vect) {
   CTimer::get().update(10);

   if (CLogger::get().isEnabled()) {
      CLogger::get().release();
   }
}
