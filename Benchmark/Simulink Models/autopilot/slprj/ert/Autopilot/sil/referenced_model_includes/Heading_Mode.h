/*
 * File: Heading_Mode.h
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

#ifndef RTW_HEADER_Heading_Mode_h_
#define RTW_HEADER_Heading_Mode_h_
#ifndef Heading_Mode_COMMON_INCLUDES_
# define Heading_Mode_COMMON_INCLUDES_
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#endif                                 /* Heading_Mode_COMMON_INCLUDES_ */

#include "Heading_Mode_types.h"

/* Block signals for model 'Heading_Mode' */
typedef struct {
  real_T hdgError;                     /* '<S1>/sum1' (Output 1)  */
} rtB_Heading_Mode;

typedef struct {
  rtB_Heading_Mode rtb;
} rtMdlrefDWork_Heading_Mode;

/* Model reference registration function */
extern void Heading_Mode_initialize(rtB_Heading_Mode *localB);
extern void Heading_Mode(const real_T *rtu_PsiRef, const real_T *rtu_Psi, const
  real_T *rtu_TAS, real_T *rty_PhiCmd, rtB_Heading_Mode *localB, real_T
  rtp_hdgGain);

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
 * '<Root>' : 'Heading_Mode'
 * '<S1>'   : 'Heading_Mode/Angle_Diff'
 * '<S2>'   : 'Heading_Mode/Model Info'
 */

/*-
 * Requirements for '<Root>': Heading_Mode
 */
#endif                                 /* RTW_HEADER_Heading_Mode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
