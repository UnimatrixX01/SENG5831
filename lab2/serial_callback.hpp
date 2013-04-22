#ifndef _SERIAL_CALLBACK_H
#define _SERIAL_CALLBACK_H

/* This interface is used by the serial class to send a notification to    */
/* the application when the user typed in a command.                       */

class CSerialCallback {
   public:
      virtual void onCommand(const char *cmd, boolean overflow)=0;
};

#endif
