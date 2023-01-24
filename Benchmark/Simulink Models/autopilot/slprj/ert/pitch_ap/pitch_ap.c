/*
 * File: pitch_ap.c
 *
 * Code generated for Simulink model 'pitch_ap'.
 *
 * Model version                  : 1.90
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:44 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "pitch_ap.h"
#include "pitch_ap_private.h"

/* Start for referenced model: 'pitch_ap' */
void pitch_ap_Start(rtDW_pitch_ap *localDW)
{
  /* Start for ModelReference: '<Root>/Altitude Mode' */
  Altitude_Mode_Start(&(localDW->AltitudeMode_DWORK1.rtb));
}

/* Output and update for referenced model: 'pitch_ap' */
void pitch_ap(const real_T *rtu_Phi, const real_T *rtu_Theta, const real_T
              *rtu_Q, const real_T *rtu_R, const real_T *rtu_Alt, const real_T
              *rtu_AltRate, const real_T *rtu_TAS, const boolean_T *rtu_APEng,
              const boolean_T *rtu_ALTMode, const real_T *rtu_AltRef, const
              real_T *rtu_PitchWheel, real_T *rty_ElvCmd, rtB_pitch_ap *localB,
              rtDW_pitch_ap *localDW, real_T rtp_dispGain, real_T rtp_intGain,
              real_T rtp_rateGain)
{
  /* local block i/o variables */
  real_T rtb_AltitudeMode;
  real_T rtb_BasicPitchMode;
  real_T rtb_Sum3_pjva;
  real_T rtb_Sum3;
  real_T rtb_UnitConversion;
  real_T rtb_Xnew;

  /* ModelReference: '<Root>/Altitude Mode' */
  Altitude_Mode(rtu_AltRef, rtu_Alt, rtu_AltRate, rtu_TAS, rtu_Theta,
                rtu_ALTMode, &rtb_AltitudeMode,
                &(localDW->AltitudeMode_DWORK1.rtb),
                &(localDW->AltitudeMode_DWORK1.rtdw));

  /* Outputs for Atomic SubSystem: '<Root>/Pitch Reference' */
  /* Switch: '<S5>/Enable' incorporates:
   *  Logic: '<S3>/Not engaged'
   *  UnitDelay: '<S5>/FixPt Unit Delay1'
   */
  if (!(*rtu_APEng)) {
    rtb_Xnew = *rtu_Theta;
  } else {
    rtb_Xnew = localDW->FixPtUnitDelay1_DSTATE;
  }

  /* End of Switch: '<S5>/Enable' */

  /* Sum: '<S3>/Sum3' incorporates:
   *  UnitDelay: '<S5>/FixPt Unit Delay1'
   */
  rtb_Sum3 = localDW->FixPtUnitDelay1_DSTATE + (*rtu_PitchWheel);

  /* Update for UnitDelay: '<S5>/FixPt Unit Delay1' */
  localDW->FixPtUnitDelay1_DSTATE = rtb_Xnew;

  /* End of Outputs for SubSystem: '<Root>/Pitch Reference' */

  /* Switch: '<Root>/Mode switch' */
  if (*rtu_ALTMode) {
    localB->thetaCmd = rtb_AltitudeMode;
  } else {
    localB->thetaCmd = rtb_Sum3;
  }

  /* End of Switch: '<Root>/Mode switch' */

  /* Outputs for Atomic SubSystem: '<Root>/Pitch Rate Feedback' */
  /* Gain: '<S4>/Unit Conversion' */
  rtb_UnitConversion = 0.017453292519943295 * (*rtu_Phi);

  /* Sum: '<S2>/Sum3' incorporates:
   *  Product: '<S2>/Product'
   *  Product: '<S2>/Product1'
   *  Trigonometry: '<S2>/cos'
   *  Trigonometry: '<S2>/sin'
   */
  rtb_Sum3_pjva = (cos(rtb_UnitConversion) * (*rtu_Q)) + (sin(rtb_UnitConversion)
    * (*rtu_R));

  /* End of Outputs for SubSystem: '<Root>/Pitch Rate Feedback' */

  /* ModelReference: '<Root>/Basic Pitch Mode' */
  attitude_controller(&localB->thetaCmd, rtu_Theta, &rtb_Sum3_pjva, rtu_APEng,
                      &rtb_BasicPitchMode, &(localDW->BasicPitchMode_DWORK1.rtdw),
                      rtp_dispGain, 20.0, rtp_rateGain, 3.0, rtp_intGain, 5.0,
                      20.0);

  /* Switch: '<Root>/Eng switch' incorporates:
   *  Constant: '<Root>/Constant'
   */
  if (*rtu_APEng) {
    *rty_ElvCmd = rtb_BasicPitchMode;
  } else {
    *rty_ElvCmd = 0.0;
  }

  /* End of Switch: '<Root>/Eng switch' */
}

/* Model initialize function */
void pitch_ap_initialize(rtB_pitch_ap *localB, rtDW_pitch_ap *localDW)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(rtB_pitch_ap));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(rtDW_pitch_ap));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Altitude Mode' */
  Altitude_Mode_initialize(&(localDW->AltitudeMode_DWORK1.rtb),
    &(localDW->AltitudeMode_DWORK1.rtdw));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Basic Pitch Mode' */
  attitude_controller_initialize(&(localDW->BasicPitchMode_DWORK1.rtdw));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
