/*
 * File: Autopilot.h
 *
 * Code generated for Simulink model 'Autopilot'.
 *
 * Model version                  : 1.152
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:38:36 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Embedded Processor
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Autopilot_h_
#define RTW_HEADER_Autopilot_h_
#ifndef Autopilot_COMMON_INCLUDES_
# define Autopilot_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "model_reference_types.h"
#endif                                 /* Autopilot_COMMON_INCLUDES_ */

#include "Autopilot_types.h"

/* Child system includes */
#include "yaw_damper.h"
#include "roll_ap.h"
#include "pitch_ap.h"

/* Block states (auto storage) for model 'Autopilot' */
typedef struct {
  rtMdlrefDWork_pitch_ap Pitch_Autopilot_DWORK1;/* '<Root>/Pitch_Autopilot' (DWork 1)  */
  rtMdlrefDWork_roll_ap Roll_Autopilot_DWORK1;/* '<Root>/Roll_Autopilot' (DWork 1)  */
  rtMdlrefDWork_yaw_damper Yaw_Damper_DWORK1;/* '<Root>/Yaw_Damper' (DWork 1)  */
} rtDW_Autopilot;

typedef struct {
  rtDW_Autopilot rtdw;
} rtMdlrefDWork_Autopilot;

/* Model reference registration function */
extern void Autopilot_initialize(rtDW_Autopilot *localDW);
extern void Autopilot_Start(rtDW_Autopilot *localDW);
extern void Autopilot(const slBus1 *rtu_AirData, const slBus2 *rtu_Inertial,
                      const boolean_T *rtu_APEng, const boolean_T *rtu_HDGMode,
                      const boolean_T *rtu_ALTMode, const real_T *rtu_HDGRef,
                      const real_T *rtu_TurnKnob, const real_T *rtu_ALTRef,
                      const real_T *rtu_PitchWheel, real_T *rty_AileronCmd,
                      real_T *rty_ElevatorCmd, real_T *rty_RudderCmd,
                      rtDW_Autopilot *localDW);

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
 * '<Root>' : 'Autopilot'
 * '<S1>'   : 'Autopilot/Model Info'
 */

/*-
 * Requirements for '<Root>': Autopilot
 */
#endif                                 /* RTW_HEADER_Autopilot_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
