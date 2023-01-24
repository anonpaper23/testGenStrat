/*
 * File: yaw_damper.h
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

#ifndef RTW_HEADER_yaw_damper_h_
#define RTW_HEADER_yaw_damper_h_
#ifndef yaw_damper_COMMON_INCLUDES_
# define yaw_damper_COMMON_INCLUDES_
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#endif                                 /* yaw_damper_COMMON_INCLUDES_ */

#include "yaw_damper_types.h"

/* Block signals for model 'yaw_damper' */
typedef struct {
  real_T yawRate;                      /* '<S1>/Sum1' (Output 1)  */
} rtB_yaw_damper;

typedef struct {
  rtB_yaw_damper rtb;
} rtMdlrefDWork_yaw_damper;

/* Model reference registration function */
extern void yaw_damper_initialize(rtB_yaw_damper *localB);
extern void yaw_damper(const real_T *rtu_Phi, const real_T *rtu_R, const real_T *
  rtu_TAS, const boolean_T *rtu_APEng, real_T *rty_RudCmd, rtB_yaw_damper
  *localB, real_T rtp_rateGain);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'yaw_damper'
 * '<S1>'   : 'yaw_damper/Basic Yaw Damper'
 * '<S2>'   : 'yaw_damper/Model Info'
 * '<S3>'   : 'yaw_damper/Basic Yaw Damper/Model Info'
 */

/*-
 * Requirements for '<Root>': yaw_damper
 */
#endif                                 /* RTW_HEADER_yaw_damper_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
