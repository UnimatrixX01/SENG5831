#include <stdio.h>
#include "cmd_processor.hpp"
#include "logger.hpp"
#include "serial.hpp"
#include "pid.hpp"

CCommandProcessor::CCommandProcessor() {
   m_serial = NULL;
}

CCommandProcessor::~CCommandProcessor() {
}

void CCommandProcessor::setSerial(CSerial *serial) {
   m_serial = serial;
}

void CCommandProcessor::onCommand(const char *cmd, boolean overflow) {
   char chr;
   int param;
   uns8 count;
   int32 tmp;

   if (m_serial == NULL) {
      return;
   }

   if (overflow) {
      m_serial->printf("Command was too long and was truncated!\r\n");
   }

   chr = '\0';
   count = sscanf(cmd, "%c %d", &chr, &param);

   if (count > 0) {
      switch(chr) {
         case 'L':
            CLogger::get().setEnabled(true);
            m_serial->printf("Logging enabled!\r\n");
            break;
         case 'l':
            CLogger::get().setEnabled(false);
            m_serial->printf("Logging disabled!\r\n");
            break;

         case 'R':
            tmp = CPID::get().getPr();
            if (count == 2) {
               CPID::get().setPr(tmp+param);
            } else {
               CPID::get().setPr(tmp+1);
            }
            m_serial->printf("Pr changed from %ld to %ld\r\n", tmp, CPID::get().getPr());
            break;
         case 'r':
            tmp = CPID::get().getPr();
            if (count == 2) {
               CPID::get().setPr(tmp-param);
            } else {
               CPID::get().setPr(tmp-1);
            }
            m_serial->printf("Pr changed from %ld to %ld\r\n", tmp, CPID::get().getPr());
            break;

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

         default:
            m_serial->printf("Unknown command: %s\r\n", cmd);
            break;
      }
   }
}
