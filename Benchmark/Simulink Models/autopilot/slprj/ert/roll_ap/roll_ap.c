/*
 * File: roll_ap.c
 *
 * Code generated for Simulink model 'roll_ap'.
 *
 * Model version                  : 1.164
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:50 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "roll_ap.h"
#include "roll_ap_private.h"

/* Output and update for referenced model: 'roll_ap' */
void roll_ap(const real_T *rtu_Phi, const real_T *rtu_Psi, const real_T *rtu_p,
             const real_T *rtu_TAS, const boolean_T *rtu_APEng, const boolean_T *
             rtu_HDGMode, const real_T *rtu_HDGRef, const real_T *rtu_TurnKnob,
             real_T *rty_AilCmd, rtB_roll_ap *localB, rtDW_roll_ap *localDW,
             real_T rtp_dispGain, real_T rtp_intGain, real_T rtp_rateGain,
             real_T rtp_hdgGain)
{
  /* local block i/o variables */
  real_T rtb_HeadingMode;
  real_T rtb_BasicRollMode;
  real_T rtb_TKswitch;
  real_T rtb_refswitch;

  /* ModelReference: '<Root>/Heading Mode'
   *
   * Block requirements for '<Root>/Heading Mode':
   *  1. 1.10. Heading Performance
   */
  Heading_Mode(rtu_HDGRef, rtu_Psi, rtu_TAS, &rtb_HeadingMode,
               &(localDW->HeadingMode_DWORK1.rtb), rtp_hdgGain);

  /* Outputs for Atomic SubSystem: '<Root>/Roll Reference'
   *
   * Block requirements for '<Root>/Roll Reference':
   *  1. 1.3. Roll Hold Reference
   */
  /* Switch: '<S2>/ref switch' incorporates:
   *  Constant: '<S2>/minus six'
   *  Constant: '<S2>/six'
   *  Constant: '<S2>/zero'
   *  Logic: '<S2>/or'
   *  RelationalOperator: '<S2>/Ref Threshold'
   *  RelationalOperator: '<S2>/Ref Threshold1'
   *
   * Block requirements for '<S2>/ref switch':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/minus six':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/six':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/zero':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/or':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/Ref Threshold':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/Ref Threshold1':
   *  1. 1.3. Roll Hold Reference
   */
  if (((*rtu_Phi) >= 6.0) || ((*rtu_Phi) <= (-6.0))) {
    rtb_refswitch = *rtu_Phi;
  } else {
    rtb_refswitch = 0.0;
  }

  /* End of Switch: '<S2>/ref switch' */

  /* Switch: '<S3>/Enable' incorporates:
   *  Logic: '<S2>/Not engaged'
   *  UnitDelay: '<S3>/FixPt Unit Delay1'
   *
   * Block requirements for '<S2>/Not engaged':
   *  1. 1.3. Roll Hold Reference
   */
  if (!(*rtu_APEng)) {
  } else {
    rtb_refswitch = localDW->FixPtUnitDelay1_DSTATE;
  }

  /* End of Switch: '<S3>/Enable' */

  /* Switch: '<S2>/TK switch' incorporates:
   *  Abs: '<S2>/Abs'
   *  Constant: '<S2>/three'
   *  RelationalOperator: '<S2>/TK Threshold'
   *  UnitDelay: '<S3>/FixPt Unit Delay1'
   *
   * Block requirements for '<S2>/TK switch':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/Abs':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/three':
   *  1. 1.3. Roll Hold Reference
   *
   * Block requirements for '<S2>/TK Threshold':
   *  1. 1.3. Roll Hold Reference
   */
  if (fabs(*rtu_TurnKnob) < 3.0) {
    rtb_TKswitch = localDW->FixPtUnitDelay1_DSTATE;
  } else {
    rtb_TKswitch = *rtu_TurnKnob;
  }

  /* End of Switch: '<S2>/TK switch' */

  /* Update for UnitDelay: '<S3>/FixPt Unit Delay1' */
  localDW->FixPtUnitDelay1_DSTATE = rtb_refswitch;

  /* End of Outputs for SubSystem: '<Root>/Roll Reference' */

  /* Switch: '<Root>/Mode switch'
   *
   * Block requirements for '<Root>/Mode switch':
   *  1. 1.2. Roll Hold Mode
   *  2. 1.4. Roll Performance
   *  3. 1.8. Heading Hold Mode
   *  4. 1.10. Heading Performance
   */
  if (*rtu_HDGMode) {
    localB->phiCmd = rtb_HeadingMode;
  } else {
    localB->phiCmd = rtb_TKswitch;
  }

  /* End of Switch: '<Root>/Mode switch' */

  /* ModelReference: '<Root>/Basic Roll Mode'
   *
   * Block requirements for '<Root>/Basic Roll Mode':
   *  1. 1.4. Roll Performance
   *  2. 1.5. Roll Rate Limit
   *  3. 1.6. Roll Angle Limit
   *  4. 1.7. Aileron Angle Limit
   */
  attitude_controller(&localB->phiCmd, rtu_Phi, rtu_p, rtu_APEng,
                      &rtb_BasicRollMode, &(localDW->BasicRollMode_DWORK1.rtdw),
                      rtp_dispGain, 30.0, rtp_rateGain, 6.0, rtp_intGain, 5.0,
                      15.0);

  /* Switch: '<Root>/Eng switch' incorporates:
   *  Constant: '<Root>/zero'
   *
   * Block requirements for '<Root>/Eng switch':
   *  1. 1.1. Roll Autopilot Engage Control
   *
   * Block requirements for '<Root>/zero':
   *  1. 1.1. Roll Autopilot Engage Control
   */
  if (*rtu_APEng) {
    *rty_AilCmd = rtb_BasicRollMode;
  } else {
    *rty_AilCmd = 0.0;
  }

  /* End of Switch: '<Root>/Eng switch' */
}

/* Model initialize function */
void roll_ap_initialize(rtB_roll_ap *localB, rtDW_roll_ap *localDW)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(rtB_roll_ap));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(rtDW_roll_ap));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Basic Roll Mode' */
  attitude_controller_initialize(&(localDW->BasicRollMode_DWORK1.rtdw));

  /* Model Initialize fcn for ModelReference Block: '<Root>/Heading Mode' */
  Heading_Mode_initialize(&(localDW->HeadingMode_DWORK1.rtb));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
