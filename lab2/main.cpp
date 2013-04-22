#include <avr/interrupt.h>

#include "motor.hpp"
#include "encoder.hpp"
#include "serial.hpp"
#include "serial_callback.hpp"
#include "logger.hpp"
#include "pid.hpp"
#include "cmd_processor.hpp"
#include "interpolator.hpp"

int main() {
   CMotor motor;
   CSerial serial(false);
   CCommandProcessor cmd;

   /* First, initialize serial communications.                             */

   serial.setCallback(&cmd);
   serial.init();
   serial.printf("Serial Initialized\r\n");
   cmd.setSerial(&serial);

   /* Then initialize other components.                                    */

   motor.init();
   CEncoder::get().init();
   CLogger::get().init(&serial);
   CPID::get().init(&motor, &CEncoder::get());
   CInterpolator::get().init(8);

   /* Finally, enable interrupts                                           */

   sei();

   /* Most things are done in the ISRs.  But check for serial messages and */
   /* print log messages here.                                             */

   while (1) {
      serial.update();
      CLogger::get().update();
   }
}
