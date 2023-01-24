/*
 * File: attitude_controller.h
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

#ifndef RTW_HEADER_attitude_controller_h_
#define RTW_HEADER_attitude_controller_h_
#ifndef attitude_controller_COMMON_INCLUDES_
# define attitude_controller_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#endif                                 /* attitude_controller_COMMON_INCLUDES_ */

#include "attitude_controller_types.h"

/* Block states (auto storage) for model 'attitude_controller' */
typedef struct {
  real_T X_DSTATE;                     /* '<S2>/X' (DWork 1)  */
} rtDW_attitude_controller;

/* Invariant block signals for model 'attitude_controller' */
typedef struct {
  const real_T Product;                /* '<S2>/Product' (Output 1)  */
} rtC_attitude_controller;

typedef struct {
  rtDW_attitude_controller rtdw;
} rtMdlrefDWork_attitude_controller;

/* Model reference registration function */
extern void attitude_controller_initialize(rtDW_attitude_controller *localDW);
extern void attitude_controller(const real_T *rtu_Disp_Cmd, const real_T
  *rtu_Disp_FB, const real_T *rtu_Rate_FB, const boolean_T *rtu_Engaged, real_T *
  rty_Surf_Cmd, rtDW_attitude_controller *localDW, real_T rtp_dispGain, real_T
  rtp_dispLim, real_T rtp_rateGain, real_T rtp_rateLim, real_T rtp_intGain,
  real_T rtp_intLim, real_T rtp_cmdLim);

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
 * '<Root>' : 'attitude_controller'
 * '<S1>'   : 'attitude_controller/Model Info'
 * '<S2>'   : 'attitude_controller/ResetIntegratorInternal'
 */

/*-
 * Requirements for '<Root>': attitude_controller
 */
#endif                                 /* RTW_HEADER_attitude_controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
