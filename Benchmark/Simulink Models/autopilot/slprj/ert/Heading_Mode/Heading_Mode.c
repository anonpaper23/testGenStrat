/*
 * File: Heading_Mode.c
 *
 * Code generated for Simulink model 'Heading_Mode'.
 *
 * Model version                  : 1.146
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:34 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Heading_Mode.h"
#include "Heading_Mode_private.h"

/* Output and update for referenced model: 'Heading_Mode' */
void Heading_Mode(const real_T *rtu_PsiRef, const real_T *rtu_Psi, const real_T *
                  rtu_TAS, real_T *rty_PhiCmd, rtB_Heading_Mode *localB, real_T
                  rtp_hdgGain)
{
  real_T rtb_sum;
  real_T y;

  /* Sum: '<S1>/sum' */
  rtb_sum = (*rtu_PsiRef) - (*rtu_Psi);

  /* Signum: '<S1>/signum' */
  if (rtb_sum < 0.0) {
    y = -1.0;
  } else if (rtb_sum > 0.0) {
    y = 1.0;
  } else {
    y = rtb_sum;
  }

  /* Sum: '<S1>/sum1' incorporates:
   *  Abs: '<S1>/abs'
   *  Constant: '<S1>/constant'
   *  Constant: '<S1>/constant1'
   *  Product: '<S1>/product'
   *  Product: '<S1>/product1'
   *  Product: '<S1>/product2'
   *  Rounding: '<S1>/floor'
   *  Signum: '<S1>/signum'
   *  Sum: '<S1>/sum2'
   */
  localB->hdgError = rtb_sum - (y * (floor((fabs(rtb_sum) + 180.0) / 360.0) *
    360.0));

  /* Product: '<Root>/Product' incorporates:
   *  Gain: '<Root>/Disp Gain'
   *
   * Block requirements for '<Root>/Product':
   *  1. 1.10. Heading Performance
   *
   * Block requirements for '<Root>/Disp Gain':
   *  1. 1.10. Heading Performance
   */
  *rty_PhiCmd = (rtp_hdgGain * localB->hdgError) * (*rtu_TAS);
}

/* Model initialize function */
void Heading_Mode_initialize(rtB_Heading_Mode *localB)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(rtB_Heading_Mode));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
