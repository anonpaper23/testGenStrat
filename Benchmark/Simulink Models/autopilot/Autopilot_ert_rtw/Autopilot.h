/*
 * File: Autopilot.h
 *
 * Code generated for Simulink model 'Autopilot'.
 *
 * Model version                  : 1.152
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:57 2014
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

/* Macros for accessing real-time model data structure */

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  rtMdlrefDWork_roll_ap Roll_Autopilot_DWORK1;/* '<Root>/Roll_Autopilot' (DWork 1)  */
  rtMdlrefDWork_pitch_ap Pitch_Autopilot_DWORK1;/* '<Root>/Pitch_Autopilot' (DWork 1)  */
  rtMdlrefDWork_yaw_damper Yaw_Damper_DWORK1;/* '<Root>/Yaw_Damper' (DWork 1)  */
} D_Work_Autopilot;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  slBus1 AirData;                      /* '<Root>/Air Data' */
  slBus2 Inertial;                     /* '<Root>/Inertial' */
  boolean_T APeng;                     /* '<Root>/AP Eng' */
  boolean_T HDGmode;                   /* '<Root>/HDG Mode' */
  boolean_T ALTMode;                   /* '<Root>/ALT Mode' */
  real_T HDGref;                       /* '<Root>/HDG Ref' */
  real_T TurnKnob;                     /* '<Root>/Turn Knob' */
  real_T ALTRef;                       /* '<Root>/ALT Ref' */
  real_T PitchWheel;                   /* '<Root>/Pitch Wheel' */
} ExternalInputs_Autopilot;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T AileronCmd;                   /* '<Root>/Aileron Cmd' */
  real_T ElevatorCmd;                  /* '<Root>/Elevator Cmd' */
  real_T RudderCmd;                    /* '<Root>/Rudder Cmd' */
} ExternalOutputs_Autopilot;

/* Block states (auto storage) */
extern D_Work_Autopilot Autopilot_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_Autopilot Autopilot_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_Autopilot Autopilot_Y;

/* External data declarations for dependent source files */
extern const slBus1 Autopilot_rtZslBus1;/* slBus1 ground */
extern const slBus2 Autopilot_rtZslBus2;/* slBus2 ground */

/* Model entry point functions */
extern void Autopilot_initialize(void);
extern void Autopilot_step(void);

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
