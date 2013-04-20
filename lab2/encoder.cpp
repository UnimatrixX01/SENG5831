#include <avr/interrupt.h>
#include "encoder.hpp"

// Motor Encoder Pins
#define INT_M2ENC1  (PCMSK3)
#define PIN_M2ENC1  (PIND)
#define DDR_M2ENC1  (DDRD)
#define BIT_M2ENC1  (1 << PORTD3)
#define PCC_M2ENC1  (1 << PCIE3)
#define PCF_M2ENC1  (1 << PCIF3)

#define INT_M2ENC2  (PCMSK3)
#define PIN_M2ENC2  (PIND)
#define DDR_M2ENC2  (DDRD)
#define BIT_M2ENC2  (1 << PORTD2)
#define PCC_M2ENC2  (1 << PCIE3)
#define PCF_M2ENC2  (1 << PCIF3)

#define TICKS_PER_ROTATION (64)

CEncoder CEncoder::m_encoder;

CEncoder::CEncoder() {
}

CEncoder::~CEncoder() {
}

CEncoder &CEncoder::get() {
    return m_encoder;
}

void CEncoder::init() {
   /* -------------------------  MOTOR ENCODER --------------------------- */

   /* Enable Pin Change Interrupt Control Registers.                       */

   DDR_M2ENC1 &= ~BIT_M2ENC1;
   INT_M2ENC1 |=  BIT_M2ENC1;
   PCICR      |=  PCC_M2ENC1;
   PCIFR      |=  PCF_M2ENC1;

   DDR_M2ENC2 &= ~BIT_M2ENC2;
   INT_M2ENC2 |=  BIT_M2ENC2;
   PCICR      |=  PCC_M2ENC2;
   PCIFR      |=  PCF_M2ENC2;

   /* Store the current state of the encoder pins.                         */

   m_last_pin1 = PIN_M2ENC1 & BIT_M2ENC1;
   m_last_pin2 = PIN_M2ENC2 & BIT_M2ENC2;

   /* Initialize the position to 0 with no error.                          */

   m_position = 0;
   m_error    = false;
}

int32 CEncoder::getPosition() {
    return m_position;
}

boolean CEncoder::getError() {
    return m_error;
}

void CEncoder::clearError() {
    m_error = false;
}

void CEncoder::update() {
   uns8 pin1 = PIN_M2ENC1 & BIT_M2ENC1;
   uns8 pin2 = PIN_M2ENC2 & BIT_M2ENC2;

   uns8 inc = pin1 ^ m_last_pin2;
   uns8 dec = pin2 ^ m_last_pin1;

   if (inc) {
      m_position++;
   }
   if (dec) {
      m_position--;
   }

   if (pin1 != m_last_pin1 && pin2 != m_last_pin2) {
      m_error = true;
   }

   m_last_pin1 = pin1;
   m_last_pin2 = pin2;
}

ISR(PCINT3_vect) {
    CEncoder::get().update();
}
