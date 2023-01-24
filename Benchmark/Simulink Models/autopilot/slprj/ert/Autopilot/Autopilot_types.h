/*
 * File: Autopilot_types.h
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

#ifndef RTW_HEADER_Autopilot_types_h_
#define RTW_HEADER_Autopilot_types_h_
#include "rtwtypes.h"
#include "model_reference_types.h"
#ifndef _DEFINED_TYPEDEF_FOR_slBus1_
#define _DEFINED_TYPEDEF_FOR_slBus1_

typedef struct {
  real_T altRate;
  real_T alpha;
  real_T beta;
  real_T airspeed;
  real_T alt;
} slBus1;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_slBus2_
#define _DEFINED_TYPEDEF_FOR_slBus2_

typedef struct {
  real_T phi;
  real_T theta;
  real_T psi;
  real_T p;
  real_T q;
  real_T r;
} slBus2;

#endif
#endif                                 /* RTW_HEADER_Autopilot_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
