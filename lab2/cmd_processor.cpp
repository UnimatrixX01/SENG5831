#include <stdio.h>
#include "cmd_processor.hpp"
#include "logger.hpp"
#include "serial.hpp"
#include "pid.hpp"
#include "interpolator.hpp"

CCommandProcessor::CCommandProcessor() {
   m_serial = NULL;
}

CCommandProcessor::~CCommandProcessor() {
}

void CCommandProcessor::setSerial(CSerial *serial) {
   m_serial = serial;
}

void CCommandProcessor::onCommand(const char *cmd, boolean overflow) {
   const char *onoff[] = {
      "OFF",
      "ON"
   };
   char chr;
   int param;
   uns8 count;
   int32 tmp;

   /* If there's no device to respond on, then don't do anything.          */

   if (m_serial == NULL) {
      return;
   }

   /* Check if the user typed a long enough string that we truncated the   */
   /* result.                                                              */

   if (overflow) {
      m_serial->printf("Command was too long and was truncated!\r\n");
   }

   /* Parse the command.                                                   */

   chr = '\0';
   count = sscanf(cmd, "%c %d", &chr, &param);

   if (count > 0) {
      switch(chr) {

         /* 'L' enables logging                                            */
         /* 'l' disables logging                                           */

         case 'L':
            CLogger::get().setEnabled(true);
            m_serial->printf("Logging enabled!\r\n");
            break;
         case 'l':
            CLogger::get().setEnabled(false);
            m_serial->printf("Logging disabled!\r\n");
            break;

         /* 'V' or 'v' causes the logger to print one line.                */

         case 'V':
         case 'v':
            CLogger::get().release();
            break;

         /* 'R' moves the motor a parameter (default 1) ticks forward.     */
         /* 'r' moves the motor a parameter (default 1) ticks backwards.   */

         case 'R':
            tmp = CInterpolator::get().getFinalPr();
            if (count == 2) {
               CInterpolator::get().setFinalPr(tmp+param);
            } else {
               CInterpolator::get().setFinalPr(tmp+1);
            }
            m_serial->printf("FinalPr changed from %ld to %ld\r\n", tmp, CInterpolator::get().getFinalPr());
            break;
         case 'r':
            tmp = CInterpolator::get().getFinalPr();
            if (count == 2) {
               CInterpolator::get().setFinalPr(tmp-param);
            } else {
               CInterpolator::get().setFinalPr(tmp-1);
            }
            m_serial->printf("FinalPr changed from %ld to %ld\r\n", tmp, CInterpolator::get().getFinalPr());
            break;

         /* 'P' increases the Kp value by a parameter (default 1)          */
         /* 'p' decreases the Kp value by a parameter (default 1)          */

         case 'P':
            tmp = CPID::get().getKp();
            if (count == 2) {
               CPID::get().setKp(tmp+param);
            } else {
               CPID::get().setKp(tmp+1);
            }
            m_serial->printf("Kp changed from %ld to %ld\r\n", tmp, CPID::get().getKp());
            break;
         case 'p':
            tmp = CPID::get().getKp();
            if (count == 2) {
               CPID::get().setKp(tmp-param);
            } else {
               CPID::get().setKp(tmp-1);
            }
            m_serial->printf("Kp changed from %ld to %ld\r\n", tmp, CPID::get().getKp());
            break;

         /* 'D' increases the Kd value by a parameter (default 1)          */
         /* 'd' decreases the Kd value by a parameter (default 1)          */

         case 'D':
            tmp = CPID::get().getKd();
            if (count == 2) {
               CPID::get().setKd(tmp+param);
            } else {
               CPID::get().setKd(tmp+1);
            }
            m_serial->printf("Kd changed from %ld to %ld\r\n", tmp, CPID::get().getKd());
            break;
         case 'd':
            tmp = CPID::get().getKd();
            if (count == 2) {
               CPID::get().setKd(tmp-param);
            } else {
               CPID::get().setKd(tmp-1);
            }
            m_serial->printf("Kd changed from %ld to %ld\r\n", tmp, CPID::get().getKd());
            break;

         /* 'C' and 'c' toggle CSV output by the logger.                   */

         case 'c':
         case 'C':
            tmp = CLogger::get().isCSV();
            CLogger::get().toggleCSV();
            m_serial->printf("CSV Output changed from %s to %s\r\n", onoff[tmp], onoff[CLogger::get().isCSV()]);
            break;

         default:
            m_serial->printf("Unknown command: %s\r\n", cmd);
            break;
      }
   }
}
