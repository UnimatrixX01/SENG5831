#ifndef _PID_HPP
#define _PID_HPP

#include "typedef.h"
#include "motor.hpp"
#include "encoder.hpp"

/* This class is responsible for running the PID to control the position   */
/* of the motor.                                                           */

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

      int32 m_PmVm;

      uns16 m_vel_upd_cnt;

   private:
      CPID();

   public:
      ~CPID();

      static CPID &get();

      void init(CMotor *motor, CEncoder *encoder);
      void update();

      void setPrDeg(int32 newPrDeg);
      void setPr(int32 newPr);
      void setKd(int32 newKd);
      void setKp(int32 newKp);

      int32 getPr();
      int32 getPm();
      int32 getKd();
      int32 getKp();
      int32 getVm();
      int32 getT();

      int32 getPmDeg();
      int32 getPrDeg();
      int32 getVmDeg();
};

#endif

