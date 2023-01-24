/*
 * File: Autopilot.c
 *
 * Code generated for Simulink model 'Autopilot'.
 *
 * Model version                  : 1.152
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:38:36 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Autopilot.h"
#include "Autopilot_private.h"

/* Start for referenced model: 'Autopilot' */
void Autopilot_Start(rtDW_Autopilot *localDW)
{
  /* Start for ModelReference: '<Root>/Pitch_Autopilot' */
  pitch_ap_Start(&(localDW->Pitch_Autopilot_DWORK1.rtdw));
}

/* Output and update for referenced model: 'Autopilot' */
void Autopilot(const slBus1 *rtu_AirData, const slBus2 *rtu_Inertial, const
               boolean_T *rtu_APEng, const boolean_T *rtu_HDGMode, const
               boolean_T *rtu_ALTMode, const real_T *rtu_HDGRef, const real_T
               *rtu_TurnKnob, const real_T *rtu_ALTRef, const real_T
               *rtu_PitchWheel, real_T *rty_AileronCmd, real_T *rty_ElevatorCmd,
               real_T *rty_RudderCmd, rtDW_Autopilot *localDW)
{
  /* local block i/o variables */
  real_T rtb_airspeed;
  real_T rtb_phi;
  real_T rtb_theta;
  real_T rtb_q;
  real_T rtb_r;
  real_T rtb_alt;
  real_T rtb_altRate;
  real_T rtb_psi;
  real_T rtb_p;

  /* BusSelector: '<Root>/Bus Selector3' */
  rtb_alt = rtu_AirData->alt;
  rtb_altRate = rtu_AirData->altRate;
  rtb_airspeed = rtu_AirData->airspeed;

  /* BusSelector: '<Root>/Bus Selector4' */
  rtb_phi = rtu_Inertial->phi;
  rtb_psi = rtu_Inertial->psi;
  rtb_p = rtu_Inertial->p;
  rtb_theta = rtu_Inertial->theta;
  rtb_q = rtu_Inertial->q;
  rtb_r = rtu_Inertial->r;

  /* ModelReference: '<Root>/Pitch_Autopilot' */
  pitch_ap(&rtb_phi, &rtb_theta, &rtb_q, &rtb_r, &rtb_alt, &rtb_altRate,
           &rtb_airspeed, rtu_APEng, rtu_ALTMode, rtu_ALTRef, rtu_PitchWheel,
           rty_ElevatorCmd, &(localDW->Pitch_Autopilot_DWORK1.rtb),
           &(localDW->Pitch_Autopilot_DWORK1.rtdw), 2.0, 0.5, 2.0);

  /* ModelReference: '<Root>/Roll_Autopilot' */
  roll_ap(&rtb_phi, &rtb_psi, &rtb_p, &rtb_airspeed, rtu_APEng, rtu_HDGMode,
          rtu_HDGRef, rtu_TurnKnob, rty_AileronCmd,
          &(localDW->Roll_Autopilot_DWORK1.rtb),
          &(localDW->Roll_Autopilot_DWORK1.rtdw), 1.0, 1.0, 3.0, 0.015);

  /* ModelReference: '<Root>/Yaw_Damper' */
  yaw_damper(&rtb_phi, &rtb_r, &rtb_airspeed, rtu_APEng, rty_RudderCmd,
             &(localDW->Yaw_Damper_DWORK1.rtb), 1.0);
}

/* Model initialize function */
void Autopilot_initialize(rtDW_Autopilot *localDW)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(rtDW_Autopilot));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Pitch_Autopilot' */
  pitch_ap_initialize(&(localDW->Pitch_Autopilot_DWORK1.rtb),
                      &(localDW->Pitch_Autopilot_DWORK1.rtdw));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Roll_Autopilot' */
  roll_ap_initialize(&(localDW->Roll_Autopilot_DWORK1.rtb),
                     &(localDW->Roll_Autopilot_DWORK1.rtdw));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Yaw_Damper' */
  yaw_damper_initialize(&(localDW->Yaw_Damper_DWORK1.rtb));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
