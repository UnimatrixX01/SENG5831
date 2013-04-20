#ifndef _PID_HPP
#define _PID_HPP

#include "typedef.h"
#include "motor.hpp"
#include "encoder.hpp"

class CPID {
   private:
      static CPID m_pid;

      CMotor *m_motor;
      CEncoder *m_encoder;

      int32 m_Pr;
      int32 m_Pm;
      int32 m_Kd;
      int32 m_Kp;
      int32 m_Vm;

      int32 m_T;

   private:
      CPID();

   public:
      ~CPID();

      static CPID &get();

      void init(CMotor *motor, CEncoder *encoder);
      void update();

      void setPr(int32 newPr);
      void setKd(int32 newKd);
      void setKp(int32 newKp);

      int32 getPr();
      int32 getPm();
      int32 getKd();
      int32 getKp();
      int32 getVm();
      int32 getT();
};

#endif

