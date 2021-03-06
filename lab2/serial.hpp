#ifndef _SERIAL_HPP
#define _SERIAL_HPP

#include "typedef.h"
#include "serial_callback.hpp"

#define REC_BUFF_SIZE 64

/* This class is responsible for managing the serial USB interface.  It    */
/* can output a string using standard printf syntax, and it also parses    */
/* user input to form commands passed back to the application via a        */
/* callback.                                                               */

class CSerial {
   private:
      char m_recv_buff[REC_BUFF_SIZE];
      uns8 m_recv_pos;

      char m_cmd[REC_BUFF_SIZE];
      uns8 m_cmd_pos;
      boolean m_cmd_overflow;

      boolean m_echo_print;

      CSerialCallback *m_callback;

   public:
      CSerial(boolean echoPrint);
      CSerial(boolean echoPrint, CSerialCallback *callback);
      ~CSerial();

      void setCallback(CSerialCallback *newCallback);
      void setEchoPrint(boolean newEchoPrint);

      void init();
      void printf(const char *format, ...);
      void waitForSend();

      void update();
};

#endif
