/*
 * File: Altitude_Mode_private.h
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

#ifndef RTW_HEADER_Altitude_Mode_private_h_
#define RTW_HEADER_Altitude_Mode_private_h_
#include "rtwtypes.h"
#include "model_reference_types.h"
#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

extern const real_T rtCP_pooled_TWughWQ5Fftr[8];
extern const real_T rtCP_pooled_nyUtRySC0JAJ[8];

#define rtCP_LookupTable_tableData     rtCP_pooled_TWughWQ5Fftr  /* Expression: [-42 -26 -19 -13 0 13 19 26]
                                                                  * Referenced by: '<Root>/Lookup Table' (Parameter: Table)
                                                                  */
#define rtCP_LookupTable_bp01Data      rtCP_pooled_nyUtRySC0JAJ  /* Expression: [-544 -206 -116 -52 0 52 116 206]
                                                                  * Referenced by: '<Root>/Lookup Table' (Parameter: BreakpointsForDimension1)
                                                                  */

extern const rtC_Altitude_Mode Altitude_Mode_ConstB;

#endif                                 /* RTW_HEADER_Altitude_Mode_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
