/*
 * File: Altitude_Mode.c
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

#include "Altitude_Mode.h"
#include "Altitude_Mode_private.h"

/* Start for referenced model: 'Altitude_Mode' */
void Altitude_Mode_Start(rtB_Altitude_Mode *localB)
{
  /* Start for Probe: '<S4>/Probe' */
  localB->Probe[0U] = 0.025;
  localB->Probe[1U] = 0.0;
}

/* Output and update for referenced model: 'Altitude_Mode' */
void Altitude_Mode(const real_T *rtu_AltRef, const real_T *rtu_Alt, const real_T
                   *rtu_AltRate, const real_T *rtu_TAS, const real_T *rtu_Theta,
                   const boolean_T *rtu_ALTMode, real_T *rty_ThetaCmd,
                   rtB_Altitude_Mode *localB, rtDW_Altitude_Mode *localDW)
{
  /* local block i/o variables */
  real_T rtb_UkYk1;
  real_T rtb_Switch;
  boolean_T rtb_NotALT;
  real_T rtb_UnitConversion;
  real_T rtb_Switch_o3ad;
  real_T rtb_Sum3;
  real_T rtb_Saturation;
  real_T rtb_Saturation_krec;
  real_T rtb_deltariselimit;
  real_T rtb_deltafalllimit;
  real_T rtb_Switch2;
  real_T u;

  /* Logic: '<Root>/Not ALT' */
  rtb_NotALT = !(*rtu_ALTMode);

  /* Switch: '<Root>/Switch' incorporates:
   *  Constant: '<Root>/ALT G limit'
   *  Constant: '<Root>/Other G limit'
   */
  if (rtb_NotALT) {
    rtb_UnitConversion = 32.2;
  } else {
    rtb_UnitConversion = 6.44;
  }

  /* End of Switch: '<Root>/Switch' */

  /* MinMax: '<Root>/MinMax' incorporates:
   *  Constant: '<Root>/Min TAS'
   */
  if ((*rtu_TAS) >= 100.0) {
    rtb_Switch_o3ad = *rtu_TAS;
  } else {
    rtb_Switch_o3ad = 100.0;
  }

  /* End of MinMax: '<Root>/MinMax' */

  /* Gain: '<S1>/Unit Conversion' incorporates:
   *  Product: '<Root>/Product'
   */
  rtb_UnitConversion = 57.295779513082323 * (rtb_UnitConversion /
    rtb_Switch_o3ad);

  /* Sum: '<Root>/Sum' */
  localB->altError = (*rtu_AltRef) - (*rtu_Alt);

  /* Lookup_n-D: '<Root>/Lookup Table' */
  rtb_Sum3 = look1_binlag(localB->altError, rtCP_LookupTable_bp01Data,
    rtCP_LookupTable_tableData, 7U);

  /* Gain: '<S2>/Unit Conversion' incorporates:
   *  Product: '<Root>/Gamma Cmd'
   *  Sum: '<Root>/Sum1'
   */
  u = 57.295779513082323 * ((rtb_Sum3 - (*rtu_AltRate)) / rtb_Switch_o3ad);

  /* Saturate: '<Root>/Saturation' */
  if (u >= 20.0) {
    rtb_Saturation = 20.0;
  } else if (u <= (-20.0)) {
    rtb_Saturation = (-20.0);
  } else {
    rtb_Saturation = u;
  }

  /* End of Saturate: '<Root>/Saturation' */

  /* Sum: '<Root>/Sum3' */
  rtb_Sum3 = (*rtu_Theta) - rtb_Saturation;

  /* UnitDelay: '<S5>/X' */
  rtb_Switch_o3ad = localDW->X_DSTATE;

  /* Switch: '<S5>/Switch' */
  if (rtb_NotALT) {
    rtb_Switch_o3ad = rtb_Sum3;
  }

  /* End of Switch: '<S5>/Switch' */

  /* Saturate: '<S5>/Saturation' */
  if (rtb_Switch_o3ad >= 20.0) {
    rtb_Saturation_krec = 20.0;
  } else if (rtb_Switch_o3ad <= (-20.0)) {
    rtb_Saturation_krec = (-20.0);
  } else {
    rtb_Saturation_krec = rtb_Switch_o3ad;
  }

  /* End of Saturate: '<S5>/Saturation' */

  /* Sum: '<S4>/Difference Inputs1' incorporates:
   *  Sum: '<Root>/Sum2'
   *  UnitDelay: '<S4>/Delay Input2'
   */
  rtb_UkYk1 = (rtb_Saturation + rtb_Saturation_krec) -
    localDW->DelayInput2_DSTATE;

  /* Product: '<S4>/delta rise limit' */
  rtb_deltariselimit = rtb_UnitConversion * localB->Probe[0];

  /* Product: '<S4>/delta fall limit' incorporates:
   *  Gain: '<Root>/negate'
   */
  rtb_deltafalllimit = ((-1.0) * rtb_UnitConversion) * localB->Probe[0];

  /* Switch: '<S6>/Switch' incorporates:
   *  RelationalOperator: '<S6>/UpperRelop'
   */
  if (rtb_UkYk1 < rtb_deltafalllimit) {
    rtb_Switch = rtb_deltafalllimit;
  } else {
    rtb_Switch = rtb_UkYk1;
  }

  /* End of Switch: '<S6>/Switch' */

  /* Switch: '<S6>/Switch2' incorporates:
   *  RelationalOperator: '<S6>/LowerRelop1'
   */
  if (rtb_UkYk1 > rtb_deltariselimit) {
    rtb_Switch2 = rtb_deltariselimit;
  } else {
    rtb_Switch2 = rtb_Switch;
  }

  /* End of Switch: '<S6>/Switch2' */

  /* Sum: '<S4>/Difference Inputs2' incorporates:
   *  UnitDelay: '<S4>/Delay Input2'
   */
  *rty_ThetaCmd = rtb_Switch2 + localDW->DelayInput2_DSTATE;

  /* Switch: '<S5>/Switch1' */
  if (rtb_NotALT) {
    /* Update for UnitDelay: '<S5>/X' */
    localDW->X_DSTATE = rtb_Sum3;
  } else {
    /* Update for UnitDelay: '<S5>/X' incorporates:
     *  Product: '<S5>/Product1'
     *  Sum: '<S5>/Sum'
     */
    localDW->X_DSTATE = (Altitude_Mode_ConstB.Product * rtb_Saturation) +
      rtb_Saturation_krec;
  }

  /* End of Switch: '<S5>/Switch1' */

  /* Update for UnitDelay: '<S4>/Delay Input2' */
  localDW->DelayInput2_DSTATE = *rty_ThetaCmd;
}

/* Model initialize function */
void Altitude_Mode_initialize(rtB_Altitude_Mode *localB, rtDW_Altitude_Mode
  *localDW)
{
  /* Registration code */

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(rtB_Altitude_Mode));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(rtDW_Altitude_Mode));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
