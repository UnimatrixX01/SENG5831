#ifndef _CMD_PROCESSOR_HPP
#define _CMD_PROCESSOR_HPP

#include "typedef.h"
#include "serial_callback.hpp"
#include "serial.hpp"

/* This class processes a command when a user types something in via the   */
/* serial line.  onCommand is called with the command that the user typed  */
/* and an error flag indicating that the user entered command was too      */
/* long.                                                                   */

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

