/*
 * File: yaw_damper.c
 *
 * Code generated for Simulink model 'yaw_damper'.
 *
 * Model version                  : 1.80
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:39 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "yaw_damper.h"
#include "yaw_damper_private.h"

/* Output and update for referenced model: 'yaw_damper' */
void yaw_damper(const real_T *rtu_Phi, const real_T *rtu_R, const real_T
                *rtu_TAS, const boolean_T *rtu_APEng, real_T *rty_RudCmd,
                rtB_yaw_damper *localB, real_T rtp_rateGain)
{
  real_T u;
  real_T y;

  /* Outputs for Atomic SubSystem: '<Root>/Basic Yaw Damper' */
  /* MinMax: '<S1>/MinMax' incorporates:
   *  Constant: '<S1>/Constant'
   */
  if ((*rtu_TAS) >= 100.0) {
    y = *rtu_TAS;
  } else {
    y = 100.0;
  }

  /* Sum: '<S1>/Sum1' incorporates:
   *  Gain: '<S1>/gravity'
   *  MinMax: '<S1>/MinMax'
   *  Product: '<S1>/Product'
   *  Trigonometry: '<S1>/sin'
   */
  localB->yawRate = ((32.2 * sin(*rtu_Phi)) / y) - (*rtu_R);

  /* End of Outputs for SubSystem: '<Root>/Basic Yaw Damper' */

  /* Switch: '<Root>/Eng switch' incorporates:
   *  Constant: '<Root>/Constant'
   */
  if (*rtu_APEng) {
    /* Outputs for Atomic SubSystem: '<Root>/Basic Yaw Damper' */
    /* Gain: '<S1>/Rate Gain' */
    u = rtp_rateGain * localB->yawRate;

    /* Saturate: '<S1>/Rud Limit' */
    if (u >= 25.0) {
      *rty_RudCmd = 25.0;
    } else if (u <= (-25.0)) {
      *rty_RudCmd = (-25.0);
    } else {
      *rty_RudCmd = u;
    }

    /* End of Saturate: '<S1>/Rud Limit' */
    /* End of Outputs for SubSystem: '<Root>/Basic Yaw Damper' */
  } else {
    *rty_RudCmd = 0.0;
  }

  /* End of Switch: '<Root>/Eng switch' */
}

/* Model initialize function */
void yaw_damper_initialize(rtB_yaw_damper *localB)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(rtB_yaw_damper));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
