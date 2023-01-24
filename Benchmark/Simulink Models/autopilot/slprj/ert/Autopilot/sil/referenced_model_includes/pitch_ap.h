/*
 * File: pitch_ap.h
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

#ifndef RTW_HEADER_pitch_ap_h_
#define RTW_HEADER_pitch_ap_h_
#ifndef pitch_ap_COMMON_INCLUDES_
# define pitch_ap_COMMON_INCLUDES_
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#endif                                 /* pitch_ap_COMMON_INCLUDES_ */

#include "pitch_ap_types.h"

/* Child system includes */
#include "attitude_controller.h"
#include "Altitude_Mode.h"

/* Block signals for model 'pitch_ap' */
typedef struct {
  real_T thetaCmd;                     /* '<Root>/Mode switch' (Output 1)  */
} rtB_pitch_ap;

/* Block states (auto storage) for model 'pitch_ap' */
typedef struct {
  real_T FixPtUnitDelay1_DSTATE;       /* '<S5>/FixPt Unit Delay1' (DWork 1)  */
  rtMdlrefDWork_Altitude_Mode AltitudeMode_DWORK1;/* '<Root>/Altitude Mode' (DWork 1)  */
  rtMdlrefDWork_attitude_controller BasicPitchMode_DWORK1;/* '<Root>/Basic Pitch Mode' (DWork 1)  */
} rtDW_pitch_ap;

typedef struct {
  rtB_pitch_ap rtb;
  rtDW_pitch_ap rtdw;
} rtMdlrefDWork_pitch_ap;

/* Model reference registration function */
extern void pitch_ap_initialize(rtB_pitch_ap *localB, rtDW_pitch_ap *localDW);
extern void pitch_ap_Start(rtDW_pitch_ap *localDW);
extern void pitch_ap(const real_T *rtu_Phi, const real_T *rtu_Theta, const
                     real_T *rtu_Q, const real_T *rtu_R, const real_T *rtu_Alt,
                     const real_T *rtu_AltRate, const real_T *rtu_TAS, const
                     boolean_T *rtu_APEng, const boolean_T *rtu_ALTMode, const
                     real_T *rtu_AltRef, const real_T *rtu_PitchWheel, real_T
                     *rty_ElvCmd, rtB_pitch_ap *localB, rtDW_pitch_ap *localDW,
                     real_T rtp_dispGain, real_T rtp_intGain, real_T
                     rtp_rateGain);

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
 * '<Root>' : 'pitch_ap'
 * '<S1>'   : 'pitch_ap/Model Info'
 * '<S2>'   : 'pitch_ap/Pitch Rate Feedback'
 * '<S3>'   : 'pitch_ap/Pitch Reference'
 * '<S4>'   : 'pitch_ap/Pitch Rate Feedback/Angle Conversion'
 * '<S5>'   : 'pitch_ap/Pitch Reference/Latch Theta'
 */

/*-
 * Requirements for '<Root>': pitch_ap
 */
#endif                                 /* RTW_HEADER_pitch_ap_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
