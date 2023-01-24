/*
 * File: Altitude_Mode.h
 *
 * Code generated for Simulink model 'Altitude_Mode'.
 *
 * Model version                  : 1.100
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:10 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Altitude_Mode_h_
#define RTW_HEADER_Altitude_Mode_h_
#ifndef Altitude_Mode_COMMON_INCLUDES_
# define Altitude_Mode_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#include "rtw_shared_utils.h"
#endif                                 /* Altitude_Mode_COMMON_INCLUDES_ */

#include "Altitude_Mode_types.h"

/* Block signals for model 'Altitude_Mode' */
typedef struct {
  real_T altError;                     /* '<Root>/Sum' (Output 1)  */
  real_T Probe[2];                     /* '<S4>/Probe' (Output 1)  */
} rtB_Altitude_Mode;

/* Block states (auto storage) for model 'Altitude_Mode' */
typedef struct {
  real_T DelayInput2_DSTATE;           /* '<S4>/Delay Input2' (DWork 1)  */
  real_T X_DSTATE;                     /* '<S5>/X' (DWork 1)  */
} rtDW_Altitude_Mode;

/* Invariant block signals for model 'Altitude_Mode' */
typedef struct {
  const real_T Product;                /* '<S5>/Product' (Output 1)  */
} rtC_Altitude_Mode;

typedef struct {
  rtB_Altitude_Mode rtb;
  rtDW_Altitude_Mode rtdw;
} rtMdlrefDWork_Altitude_Mode;

/* Model reference registration function */
extern void Altitude_Mode_initialize(rtB_Altitude_Mode *localB,
  rtDW_Altitude_Mode *localDW);
extern void Altitude_Mode_Start(rtB_Altitude_Mode *localB);
extern void Altitude_Mode(const real_T *rtu_AltRef, const real_T *rtu_Alt, const
  real_T *rtu_AltRate, const real_T *rtu_TAS, const real_T *rtu_Theta, const
  boolean_T *rtu_ALTMode, real_T *rty_ThetaCmd, rtB_Altitude_Mode *localB,
  rtDW_Altitude_Mode *localDW);

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
 * '<Root>' : 'Altitude_Mode'
 * '<S1>'   : 'Altitude_Mode/Angle Conversion'
 * '<S2>'   : 'Altitude_Mode/Angle Conversion1'
 * '<S3>'   : 'Altitude_Mode/Model Info'
 * '<S4>'   : 'Altitude_Mode/Rate Limiter'
 * '<S5>'   : 'Altitude_Mode/ResetIntegratorExternal'
 * '<S6>'   : 'Altitude_Mode/Rate Limiter/Saturation Dynamic'
 */

/*-
 * Requirements for '<Root>': Altitude_Mode
 */
#endif                                 /* RTW_HEADER_Altitude_Mode_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
