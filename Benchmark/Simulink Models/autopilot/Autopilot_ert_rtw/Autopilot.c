/*
 * File: Autopilot.c
 *
 * Code generated for Simulink model 'Autopilot'.
 *
 * Model version                  : 1.152
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:57 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Autopilot.h"
#include "Autopilot_private.h"

const slBus1 Autopilot_rtZslBus1 = {
  0.0,                                 /* altRate */
  0.0,                                 /* alpha */
  0.0,                                 /* beta */
  0.0,                                 /* airspeed */
  0.0                                  /* alt */
} ;                                    /* slBus1 ground */

const slBus2 Autopilot_rtZslBus2 = {
  0.0,                                 /* phi */
  0.0,                                 /* theta */
  0.0,                                 /* psi */
  0.0,                                 /* p */
  0.0,                                 /* q */
  0.0                                  /* r */
} ;                                    /* slBus2 ground */

/* Block states (auto storage) */
D_Work_Autopilot Autopilot_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Autopilot Autopilot_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Autopilot Autopilot_Y;

/* Model step function */
void Autopilot_step(void)
{
  /* local block i/o variables */
  real_T rtb_phi;
  real_T rtb_theta;
  real_T rtb_q;
  real_T rtb_r;
  real_T rtb_airspeed;
  real_T rtb_psi;
  real_T rtb_p;
  real_T rtb_alt;
  real_T rtb_altRate;

  /* BusSelector: '<Root>/Bus Selector4' incorporates:
   *  Inport: '<Root>/Inertial'
   */
  rtb_phi = Autopilot_U.Inertial.phi;
  rtb_psi = Autopilot_U.Inertial.psi;
  rtb_p = Autopilot_U.Inertial.p;
  rtb_theta = Autopilot_U.Inertial.theta;
  rtb_q = Autopilot_U.Inertial.q;
  rtb_r = Autopilot_U.Inertial.r;

  /* BusSelector: '<Root>/Bus Selector3' incorporates:
   *  Inport: '<Root>/Air Data'
   */
  rtb_alt = Autopilot_U.AirData.alt;
  rtb_altRate = Autopilot_U.AirData.altRate;
  rtb_airspeed = Autopilot_U.AirData.airspeed;

  /* ModelReference: '<Root>/Roll_Autopilot' */
  roll_ap(&rtb_phi, &rtb_psi, &rtb_p, &rtb_airspeed, &Autopilot_U.APeng,
          &Autopilot_U.HDGmode, &Autopilot_U.HDGref, &Autopilot_U.TurnKnob,
          &Autopilot_Y.AileronCmd, &(Autopilot_DWork.Roll_Autopilot_DWORK1.rtb),
          &(Autopilot_DWork.Roll_Autopilot_DWORK1.rtdw), 1.0, 1.0, 3.0, 0.015);

  /* ModelReference: '<Root>/Pitch_Autopilot' */
  pitch_ap(&rtb_phi, &rtb_theta, &rtb_q, &rtb_r, &rtb_alt, &rtb_altRate,
           &rtb_airspeed, &Autopilot_U.APeng, &Autopilot_U.ALTMode,
           &Autopilot_U.ALTRef, &Autopilot_U.PitchWheel,
           &Autopilot_Y.ElevatorCmd,
           &(Autopilot_DWork.Pitch_Autopilot_DWORK1.rtb),
           &(Autopilot_DWork.Pitch_Autopilot_DWORK1.rtdw), 2.0, 0.5, 2.0);

  /* ModelReference: '<Root>/Yaw_Damper' */
  yaw_damper(&rtb_phi, &rtb_r, &rtb_airspeed, &Autopilot_U.APeng,
             &Autopilot_Y.RudderCmd, &(Autopilot_DWork.Yaw_Damper_DWORK1.rtb),
             1.0);
}

/* Model initialize function */
void Autopilot_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&Autopilot_DWork, 0,
                sizeof(D_Work_Autopilot));

  /* external inputs */
  (void) memset((void *)&Autopilot_U, 0,
                sizeof(ExternalInputs_Autopilot));
  Autopilot_U.AirData = Autopilot_rtZslBus1;
  Autopilot_U.Inertial = Autopilot_rtZslBus2;
  Autopilot_U.HDGref = 0.0;
  Autopilot_U.TurnKnob = 0.0;
  Autopilot_U.ALTRef = 0.0;
  Autopilot_U.PitchWheel = 0.0;

  /* external outputs */
  Autopilot_Y.AileronCmd = 0.0;
  Autopilot_Y.ElevatorCmd = 0.0;
  Autopilot_Y.RudderCmd = 0.0;

  /* Model Initialize fcn for ModelReference Block: '<Root>/Pitch_Autopilot' */
  pitch_ap_initialize(&(Autopilot_DWork.Pitch_Autopilot_DWORK1.rtb),
                      &(Autopilot_DWork.Pitch_Autopilot_DWORK1.rtdw));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Roll_Autopilot' */
  roll_ap_initialize(&(Autopilot_DWork.Roll_Autopilot_DWORK1.rtb),
                     &(Autopilot_DWork.Roll_Autopilot_DWORK1.rtdw));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Yaw_Damper' */
  yaw_damper_initialize(&(Autopilot_DWork.Yaw_Damper_DWORK1.rtb));

  /* Start for ModelReference: '<Root>/Pitch_Autopilot' */
  pitch_ap_Start(&(Autopilot_DWork.Pitch_Autopilot_DWORK1.rtdw));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
