/*
 * File: attitude_controller.c
 *
 * Code generated for Simulink model 'attitude_controller'.
 *
 * Model version                  : 1.67
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:29 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "attitude_controller.h"
#include "attitude_controller_private.h"

/* Output and update for referenced model: 'attitude_controller' */
void attitude_controller(const real_T *rtu_Disp_Cmd, const real_T *rtu_Disp_FB,
  const real_T *rtu_Rate_FB, const boolean_T *rtu_Engaged, real_T *rty_Surf_Cmd,
  rtDW_attitude_controller *localDW, real_T rtp_dispGain, real_T rtp_dispLim,
  real_T rtp_rateGain, real_T rtp_rateLim, real_T rtp_intGain, real_T rtp_intLim,
  real_T rtp_cmdLim)
{
  boolean_T rtb_Notengaged;
  real_T rtb_Sum2;
  real_T rtb_Sum1;
  real_T y;

  /* Logic: '<Root>/Not engaged' */
  rtb_Notengaged = !(*rtu_Engaged);

  /* UnitDelay: '<S2>/X' */
  rtb_Sum2 = localDW->X_DSTATE;

  /* Switch: '<S2>/Switch' incorporates:
   *  Constant: '<S2>/Constant2'
   */
  if (rtb_Notengaged) {
    rtb_Sum2 = 0.0;
  }

  /* End of Switch: '<S2>/Switch' */

  /* Saturate: '<S2>/Saturation' */
  if (rtb_Sum2 >= rtp_intLim) {
    rtb_Sum2 = rtp_intLim;
  } else {
    if (rtb_Sum2 <= (-rtp_intLim)) {
      rtb_Sum2 = -rtp_intLim;
    }
  }

  /* End of Saturate: '<S2>/Saturation' */

  /* Saturate: '<Root>/Disp Limit' */
  if ((*rtu_Disp_Cmd) >= rtp_dispLim) {
    y = rtp_dispLim;
  } else if ((*rtu_Disp_Cmd) <= (-rtp_dispLim)) {
    y = -rtp_dispLim;
  } else {
    y = *rtu_Disp_Cmd;
  }

  /* Gain: '<Root>/Disp Gain' incorporates:
   *  Saturate: '<Root>/Disp Limit'
   *  Sum: '<Root>/Sum'
   */
  y = rtp_dispGain * (y - (*rtu_Disp_FB));

  /* Saturate: '<Root>/Rate Limit' */
  if (y >= rtp_rateLim) {
    y = rtp_rateLim;
  } else {
    if (y <= (-rtp_rateLim)) {
      y = -rtp_rateLim;
    }
  }

  /* Sum: '<Root>/Sum1' incorporates:
   *  Saturate: '<Root>/Rate Limit'
   */
  rtb_Sum1 = y - (*rtu_Rate_FB);

  /* Sum: '<Root>/Sum2' incorporates:
   *  Gain: '<Root>/Rate Gain'
   */
  y = rtb_Sum2 + (rtp_rateGain * rtb_Sum1);

  /* Saturate: '<Root>/Cmd Limit' */
  if (y >= rtp_cmdLim) {
    *rty_Surf_Cmd = rtp_cmdLim;
  } else if (y <= (-rtp_cmdLim)) {
    *rty_Surf_Cmd = -rtp_cmdLim;
  } else {
    *rty_Surf_Cmd = y;
  }

  /* End of Saturate: '<Root>/Cmd Limit' */

  /* Switch: '<S2>/Switch1' */
  if (rtb_Notengaged) {
    /* Update for UnitDelay: '<S2>/X' incorporates:
     *  Constant: '<S2>/Constant2'
     */
    localDW->X_DSTATE = 0.0;
  } else {
    /* Update for UnitDelay: '<S2>/X' incorporates:
     *  Gain: '<Root>/Int Gain'
     *  Product: '<S2>/Product1'
     *  Sum: '<S2>/Sum'
     */
    localDW->X_DSTATE = (attitude_controller_ConstB.Product * (rtp_intGain *
      rtb_Sum1)) + rtb_Sum2;
  }

  /* End of Switch: '<S2>/Switch1' */
}

/* Model initialize function */
void attitude_controller_initialize(rtDW_attitude_controller *localDW)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(rtDW_attitude_controller));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
