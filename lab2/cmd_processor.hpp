#ifndef _CMD_PROCESSOR_HPP
#define _CMD_PROCESSOR_HPP

#include "typedef.h"
#include "serial_callback.hpp"
#include "serial.hpp"

class CCommandProcessor : public CSerialCallback {
   private:
      CSerial *m_serial;

   public:
      CCommandProcessor();
      ~CCommandProcessor();

      void setSerial(CSerial *serial);
      void onCommand(const char *cmd, boolean overflow);
};

#endif

