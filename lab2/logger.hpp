#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include "typedef.h"
#include "serial.hpp"

class CLogger {
   private:
      static CLogger m_logger;
      boolean m_logger_enabled;
      boolean m_logger_release;

      CSerial *m_serial;

   private:
      CLogger();

   public:
      ~CLogger();

      static CLogger &get();

      void init(CSerial *serial);
      void setEnabled(boolean isEnabled);
      void update();
      void release();
};

#endif

