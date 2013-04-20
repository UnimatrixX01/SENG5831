#include <pololu/orangutan.h>  
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "serial.hpp"

CSerial::CSerial(boolean echoPrint) {
   m_callback = NULL;
   memset(m_recv_buff, 0, REC_BUFF_SIZE);
   memset(m_cmd, 0, REC_BUFF_SIZE);
   m_recv_pos = 0;
   m_cmd_pos = 0;
   m_cmd_overflow = false;
   m_echo_print = echoPrint;
}

CSerial::CSerial(boolean echoPrint, CSerialCallback *callback) {
   m_callback = callback;
   memset(m_recv_buff, 0, REC_BUFF_SIZE);
   memset(m_cmd, 0, REC_BUFF_SIZE);
   m_recv_pos = 0;
   m_cmd_pos = 0;
   m_cmd_overflow = false;
   m_echo_print = echoPrint;
}

CSerial::~CSerial() {
}

void CSerial::init() {
   serial_set_baud_rate(USB_COMM, 9600);
   serial_receive_ring(USB_COMM, m_recv_buff, REC_BUFF_SIZE);
}

void CSerial::setCallback(CSerialCallback *newCallback) {
   m_callback = newCallback;
}

void CSerial::setEchoPrint(boolean newEchoPrint) {
   m_echo_print = newEchoPrint;
}

void CSerial::waitForSend() {
   while(!serial_send_buffer_empty(USB_COMM)) {
      serial_check();
   }
}

void CSerial::printf(const char *format, ...) {
   va_list args;
   int16 len;
   char *buffer = NULL;

   va_start(args, format);
   len = vsnprintf(NULL, 0, format, args);
   va_end(args);

   if (len >= 0) {
      buffer = (char *)malloc(len+1);
      va_start(args, format);
      vsnprintf(buffer, len+1, format, args);
      va_end(args);

      serial_send( USB_COMM, buffer, len+1 );
      waitForSend();

      free(buffer);
   }
}

void CSerial::update() {
   serial_check();

   while (serial_get_received_bytes(USB_COMM) != m_recv_pos) {
      m_cmd[m_cmd_pos] = m_recv_buff[m_recv_pos];

      if (m_echo_print) {
         printf("%c", m_recv_buff[m_recv_pos]);
      }

      if (m_cmd[m_cmd_pos] == '\b') {
         if (m_cmd_pos > 0) {
            m_cmd_pos--;
         }
      } else if ((m_callback != NULL) &&
                 ((m_cmd[m_cmd_pos] == '\r') ||
                  (m_cmd[m_cmd_pos] == '\n'))) {
         m_cmd[m_cmd_pos] = '\0';
         m_callback->onCommand(m_cmd, m_cmd_overflow);
         m_cmd_pos = 0;
         m_cmd_overflow = false;
      } else if (m_cmd_pos < REC_BUFF_SIZE - 1) {
         m_cmd_pos++;
      } else {
         m_cmd_overflow = true;
      }

      m_recv_pos = ((m_recv_pos + 1) % REC_BUFF_SIZE);
   }
}

extern "C" {
   void __cxa_pure_virtual() {
      while (1);
   }
}
