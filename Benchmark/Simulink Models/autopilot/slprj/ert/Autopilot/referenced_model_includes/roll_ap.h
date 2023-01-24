/*
 * File: roll_ap.h
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

#ifndef RTW_HEADER_roll_ap_h_
#define RTW_HEADER_roll_ap_h_
#ifndef roll_ap_COMMON_INCLUDES_
# define roll_ap_COMMON_INCLUDES_
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#endif                                 /* roll_ap_COMMON_INCLUDES_ */

#include "roll_ap_types.h"

/* Child system includes */
#include "attitude_controller.h"
#include "Heading_Mode.h"

/* Block signals for model 'roll_ap' */
typedef struct {
  real_T phiCmd;                       /* '<Root>/Mode switch' (Output 1)  */
} rtB_roll_ap;

/* Block states (auto storage) for model 'roll_ap' */
typedef struct {
  real_T FixPtUnitDelay1_DSTATE;       /* '<S3>/FixPt Unit Delay1' (DWork 1)  */
  rtMdlrefDWork_Heading_Mode HeadingMode_DWORK1;/* '<Root>/Heading Mode' (DWork 1)  */
  rtMdlrefDWork_attitude_controller BasicRollMode_DWORK1;/* '<Root>/Basic Roll Mode' (DWork 1)  */
} rtDW_roll_ap;

typedef struct {
  rtB_roll_ap rtb;
  rtDW_roll_ap rtdw;
} rtMdlrefDWork_roll_ap;

/* Model reference registration function */
extern void roll_ap_initialize(rtB_roll_ap *localB, rtDW_roll_ap *localDW);
extern void roll_ap(const real_T *rtu_Phi, const real_T *rtu_Psi, const real_T
                    *rtu_p, const real_T *rtu_TAS, const boolean_T *rtu_APEng,
                    const boolean_T *rtu_HDGMode, const real_T *rtu_HDGRef,
                    const real_T *rtu_TurnKnob, real_T *rty_AilCmd, rtB_roll_ap *
                    localB, rtDW_roll_ap *localDW, real_T rtp_dispGain, real_T
                    rtp_intGain, real_T rtp_rateGain, real_T rtp_hdgGain);

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
 * '<Root>' : 'roll_ap'
 * '<S1>'   : 'roll_ap/Model Info'
 * '<S2>'   : 'roll_ap/Roll Reference'
 * '<S3>'   : 'roll_ap/Roll Reference/Latch Phi'
 */

/*-
 * Requirements for '<Root>': roll_ap
 */
#endif                                 /* RTW_HEADER_roll_ap_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
