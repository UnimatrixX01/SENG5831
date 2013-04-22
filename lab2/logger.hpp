#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include "typedef.h"
#include "serial.hpp"

/* This class will occasionally print system state to the user             */
/* periodically.                                                           */

class CLogger {
   private:
      static CLogger m_logger;
      boolean m_logger_enabled;
      boolean m_logger_release;

      boolean m_csv;

      CSerial *m_serial;

   private:
      CLogger();

   public:
      ~CLogger();

      static CLogger &get();

      boolean isEnabled();
      void init(CSerial *serial);
      void setEnabled(boolean isEnabled);
      void update();
      void release();

      void toggleCSV();
      boolean isCSV();

   private:
      void printHeader();
};

#endif

