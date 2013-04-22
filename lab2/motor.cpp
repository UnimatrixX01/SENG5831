#include <avr/interrupt.h>
#include "motor.hpp"

/* Motor On/Off Pin.  This is connected to port OC2B on timer 2.           */

#define PRT_M2      (PORTD)
#define DDR_M2      (DDRD)
#define BIT_M2      (1 << PORTD6)

/* Motor Direction Pin                                                     */

#define PRT_M2DIR   (PORTC)
#define DDR_M2DIR   (DDRC)
#define BIT_M2DIR   (1 << PORTC6)

CMotor::CMotor() {
    setStillThreshold(10);
    setMaxSpeed(0xFF);
}

CMotor::~CMotor() {
}

void CMotor::init() {
   /* Configure the Motor PWM to use timer 2.                              */

   /* Configure the motor for fast PWM.                                    */

   TCCR2B &= ~(1 << WGM22);
   TCCR2A |=  (1 << WGM21);
   TCCR2A |=  (1 << WGM20);

   /* Set the prescaler to 8                                               */

   TCCR2B &= ~(1 << CS22);
   TCCR2B |=  (1 << CS21);
   TCCR2B &= ~(1 << CS20);

   /* Set motor output direction ports                                     */

   DDR_M2     |= BIT_M2;
   PRT_M2     &= ~BIT_M2;
   DDR_M2DIR  |= BIT_M2DIR;
   PRT_M2DIR  &= ~BIT_M2DIR;

   /* The motor starts out with 0 speed (ie, OC2B is not connected to the  */
   /* timer)                                                               */
}

void CMotor::setStillThreshold(uns8 thresh) {
    m_still_thresh = thresh;
}

void CMotor::setMaxSpeed(uns32 newMaxSpeed) {
    m_max_speed = newMaxSpeed;
}

void CMotor::setSpeed(int32 newSpeed) {

   /* Clamp the speed to the max speed.                                    */

   if (newSpeed < -m_max_speed) {
      newSpeed = -m_max_speed;
   }
   else if (newSpeed > m_max_speed) {
      newSpeed = m_max_speed;
   }

   /* Set the direction and motor PWM.                                     */

   if (newSpeed < 0) {
      OCR2B = -newSpeed;
      PRT_M2DIR |= BIT_M2DIR;
   } else {
      OCR2B = newSpeed;
      PRT_M2DIR &= ~BIT_M2DIR;
   }

   /* If the speed is slow enough, disconnect the motor.  Otherwise,       */
   /* connect the timer to pin OC2B.  Set the timer to clear Oc2B on       */
   /* compare match and set OC2B at bottom (non-inverting mode).           */

   if (OCR2B < m_still_thresh) {
      TCCR2A &= ~(1<<COM2B1);
   } else {
      TCCR2A |= 1<<COM2B1;
   }
}
