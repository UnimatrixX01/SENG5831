#ifndef _SERIAL_CALLBACK_H
#define _SERIAL_CALLBACK_H

class CSerialCallback {
   public:
      virtual void onCommand(const char *cmd, boolean overflow)=0;
};

#endif
