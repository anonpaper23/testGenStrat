/*
 * File: model_reference_types.h
 *
 * Code generated for Simulink model 'Altitude_Mode'.
 *
 * Model version                  : 1.100
 * Simulink Coder version         : 8.5 (R2013b) 08-Aug-2013
 * C/C++ source code generated on : Mon Feb 03 08:13:10 2014
 */

#ifndef __MODEL_REFERENCE_TYPES_H__
#define __MODEL_REFERENCE_TYPES_H__
#include "rtwtypes.h"
#ifndef __MODEL_REFERENCE_TYPES__
#define __MODEL_REFERENCE_TYPES__

/*===========================================================================*
 * Model reference type definitions                                          *
 *===========================================================================*/
/*
 * This structure is used by model reference to
 * communicate timing information through the hierarchy.
 */
typedef struct _rtTimingBridge_tag rtTimingBridge;
struct _rtTimingBridge_tag {
  uint32_T nTasks;
  uint32_T** clockTick;
  uint32_T** clockTickH;
  uint32_T* taskCounter;
  real_T** taskTime;
  boolean_T** rateTransition;
  boolean_T *firstInitCond;
};

#endif                                 /* __MODEL_REFERENCE_TYPES__ */
#endif                                 /* __MODEL_REFERENCE_TYPES_H__ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
