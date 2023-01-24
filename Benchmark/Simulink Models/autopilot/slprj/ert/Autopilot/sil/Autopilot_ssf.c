/*
 * Autopilot_ssf.c
 *
 * Automatically generated s-function with I/O interface for:
 * Component: Autopilot
 * Component Simulink Path: Autopilot
 * Simulation Mode: SIL
 *
 */

#define S_FUNCTION_NAME Autopilot_ssf
#define S_FUNCTION_LEVEL 2
#if !defined(RTW_GENERATED_S_FUNCTION)
#define RTW_GENERATED_S_FUNCTION
#endif

#include <stdio.h>
#include <string.h>
#include "simstruc.h"
#include "fixedpoint.h"

#include "rtiostream_utils.h"

static char * getSimulinkBlockPath(SimStruct *S) {
    char * simulinkBlockPath = NULL;
    const char * origBlockPath = ssGetPath(S);
    const char * searchString = "TmpSFcnForModelReference_";
    char * searchPtr;
    size_t origLength, searchAndNameLength, copyAmount;
    char * secondPart;
    size_t nameLength;
    origLength = strlen(origBlockPath);
    searchPtr = strstr(origBlockPath, searchString);
    if (searchPtr == NULL) {
        return simulinkBlockPath;
    }
    searchAndNameLength = strlen(searchPtr);
    copyAmount = origLength - searchAndNameLength;
    simulinkBlockPath = (char *) mxCalloc((mwSize) (origLength + 1), sizeof(char));
    simulinkBlockPath = strncpy(simulinkBlockPath, origBlockPath, copyAmount);
    simulinkBlockPath[copyAmount] = '\0';
    nameLength = searchAndNameLength - strlen(searchString);
    secondPart = &simulinkBlockPath[copyAmount];
    secondPart = strncpy(secondPart, &origBlockPath[origLength - nameLength], nameLength);
    secondPart[nameLength] = '\0';
    return simulinkBlockPath;
}

typedef struct {
    libH_type libH;
    char * lib;
    mxArray * MATLABObject;
    double streamID;
    double recvTimeout;
    double sendTimeout;
} rtIOStreamData;

typedef struct {
    uint8_T * data;
    mwSize size;
} IOBuffer_T;

typedef struct {
    FILE ** Fd;
    mwSize size;
    int32_T fidOffset;
} targetIOFd_T;

/* implements calls into MATLAB */
static int callMATLAB(SimStruct * S, 
                      int nlhs, 
                      mxArray * plhs[], 
                      int nrhs, 
                      mxArray * prhs[], 
                      const char * functionName, 
                      int withTrap) {
    int errorOccurred = 0;
    if (withTrap) {
        mxArray * mException;
        mException = mexCallMATLABWithTrap(nlhs, plhs, nrhs, prhs, functionName);
        {
            int i;
            for (i=0; i<nrhs; i++) {
                mxDestroyArray(prhs[i]);
            } /* for */
        }
        if (mException != NULL) {
            mxArray * rhsDisplayMException[1];
            errorOccurred = 1;
            rhsDisplayMException[0] = mException;
            mException = mexCallMATLABWithTrap(0, NULL, 1, rhsDisplayMException, "rtw.pil.SILPILInterface.displayMException");
            mxDestroyArray(rhsDisplayMException[0]);
            if (mException != NULL) {
                mxDestroyArray(mException);
                ssSetErrorStatus( S,"Error calling rtw.pil.SILPILInterface.displayMException on mException object.");
            } else {
                ssSetErrorStatus( S,"mexCallMATLABWithTrap failed. See the full MException report in the MATLAB Command Window.");
            } /* if */
        } /* if */
    } else {
        errorOccurred = mexCallMATLAB(nlhs, plhs, nrhs, prhs, functionName);
        {
            int i;
            for (i=0; i<nrhs; i++) {
                mxDestroyArray(prhs[i]);
            } /* for */
        }
        if (errorOccurred) {
            ssSetErrorStatus( S,"mexCallMATLAB failed!");
        } /* if */
    } /* if */
    return errorOccurred;
}

/* enlarges the dynamically allocated shared I/O buffer
 * IOBufferPtr->data is NULL on failure */
static void growIOBuffer(SimStruct *S, IOBuffer_T * IOBuffer, mwSize requiredSize) {
    if (IOBuffer->size < requiredSize) {
        IOBuffer->data = mxRealloc(IOBuffer->data, requiredSize * sizeof(uint8_T));
        if (IOBuffer->data == NULL) {
            ssSetErrorStatus( S,"growIOBuffer: mxRealloc failed.");
        } else {
            mexMakeMemoryPersistent(IOBuffer->data);
            IOBuffer->size = requiredSize;
        } /* if */
    } /* if */
}

/* grow the buffer for target I/O Fd array
 * targetIOFd->Fd is NULL on failure */
static void growTargetIOFd(SimStruct *S, targetIOFd_T * IOFd, mwSize requiredSize) {
    if (IOFd->size < requiredSize) {
        IOFd->Fd = mxRealloc(IOFd->Fd, requiredSize * sizeof(FILE*));
        if (IOFd->Fd == NULL) {
            ssSetErrorStatus( S,"growTargetIOFd: mxRealloc failed.");
        } else {
            mexMakeMemoryPersistent(IOFd->Fd);
            IOFd->size = requiredSize;
        } /* if */
    } /* if */
}

static void closeAndFreeTargetIOFd(SimStruct *S) {
    int i;
    if(ssGetPWork(S) != NULL) {
        targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
        if(targetIOFdPtr != NULL) {
            if (targetIOFdPtr->Fd != NULL) {
                for(i=0; i<targetIOFdPtr->size; i++) {
                    if (targetIOFdPtr->Fd[i] != NULL) {
                        fclose(targetIOFdPtr->Fd[i]);
                    } /* if */
                } /* for */
                mxFree(targetIOFdPtr->Fd);
            } /* if */
            mxFree(targetIOFdPtr);
        } /* if */
        ssSetPWorkValue(S, 2, NULL);
    } /* if */
}

/* implements blocking receive via RtIOStream */
static int blockingRtIOStreamRecv(SimStruct *S,  size_t recvSize, int withTrap, uint8_T * dataIn) {

    int errorOccurred = 0;
    int timeoutOccurred = 0;
    rtIOStreamData * rtIOStreamDataPtr = (rtIOStreamData *) ssGetPWorkValue(S, 0);
    int * pCommErrorOccurred = (int *) ssGetPWorkValue(S, 3);

    if (rtIOStreamDataPtr == NULL) {
        ssSetErrorStatus( S,"rtIOStreamDataPtr is NULL.");
        errorOccurred = 1;
        return errorOccurred;
    } /* if */

    if (pCommErrorOccurred == NULL) {
        ssSetErrorStatus( S,"pCommErrorOccurred is NULL.");
        errorOccurred = 1;
        return errorOccurred;
    } /* if */

    {
        int attemptToReceiveData = 1;
        while(attemptToReceiveData) {
            errorOccurred = 0;
            timeoutOccurred = 0;
            errorOccurred = rtIOStreamBlockingRecv(&rtIOStreamDataPtr->libH, 
               (const int) rtIOStreamDataPtr->streamID, 
               dataIn, 
               recvSize, 
               (const int) rtIOStreamDataPtr->recvTimeout, 
               &timeoutOccurred);

            if (errorOccurred && timeoutOccurred) {
                mxArray *rhs[3];
                mxArray *lhs[1];
                char * simulinkBlockPath = getSimulinkBlockPath(S);
                if (simulinkBlockPath == NULL) {
                    ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
                    return errorOccurred;
                }
                rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
                rhs[ 1 ] = mxCreateDoubleScalar( 1 );
                rhs[ 2 ] = mxCreateString(simulinkBlockPath);
                callMATLAB(S,  1, lhs, 3, rhs, "rtw.pil.SILPILInterface.sfunctionIsSILDebugEnabledAndSILExeAlive", 0);
                mxFree((void *) simulinkBlockPath);
                attemptToReceiveData = (int) *mxGetPr(lhs[0]);
                mxDestroyArray(lhs[0]);
            } else {
                /* rtIOStreamBlockingRecv completed successfully or a non-timeout related error occurred */
                attemptToReceiveData = 0;
            } /* if */
        } /* while */
    }

    *pCommErrorOccurred = 0;
    if (errorOccurred) {
        mxArray *rhs[2];
        rhs[0] = mxCreateDoubleScalar(timeoutOccurred);
        rhs[1] = mxCreateDoubleScalar(rtIOStreamDataPtr->recvTimeout);
        *pCommErrorOccurred = 1;
        errorOccurred = callMATLAB(S,  0, NULL, 2, rhs, "rtw.pil.SILPILInterface.handleRtIOStreamRecvError", withTrap);
    } /* if */

    return errorOccurred;
}

/* implements blocking send via RtIOStream */
static int blockingRtIOStreamSend(SimStruct *S, uint8_T * dataOut, size_t sendSize, int withTrap) {

    int errorOccurred = 0;
    int timeoutOccurred = 0;
    rtIOStreamData * rtIOStreamDataPtr = (rtIOStreamData *) ssGetPWorkValue(S, 0);
    int * pCommErrorOccurred = (int *) ssGetPWorkValue(S, 3);

    if (rtIOStreamDataPtr == NULL) {
        ssSetErrorStatus( S,"rtIOStreamDataPtr is NULL.");
        errorOccurred = 1;
        return errorOccurred;
    } /* if */

    if (pCommErrorOccurred == NULL) {
        ssSetErrorStatus( S,"pCommErrorOccurred is NULL.");
        errorOccurred = 1;
        return errorOccurred;
    } /* if */

    {
        int attemptToSendData = 1;
        while(attemptToSendData) {
            errorOccurred = 0;
            timeoutOccurred = 0;
            errorOccurred = rtIOStreamBlockingSend(&rtIOStreamDataPtr->libH, 
               (const int) rtIOStreamDataPtr->streamID, 
               dataOut, 
               sendSize, 
               (const int) rtIOStreamDataPtr->sendTimeout, 
               &timeoutOccurred);

            if (errorOccurred && timeoutOccurred) {
                mxArray *rhs[3];
                mxArray *lhs[1];
                char * simulinkBlockPath = getSimulinkBlockPath(S);
                if (simulinkBlockPath == NULL) {
                    ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
                    return errorOccurred;
                }
                rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
                rhs[ 1 ] = mxCreateDoubleScalar( 1 );
                rhs[ 2 ] = mxCreateString(simulinkBlockPath);
                callMATLAB(S,  1, lhs, 3, rhs, "rtw.pil.SILPILInterface.sfunctionIsSILDebugEnabledAndSILExeAlive", 0);
                mxFree((void *) simulinkBlockPath);
                attemptToSendData = (int) *mxGetPr(lhs[0]);
                mxDestroyArray(lhs[0]);
            } else {
                /* rtIOStreamBlockingSend completed successfully or a non-timeout related error occurred */
                attemptToSendData = 0;
            } /* if */
        } /* while */
    }

    *pCommErrorOccurred = 0;
    if (errorOccurred) {
        mxArray *rhs[2];
        rhs[0] = mxCreateDoubleScalar(timeoutOccurred);
        rhs[1] = mxCreateDoubleScalar(rtIOStreamDataPtr->sendTimeout);
        *pCommErrorOccurred = 1;
        errorOccurred = callMATLAB(S,  0, NULL, 2, rhs, "rtw.pil.SILPILInterface.handleRtIOStreamSendError", withTrap);
    } /* if */

    return errorOccurred;
}

/* implements command dispatch layer */
static int commandDispatch(SimStruct *S, IOBuffer_T * IOBuffer, mwSize dataOutSize, mwSize * dataInSize, int withTrap) {
    int errorOccurred = 0;
    uint8_T * mxMemUnitPtr;
    mwSize responseSize = 0;
    uint8_T responseHeaderData[5];
    uint8_T COMMAND_COMPLETE = 0;
    *dataInSize = 0;
    errorOccurred = blockingRtIOStreamSend(S, IOBuffer->data, dataOutSize, withTrap);
    if (errorOccurred) {
        return errorOccurred;
    } /* if */
    while (!COMMAND_COMPLETE) {
        uint32_T WRITE_DATA_AVAIL;
        errorOccurred = blockingRtIOStreamRecv(S, 5, withTrap, &responseHeaderData[0]);
        if (errorOccurred) {
            return errorOccurred;
        } /* if */
        mxMemUnitPtr = (uint8_T *) &responseHeaderData[0];
        {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = (uint8_T *) &WRITE_DATA_AVAIL;
            {
                size_t num_elements = 1;
                size_t bytes_consumed = num_elements * sizeof(uint32_T);
                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
            }
        }
        {
            uint8_T * simDataMemUnitPtr;
            simDataMemUnitPtr = (uint8_T *) &COMMAND_COMPLETE;
            {
                size_t num_elements = 1;
                size_t bytes_consumed = num_elements * sizeof(uint8_T);
                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
            }
        }
        if (WRITE_DATA_AVAIL) {
            growIOBuffer(S, IOBuffer, responseSize + (mwSize) WRITE_DATA_AVAIL);
            if (IOBuffer->data == NULL) {
                ssSetErrorStatus( S,"commandDispatch: growIOBuffer failed.");
                errorOccurred = 1;
                return errorOccurred;
            } /* if */
            errorOccurred = blockingRtIOStreamRecv(S, WRITE_DATA_AVAIL, withTrap, &IOBuffer->data[responseSize]);
            if (errorOccurred) {
                return errorOccurred;
            } /* if */
            responseSize += WRITE_DATA_AVAIL;
        } /* if */
    } /* while */
    *dataInSize = responseSize;
    return errorOccurred;
}

	MdlRefChildMdlRec childModels[8] = {
          "Altitude_Mode","Autopilot:pitch_ap:Altitude_Mode",0 , 
          "Autopilot","Autopilot",0 , 
          "Heading_Mode","Autopilot:roll_ap:Heading_Mode",0 , 
          "attitude_controller","Autopilot:pitch_ap:attitude_controller",0 , 
          "attitude_controller0","Autopilot:roll_ap:attitude_controller0",0 , 
          "pitch_ap","Autopilot:pitch_ap",0 , 
          "roll_ap","Autopilot:roll_ap",0 , 
          "yaw_damper","Autopilot:yaw_damper",0 };


/* This function checks the attributes of tunable parameters. */
#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)
static void mdlCheckParameters(SimStruct *S) {
}
#endif /* MDL_CHECK_PARAMETERS */

static void mdlInitializeSizes(SimStruct *S) {
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
    #if defined(MDL_CHECK_PARAMETERS)
       mdlCheckParameters(S);
    #endif
       if (ssGetErrorStatus(S) != (NULL)) return;
    } else {
       /* Parameter mismatch will be reported by Simulink */
       return;
    }
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);
    /* no support for SimState */
    ssSetSimStateCompliance(S, DISALLOW_SIM_STATE);
    
    /* Allow signal dimensions greater than 2 */
    ssAllowSignalsWithMoreThan2D(S);
    
    /* Allow fixed-point data types with 33 or more bits */
    ssFxpSetU32BitRegionCompliant(S,1);
    
    {
        mxArray * lhs[1];
        mxArray * error = NULL;
        char * installVersion;
        error = mexCallMATLABWithTrap(1, lhs, 0, NULL, "rtw.pil.SILPILInterface.getPILVersion");
        if (error != NULL) {
            mxDestroyArray(error);
            ssSetErrorStatus( S,"Failed to determine the installed In-the-Loop version for comparison against the In-the-Loop s-function version (release 6.5 (R2013b)_11). To avoid this error, remove the In-the-Loop s-function from your MATLAB path (e.g. delete it or move to a clean working directory).");
            return;
        } /* if */
        if (mxIsEmpty(lhs[0])) {
            ssSetErrorStatus( S,"rtw.pil.SILPILInterface.getPILVersion returned empty!");
            return;
        } /* if */
        installVersion = mxArrayToString(lhs[0]);
        mxDestroyArray(lhs[0]);
        if (installVersion == NULL) {
            ssSetErrorStatus( S,"Failed to determine installed In-the-Loop version.");
            return;
        } /* if */
        if (strcmp(installVersion, "6.5 (R2013b)_11") != 0) {
            ssSetErrorStatus( S,"The In-the-Loop s-function is incompatible with the installed In-the-Loop version (see ver('ecoder')); it was generated for release 6.5 (R2013b)_11. To avoid this error, remove the In-the-Loop s-function from your MATLAB path (e.g. delete it or move to a clean working directory)");
            return;
        } /* if */
        mxFree(installVersion);
    }

    if (S->mdlInfo->genericFcn != NULL) {
        _GenericFcn fcn = S->mdlInfo->genericFcn;
        int_T hwSettings[15];
        int_T  opSettings[1];
        boolean_T hasDiscTs;
        real_T lifeSpan, startTime,  stopTime;
        real_T rtInf;

        union {
            struct {
                uint32_T wordL;
                uint32_T wordH;
            } bitVal;
            real_T fltVal;
        } tmpVal;

        tmpVal.bitVal.wordH = 0x7FF00000U;
        tmpVal.bitVal.wordL = 0x00000000U;
        rtInf = tmpVal.fltVal;

        hasDiscTs = 1;
        lifeSpan = rtInf;
        startTime = 0.0;
        stopTime = 25.0;
        (fcn)(S, GEN_FCN_CHK_MODELREF_SOLVER_TYPE_EARLY, 2, NULL);
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_LIFE_SPAN, -1, &lifeSpan)) return;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_START_TIME, -1, &startTime)) return;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_STOP_TIME, -1, &stopTime)) return;
        hwSettings[0] = 8;
        hwSettings[1] = 16;
        hwSettings[2] = 32;
        hwSettings[3] = 32;
        hwSettings[4] = 32;
        hwSettings[5] = 64;
        hwSettings[6] = 32;
        hwSettings[7] = 0;
        hwSettings[8] = 2;
        hwSettings[9] = 32;
        hwSettings[10] = 1;
        hwSettings[11] = 0;
        hwSettings[12] = 2;
        hwSettings[13] = 64;
        hwSettings[14] = 0;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_HARDWARE_SETTINGS, 15, hwSettings)) return;
        opSettings[0] = 0;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_OPTIM_SETTINGS, 1, opSettings)) return;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_CONCURRETNT_TASK_SUPPORT, 0, NULL)) return;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_SOLVER_TYPE, 2, &hasDiscTs)) return;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_SOLVER_MODE, SOLVER_MODE_SINGLETASKING, NULL)) return;
        if (!(fcn)(S, GEN_FCN_CHK_MODELREF_DATA_DICTIONARY, 0, (void *)"")) return;
    }

    ssSetRTWGeneratedSFcn(S, 2);

    if (S->mdlInfo->genericFcn != NULL) {
        _GenericFcn fcn = S->mdlInfo->genericFcn;
        (fcn)(S, GEN_FCN_CHK_MODELREF_FRAME_UPGRADE_DIAGNOSTICS, 1, NULL);
    }


    if (!ssSetNumInputPorts(S, 9)) return;

    /* Input Port 0 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 0, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        ssRegisterTypeFromNamedObject(S, "slBus1", &dataTypeId);
        if (dataTypeId == INVALID_DTYPE_ID) return;
        ssSetInputPortDataType(S, 0, dataTypeId);
    }
    ssSetBusInputAsStruct(S, 0, 1);
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 0, &di);
    }
    ssSetInputPortDimensionsMode(S, 0, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 0, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 0, 0.025);
    ssSetInputPortOffsetTime(S, 0, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 0, FRAME_NO);

    /* Input Port 1 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 1, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        ssRegisterTypeFromNamedObject(S, "slBus2", &dataTypeId);
        if (dataTypeId == INVALID_DTYPE_ID) return;
        ssSetInputPortDataType(S, 1, dataTypeId);
    }
    ssSetBusInputAsStruct(S, 1, 1);
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 1, &di);
    }
    ssSetInputPortDimensionsMode(S, 1, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 1, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 1, 0.025);
    ssSetInputPortOffsetTime(S, 1, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 1, FRAME_NO);

    /* Input Port 2 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 2, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 8;
        ssSetInputPortDataType(S, 2, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 2, &di);
    }
    ssSetInputPortDimensionsMode(S, 2, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 2, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 2, 0.025);
    ssSetInputPortOffsetTime(S, 2, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 2, FRAME_NO);

    /* Input Port 3 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 3, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 3, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 8;
        ssSetInputPortDataType(S, 3, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 3, &di);
    }
    ssSetInputPortDimensionsMode(S, 3, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 3, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 3, 0.025);
    ssSetInputPortOffsetTime(S, 3, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 3, FRAME_NO);

    /* Input Port 4 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 4, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 4, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 8;
        ssSetInputPortDataType(S, 4, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 4, &di);
    }
    ssSetInputPortDimensionsMode(S, 4, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 4, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 4, 0.025);
    ssSetInputPortOffsetTime(S, 4, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 4, FRAME_NO);

    /* Input Port 5 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 5, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 5, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetInputPortDataType(S, 5, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 5, &di);
    }
    ssSetInputPortDimensionsMode(S, 5, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 5, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 5, 0.025);
    ssSetInputPortOffsetTime(S, 5, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 5, FRAME_NO);

    /* Input Port 6 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 6, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 6, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetInputPortDataType(S, 6, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 6, &di);
    }
    ssSetInputPortDimensionsMode(S, 6, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 6, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 6, 0.025);
    ssSetInputPortOffsetTime(S, 6, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 6, FRAME_NO);

    /* Input Port 7 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 7, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 7, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetInputPortDataType(S, 7, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 7, &di);
    }
    ssSetInputPortDimensionsMode(S, 7, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 7, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 7, 0.025);
    ssSetInputPortOffsetTime(S, 7, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 7, FRAME_NO);

    /* Input Port 8 */
    /* contiguous inport */
    ssSetInputPortRequiredContiguous(S, 8, 1);
    /* directfeedthrough */
    ssSetInputPortDirectFeedThrough(S, 8, 1);
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetInputPortDataType(S, 8, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetInputPortDimensionInfo(S, 8, &di);
    }
    ssSetInputPortDimensionsMode(S, 8, FIXED_DIMS_MODE);
    /* complexity */
    ssSetInputPortComplexSignal(S, 8, COMPLEX_NO);
    /* using port based sample times */
    ssSetInputPortSampleTime(S, 8, 0.025);
    ssSetInputPortOffsetTime(S, 8, 0);
    /* sampling mode */
    ssSetInputPortFrameData(S, 8, FRAME_NO);

    if (!ssSetNumOutputPorts(S, 3)) return;

    /* Output Port 0 */
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetOutputPortDataType(S, 0, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetOutputPortDimensionInfo(S, 0, &di);
    }
    ssSetOutputPortDimensionsMode(S, 0, FIXED_DIMS_MODE);
    /* complexity */
    ssSetOutputPortComplexSignal(S, 0, COMPLEX_NO);
    /* using port based sample times */
    ssSetOutputPortSampleTime(S, 0, 0.025);
    ssSetOutputPortOffsetTime(S, 0, 0);
    /* sampling mode */
    ssSetOutputPortFrameData(S, 0, FRAME_NO);

    /* Output Port 1 */
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetOutputPortDataType(S, 1, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetOutputPortDimensionInfo(S, 1, &di);
    }
    ssSetOutputPortDimensionsMode(S, 1, FIXED_DIMS_MODE);
    /* complexity */
    ssSetOutputPortComplexSignal(S, 1, COMPLEX_NO);
    /* using port based sample times */
    ssSetOutputPortSampleTime(S, 1, 0.025);
    ssSetOutputPortOffsetTime(S, 1, 0);
    /* sampling mode */
    ssSetOutputPortFrameData(S, 1, FRAME_NO);

    /* Output Port 2 */
    if (ssGetSimMode(S) != SS_SIMMODE_SIZES_CALL_ONLY) {
        DTypeId dataTypeId = INVALID_DTYPE_ID;
        /* set datatype */
        dataTypeId = 0;
        ssSetOutputPortDataType(S, 2, dataTypeId);
    }
    /* dimensions */
    {
        DECL_AND_INIT_DIMSINFO(di);
        int_T dims[ 1 ] = { 1 };
        di.numDims = 1;
        di.dims = dims;
        di.width = 1;
        ssSetOutputPortDimensionInfo(S, 2, &di);
    }
    ssSetOutputPortDimensionsMode(S, 2, FIXED_DIMS_MODE);
    /* complexity */
    ssSetOutputPortComplexSignal(S, 2, COMPLEX_NO);
    /* using port based sample times */
    ssSetOutputPortSampleTime(S, 2, 0.025);
    ssSetOutputPortOffsetTime(S, 2, 0);
    /* sampling mode */
    ssSetOutputPortFrameData(S, 2, FRAME_NO);

    /* using port based sample times */
    ssSetNumSampleTimes(S, PORT_BASED_SAMPLE_TIMES);
    ssSetNumPWork(S, 4);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);
    /* this s-function is sample time dependent: do not allow sub-models containing it to inherit sample times */
    ssSetModelReferenceSampleTimeInheritanceRule(S, DISALLOW_SAMPLE_TIME_INHERITANCE);
    {
        uint_T options = 0;
        /* do not allow (including inheritance of) constant block-based sample times*/
        options |= SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME;
        options |= SS_OPTION_SUPPORTS_ALIAS_DATA_TYPES;
        options |= SS_OPTION_CALL_TERMINATE_ON_EXIT;
        ssSetOptions(S, options);
    }
    ssSetModelReferenceNormalModeSupport(S, MDL_START_AND_MDL_PROCESS_PARAMS_OK);
    slmrSetHasNonVirtualConstantTs(S, true);
    {
        static ssRTWStorageType storageClass[12] = {SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO, SS_RTW_STORAGE_AUTO};
        ssSetModelRefPortRTWStorageClasses(S, storageClass);
    }

    /* Set output port IC attributes */
    ssSetOutputPortICAttributes(S, 0, false, false, false);
    /* Set output port IC attributes */
    ssSetOutputPortICAttributes(S, 1, false, false, false);
    /* Set output port IC attributes */
    ssSetOutputPortICAttributes(S, 2, false, false, false);
    slmrModelRefSetOutputPortDrivenByNonCondExecStateflow(S, 0, false);
    slmrModelRefSetOutputPortDrivenByNonCondExecStateflow(S, 1, false);
    slmrModelRefSetOutputPortDrivenByNonCondExecStateflow(S, 2, false);
    ssSetModelRefOutputBlkIOUpdatedInAnotherExecContext(S, 0, false);
    ssSetModelRefOutputBlkIOUpdatedInAnotherExecContext(S, 1, false);
    ssSetModelRefOutputBlkIOUpdatedInAnotherExecContext(S, 2, false);
    ssSetModelReferenceConsistentOutportInitialization(S, true);
    ssSetModelRefHasEnablePort(S,0);
    ssSetOptimizeModelRefInitCode(S, 1);
    if (S->mdlInfo->genericFcn != (NULL)) {
        	ssRegModelRefChildModel(S, 8, childModels);

    }
    ssSetModelRefSignalLoggingSaveFormat(S, SS_LOG_FORMAT_MIXED);
}

#define MDL_SET_INPUT_PORT_SAMPLE_TIME /* Change to #undef to remove function */
#if defined(MDL_SET_INPUT_PORT_SAMPLE_TIME) && defined(MATLAB_MEX_FILE)
static void mdlSetInputPortSampleTime(SimStruct *S, int_T portIdx, real_T sampleTime, real_T offsetTime) {
    /* sample times are fully specified, not inherited */
}
#endif /* MDL_SET_INPUT_PORT_SAMPLE_TIME */

#define MDL_SET_OUTPUT_PORT_SAMPLE_TIME /* Change to #undef to remove function */
#if defined(MDL_SET_OUTPUT_PORT_SAMPLE_TIME) && defined(MATLAB_MEX_FILE)
static void mdlSetOutputPortSampleTime(SimStruct *S, int_T portIdx, real_T sampleTime, real_T offsetTime) {
    /* sample times are fully specified, not inherited */
}
#endif /* MDL_SET_OUTPUT_PORT_SAMPLE_TIME */

static void mdlInitializeSampleTimes(SimStruct *S) {
    /* using port based sample times */
}

/* forward declaration for mdlProcessParameters */
#define MDL_PROCESS_PARAMETERS  /* Change to #undef to remove function */
#if defined(MDL_PROCESS_PARAMETERS)
static void mdlProcessParameters(SimStruct *S);
#endif /* MDL_PROCESS_PARAMETERS */
#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START)
static void mdlStart(SimStruct *S) {
    {
        mxArray *rhs[4];
        mxArray *lhs[1];
        char * rootLoggingPath;
        char * simulinkBlockPath = getSimulinkBlockPath(S);
        if (simulinkBlockPath == NULL) {
            ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
            return;
        }
        rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
        rhs[ 1 ] = mxCreateDoubleScalar( 1 );
        rhs[ 2 ] = mxCreateString(simulinkBlockPath);
        rhs[3] = mxCreateString(ssGetPath(ssGetRootSS(S)));
        callMATLAB(S,  1, lhs, 4 , rhs, "rtw.pil.SILPILInterface.sfunctionInitializeHook", 0);
        rootLoggingPath = mxArrayToString(lhs[0]);
        mxDestroyArray(lhs[0]);
        {
            int * pCommErrorOccurred = (int *) mxCalloc(1, sizeof(int));
            if (pCommErrorOccurred == NULL) {
                ssSetErrorStatus( S,"Error in allocating memory for pCommErrorOccurred through mxCalloc.");
                return;
            } /* if */
            *pCommErrorOccurred = 0;
            mexMakeMemoryPersistent(pCommErrorOccurred);
            ssSetPWorkValue(S, 3, pCommErrorOccurred);
        }

        mxFree((void *) simulinkBlockPath);
        mxFree(rootLoggingPath);
    }
    {
        {
            mxArray *rhs[4];
            char * simulinkBlockPath = getSimulinkBlockPath(S);
            if (simulinkBlockPath == NULL) {
                ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
                return;
            }
            rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
            rhs[ 1 ] = mxCreateDoubleScalar( 1 );
            rhs[ 2 ] = mxCreateString(simulinkBlockPath);
            rhs[3] = mxCreateString("uint8");
            callMATLAB(S,  0, NULL, 4, rhs, "rtw.pil.SILPILInterface.sfunctionPILStartHook", 0);
            mxFree((void *) simulinkBlockPath);
        }
    }
    {
        mxArray *rhs[3];
        mxArray *lhs[5];
        char * simulinkBlockPath = getSimulinkBlockPath(S);
        if (simulinkBlockPath == NULL) {
            ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
            return;
        }
        rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
        rhs[ 1 ] = mxCreateDoubleScalar( 1 );
        rhs[ 2 ] = mxCreateString(simulinkBlockPath);
        callMATLAB(S,  5, lhs, 3, rhs, "rtw.pil.SILPILInterface.sfunctionGetRtIOStreamInfoHook", 0);
        mxFree((void *) simulinkBlockPath);
        {
            rtIOStreamData * rtIOStreamDataPtr = (rtIOStreamData *) mxCalloc(1, sizeof(rtIOStreamData));
            if (rtIOStreamDataPtr == NULL) {
                ssSetErrorStatus( S,"Error in allocating memory through mxCalloc.");
                return;
            } /* if */
            rtIOStreamDataPtr->lib = mxArrayToString(lhs[0]);
            rtIOStreamDataPtr->MATLABObject = mxDuplicateArray(lhs[1]);
            mexMakeMemoryPersistent(rtIOStreamDataPtr);
            mexMakeMemoryPersistent(rtIOStreamDataPtr->lib);
            mexMakeArrayPersistent(rtIOStreamDataPtr->MATLABObject);
            rtIOStreamDataPtr->streamID = *mxGetPr(lhs[2]);
            rtIOStreamDataPtr->recvTimeout = *mxGetPr(lhs[3]);
            rtIOStreamDataPtr->sendTimeout = *mxGetPr(lhs[4]);
            {
                int errorStatus = rtIOStreamLoadLib(&rtIOStreamDataPtr->libH, rtIOStreamDataPtr->lib);
                if (errorStatus) {
                    ssSetErrorStatus( S,"rtIOStreamLoadLib failed.");
                    return;
                } /* if */
            }
            ssSetPWorkValue(S, 0, rtIOStreamDataPtr);
        }
        {
            int i;
            for (i=0; i<5; i++) {
                mxDestroyArray(lhs[i]);
            } /* for */
        }
    }
    {
        IOBuffer_T * IOBufferPtr = (IOBuffer_T *) mxCalloc(1, sizeof(IOBuffer_T));
        if (IOBufferPtr == NULL) {
            ssSetErrorStatus( S,"Error in allocating memory through mxCalloc.");
            return;
        } /* if */
        mexMakeMemoryPersistent(IOBufferPtr);
        ssSetPWorkValue(S, 1, IOBufferPtr);
    }
    {
        targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) mxCalloc(1, sizeof(targetIOFd_T));
        if (targetIOFdPtr == NULL) {
            return;
        } /* if */
        mexMakeMemoryPersistent(targetIOFdPtr);
        targetIOFdPtr->size = 0;
        targetIOFdPtr->Fd = NULL;
        targetIOFdPtr->fidOffset = -1;
        ssSetPWorkValue(S, 2, targetIOFdPtr);
    }
    {
        uint8_T * mxMemUnitPtr;
        mwSize dataInSize = 0;
        IOBuffer_T * IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
        if (IOBufferPtr != NULL) {
            growIOBuffer(S, IOBufferPtr, 5);
            if (IOBufferPtr->data != NULL) {
                mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                /* write command id */
                *mxMemUnitPtr = 0;
                mxMemUnitPtr++;
                {
                    uint8_T * simDataMemUnitPtr;
                    uint32_T commandDataFcnid = 0;
                    simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;
                    {
                        size_t num_elements = 1;
                        size_t bytes_consumed = num_elements * sizeof(uint32_T);
                        memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                    }
                }
                {
                    int errorOccurred;
                    errorOccurred = commandDispatch(S, IOBufferPtr, 5, &dataInSize, 0);
                    if (errorOccurred) {
                        return;
                    } /* if */
                }
                if (dataInSize > 0) {
                    size_t dataInMemUnitSize = dataInSize;
                    uint8_T responseId;
                    uint8_T * mxMemUnitPtrEnd;
                    #define RESPONSE_ERROR 0
                    #define RESPONSE_PRINTF 3
                    #define RESPONSE_FOPEN 4
                    #define RESPONSE_FPRINTF 5
                    #define TARGET_IO_SUCCESS 0
                    #define RESPONSE_TYPE_SIZE 6
                    mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                    mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                    while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                        /* read response id */
                        responseId = *mxMemUnitPtr;
                        mxMemUnitPtr++;
                        switch (responseId) {
                            case RESPONSE_ERROR: {
                                uint8_T errorId = *mxMemUnitPtr;
                                mxMemUnitPtr++;
                                if (errorId) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pilverification:PILError");
                                        prhs[1] = mxCreateDoubleScalar(errorId);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } /* if */
                                break;
                            }
                            case RESPONSE_PRINTF: {
                                uint8_T PRINTF_ERROR;
                                uint16_T PRINTF_SIZE;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                        prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } else {
                                    uint8_T *pPrintBuff;
                                    pPrintBuff = mxMemUnitPtr;
                                    if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
                                        mexPrintf("%s", pPrintBuff);
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + PRINTF_SIZE;
                                break;
                            }
                            case RESPONSE_FOPEN: {
                                uint16_T FOPEN_FID;
                                uint16_T FOPEN_NAME_SIZE;
                                targetIOFd_T *targetIOFdPtr;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                if (targetIOFdPtr != NULL) {
                                    /* check fid increments by 1 */
                                    if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
                                        targetIOFdPtr->fidOffset = FOPEN_FID;
                                        growTargetIOFd(S, targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
                                        if (targetIOFdPtr->Fd != NULL)  {
                                            uint8_T *pFopenBuff;
                                            targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
                                            pFopenBuff = mxMemUnitPtr;
                                            if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
                                                FILE * tmpFd = NULL;
                                                tmpFd = fopen((char *) pFopenBuff,"w");
                                                if (tmpFd != NULL) {
                                                    /* save the file descriptor */
                                                    targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
                                                } else {
                                                    {
                                                        mxArray * prhs[ 2 ];
                                                        prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
                                                        prhs[1] = mxCreateString((char *) pFopenBuff);
                                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                        return;
                                                    }
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } else {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
                                            prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + FOPEN_NAME_SIZE;
                                break;
                            }
                            case RESPONSE_FPRINTF: {
                                uint8_T FPRINTF_ERROR;
                                uint16_T FPRINTF_FID;
                                uint16_T FPRINTF_SIZE;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                        prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } else {
                                    targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                    if (targetIOFdPtr != NULL) {
                                        if (targetIOFdPtr->size > FPRINTF_FID) {
                                            if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
                                                uint8_T *pFprintfBuff;
                                                pFprintfBuff = mxMemUnitPtr;
                                                if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
                                                    fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + FPRINTF_SIZE;
                                break;
                            }
                            case RESPONSE_TYPE_SIZE: {
                                uint8_T typeId = *mxMemUnitPtr;
                                uint8_T typeBytes;
                                mxMemUnitPtr++;
                                typeBytes = *mxMemUnitPtr;
                                mxMemUnitPtr++;
                                switch (typeId) {
                                    case SS_SINGLE: {
                                        if (typeBytes != 4) {
                                            {
                                                mxArray * prhs[ 3 ];
                                                prhs[0] = mxCreateString("PIL:pilverification:SingleUnsupported");
                                                prhs[1] = mxCreateDoubleScalar(4);
                                                prhs[2] = mxCreateDoubleScalar(typeBytes);
                                                callMATLAB(S,  0, NULL, 3, prhs, "DAStudio.error", 0);
                                                return;
                                            }
                                        } /* if */
                                        break;
                                    }
                                    case SS_DOUBLE: {
                                        if (typeBytes != 8) {
                                            {
                                                mxArray * prhs[ 3 ];
                                                prhs[0] = mxCreateString("PIL:pilverification:DoubleUnsupported");
                                                prhs[1] = mxCreateDoubleScalar(8);
                                                prhs[2] = mxCreateDoubleScalar(typeBytes);
                                                callMATLAB(S,  0, NULL, 3, prhs, "DAStudio.error", 0);
                                                return;
                                            }
                                        } /* if */
                                        break;
                                    }
                                    default: {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pilverification:UnknownTypeId");
                                            prhs[1] = mxCreateDoubleScalar(typeId);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                        break;
                                    }
                                } /* switch */
                                break;
                            }
                            default: {
                                {
                                    mxArray * prhs[ 2 ];
                                    prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
                                    prhs[1] = mxCreateDoubleScalar(responseId);
                                    callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                    return;
                                }
                                break;
                            }
                        } /* switch */
                    } /* while */
                } /* if */
            } /* if */
        } /* if */
    }
    /* initialize parameters */
    mdlProcessParameters(S);
    {
        uint8_T * mxMemUnitPtr;
        mwSize dataInSize = 0;
        IOBuffer_T * IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
        if (IOBufferPtr != NULL) {
            growIOBuffer(S, IOBufferPtr, 5);
            if (IOBufferPtr->data != NULL) {
                mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                /* write command id */
                *mxMemUnitPtr = 1;
                mxMemUnitPtr++;
                {
                    uint8_T * simDataMemUnitPtr;
                    uint32_T commandDataFcnid = 0;
                    simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;
                    {
                        size_t num_elements = 1;
                        size_t bytes_consumed = num_elements * sizeof(uint32_T);
                        memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                    }
                }
                {
                    int errorOccurred;
                    errorOccurred = commandDispatch(S, IOBufferPtr, 5, &dataInSize, 0);
                    if (errorOccurred) {
                        return;
                    } /* if */
                }
                if (dataInSize > 0) {
                    size_t dataInMemUnitSize = dataInSize;
                    uint8_T responseId;
                    uint8_T * mxMemUnitPtrEnd;
                    #define RESPONSE_ERROR 0
                    #define RESPONSE_OUTPUT_DATA 1
                    #define RESPONSE_PRINTF 3
                    #define RESPONSE_FOPEN 4
                    #define RESPONSE_FPRINTF 5
                    #define TARGET_IO_SUCCESS 0
                    mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                    mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                    while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                        /* read response id */
                        responseId = *mxMemUnitPtr;
                        mxMemUnitPtr++;
                        switch (responseId) {
                            case RESPONSE_ERROR: {
                                uint8_T errorId = *mxMemUnitPtr;
                                mxMemUnitPtr++;
                                if (errorId) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pilverification:PILError");
                                        prhs[1] = mxCreateDoubleScalar(errorId);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } /* if */
                                break;
                            }
                            case RESPONSE_PRINTF: {
                                uint8_T PRINTF_ERROR;
                                uint16_T PRINTF_SIZE;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                        prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } else {
                                    uint8_T *pPrintBuff;
                                    pPrintBuff = mxMemUnitPtr;
                                    if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
                                        mexPrintf("%s", pPrintBuff);
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + PRINTF_SIZE;
                                break;
                            }
                            case RESPONSE_FOPEN: {
                                uint16_T FOPEN_FID;
                                uint16_T FOPEN_NAME_SIZE;
                                targetIOFd_T *targetIOFdPtr;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                if (targetIOFdPtr != NULL) {
                                    /* check fid increments by 1 */
                                    if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
                                        targetIOFdPtr->fidOffset = FOPEN_FID;
                                        growTargetIOFd(S, targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
                                        if (targetIOFdPtr->Fd != NULL)  {
                                            uint8_T *pFopenBuff;
                                            targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
                                            pFopenBuff = mxMemUnitPtr;
                                            if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
                                                FILE * tmpFd = NULL;
                                                tmpFd = fopen((char *) pFopenBuff,"w");
                                                if (tmpFd != NULL) {
                                                    /* save the file descriptor */
                                                    targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
                                                } else {
                                                    {
                                                        mxArray * prhs[ 2 ];
                                                        prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
                                                        prhs[1] = mxCreateString((char *) pFopenBuff);
                                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                        return;
                                                    }
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } else {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
                                            prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + FOPEN_NAME_SIZE;
                                break;
                            }
                            case RESPONSE_FPRINTF: {
                                uint8_T FPRINTF_ERROR;
                                uint16_T FPRINTF_FID;
                                uint16_T FPRINTF_SIZE;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                        prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } else {
                                    targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                    if (targetIOFdPtr != NULL) {
                                        if (targetIOFdPtr->size > FPRINTF_FID) {
                                            if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
                                                uint8_T *pFprintfBuff;
                                                pFprintfBuff = mxMemUnitPtr;
                                                if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
                                                    fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + FPRINTF_SIZE;
                                break;
                            }
                            case RESPONSE_OUTPUT_DATA: {
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = ( uint8_T *) ssGetOutputPortSignal(S, 0);
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(real64_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = ( uint8_T *) ssGetOutputPortSignal(S, 1);
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(real64_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = ( uint8_T *) ssGetOutputPortSignal(S, 2);
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(real64_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                break;
                            }
                            default: {
                                {
                                    mxArray * prhs[ 2 ];
                                    prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
                                    prhs[1] = mxCreateDoubleScalar(responseId);
                                    callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                    return;
                                }
                                break;
                            }
                        } /* switch */
                    } /* while */
                } /* if */
            } /* if */
        } /* if */
    }
}
#endif /* MDL_START */

#if defined(MDL_PROCESS_PARAMETERS)
static void mdlProcessParameters(SimStruct *S) {
    int isOkToProcessParams;
    {
        {
            mxArray *rhs[3];
            mxArray *lhs[1];
            char * simulinkBlockPath = getSimulinkBlockPath(S);
            if (simulinkBlockPath == NULL) {
                ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
                return;
            }
            rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
            rhs[ 1 ] = mxCreateDoubleScalar( 1 );
            rhs[ 2 ] = mxCreateString(simulinkBlockPath);
            callMATLAB(S,  1, lhs, 3, rhs, "rtw.pil.SILPILInterface.sfunctionIsOkToProcessParamsHook", 0);
            mxFree((void *) simulinkBlockPath);
            isOkToProcessParams = (int) (*mxGetPr(lhs[0]));
            mxDestroyArray(lhs[0]);
        }
    }
    if (isOkToProcessParams) {
        {
            uint8_T * mxMemUnitPtr;
            mwSize dataInSize = 0;
            IOBuffer_T * IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
            if (IOBufferPtr != NULL) {
                growIOBuffer(S, IOBufferPtr, 5);
                if (IOBufferPtr->data != NULL) {
                    mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                    /* write command id */
                    *mxMemUnitPtr = 8;
                    mxMemUnitPtr++;
                    {
                        uint8_T * simDataMemUnitPtr;
                        uint32_T commandDataFcnid = 0;
                        simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(uint32_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        int errorOccurred;
                        errorOccurred = commandDispatch(S, IOBufferPtr, 5, &dataInSize, 0);
                        if (errorOccurred) {
                            return;
                        } /* if */
                    }
                    if (dataInSize > 0) {
                        size_t dataInMemUnitSize = dataInSize;
                        uint8_T responseId;
                        uint8_T * mxMemUnitPtrEnd;
                        #define RESPONSE_ERROR 0
                        #define RESPONSE_PRINTF 3
                        #define RESPONSE_FOPEN 4
                        #define RESPONSE_FPRINTF 5
                        #define TARGET_IO_SUCCESS 0
                        mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                        mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                        while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                            /* read response id */
                            responseId = *mxMemUnitPtr;
                            mxMemUnitPtr++;
                            switch (responseId) {
                                case RESPONSE_ERROR: {
                                    uint8_T errorId = *mxMemUnitPtr;
                                    mxMemUnitPtr++;
                                    if (errorId) {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pilverification:PILError");
                                            prhs[1] = mxCreateDoubleScalar(errorId);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } /* if */
                                    break;
                                }
                                case RESPONSE_PRINTF: {
                                    uint8_T PRINTF_ERROR;
                                    uint16_T PRINTF_SIZE;
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                            prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } else {
                                        uint8_T *pPrintBuff;
                                        pPrintBuff = mxMemUnitPtr;
                                        if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
                                            mexPrintf("%s", pPrintBuff);
                                        } /* if */
                                    } /* if */
                                    mxMemUnitPtr = mxMemUnitPtr + PRINTF_SIZE;
                                    break;
                                }
                                case RESPONSE_FOPEN: {
                                    uint16_T FOPEN_FID;
                                    uint16_T FOPEN_NAME_SIZE;
                                    targetIOFd_T *targetIOFdPtr;
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                    if (targetIOFdPtr != NULL) {
                                        /* check fid increments by 1 */
                                        if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
                                            targetIOFdPtr->fidOffset = FOPEN_FID;
                                            growTargetIOFd(S, targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
                                            if (targetIOFdPtr->Fd != NULL)  {
                                                uint8_T *pFopenBuff;
                                                targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
                                                pFopenBuff = mxMemUnitPtr;
                                                if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
                                                    FILE * tmpFd = NULL;
                                                    tmpFd = fopen((char *) pFopenBuff,"w");
                                                    if (tmpFd != NULL) {
                                                        /* save the file descriptor */
                                                        targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
                                                    } else {
                                                        {
                                                            mxArray * prhs[ 2 ];
                                                            prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
                                                            prhs[1] = mxCreateString((char *) pFopenBuff);
                                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                            return;
                                                        }
                                                    } /* if */
                                                } /* if */
                                            } /* if */
                                        } else {
                                            {
                                                mxArray * prhs[ 2 ];
                                                prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
                                                prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
                                                callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                return;
                                            }
                                        } /* if */
                                    } /* if */
                                    mxMemUnitPtr = mxMemUnitPtr + FOPEN_NAME_SIZE;
                                    break;
                                }
                                case RESPONSE_FPRINTF: {
                                    uint8_T FPRINTF_ERROR;
                                    uint16_T FPRINTF_FID;
                                    uint16_T FPRINTF_SIZE;
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                            prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } else {
                                        targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                        if (targetIOFdPtr != NULL) {
                                            if (targetIOFdPtr->size > FPRINTF_FID) {
                                                if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
                                                    uint8_T *pFprintfBuff;
                                                    pFprintfBuff = mxMemUnitPtr;
                                                    if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
                                                        fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
                                                    } /* if */
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } /* if */
                                    mxMemUnitPtr = mxMemUnitPtr + FPRINTF_SIZE;
                                    break;
                                }
                                default: {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
                                        prhs[1] = mxCreateDoubleScalar(responseId);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                    break;
                                }
                            } /* switch */
                        } /* while */
                    } /* if */
                } /* if */
            } /* if */
        }
    } /* if */
}
#endif /* MDL_PROCESS_PARAMETERS */

#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
static void mdlInitializeConditions(SimStruct *S) {
    if (!ssIsFirstInitCond(S)) {
       ssSetErrorStatus(S, "This In-the-Loop block cannot be called from within a subsystem that can reset its states because the target code has been optimized to remove zero initialization of states. Turn off optimization setting 'Optimize initialization code for model reference' on the 'Optimization' page of the Configuration Parameters dialog of the referenced model to use this block in a subsystem that can reset its states.");
       return;
    }
    {
        uint8_T * mxMemUnitPtr;
        mwSize dataInSize = 0;
        IOBuffer_T * IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
        if (IOBufferPtr != NULL) {
            growIOBuffer(S, IOBufferPtr, 5);
            if (IOBufferPtr->data != NULL) {
                mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                /* write command id */
                *mxMemUnitPtr = 2;
                mxMemUnitPtr++;
                {
                    uint8_T * simDataMemUnitPtr;
                    uint32_T commandDataFcnid = 0;
                    simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;
                    {
                        size_t num_elements = 1;
                        size_t bytes_consumed = num_elements * sizeof(uint32_T);
                        memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                    }
                }
                {
                    int errorOccurred;
                    errorOccurred = commandDispatch(S, IOBufferPtr, 5, &dataInSize, 0);
                    if (errorOccurred) {
                        return;
                    } /* if */
                }
                if (dataInSize > 0) {
                    size_t dataInMemUnitSize = dataInSize;
                    uint8_T responseId;
                    uint8_T * mxMemUnitPtrEnd;
                    #define RESPONSE_ERROR 0
                    #define RESPONSE_PRINTF 3
                    #define RESPONSE_FOPEN 4
                    #define RESPONSE_FPRINTF 5
                    #define TARGET_IO_SUCCESS 0
                    mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                    mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                    while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                        /* read response id */
                        responseId = *mxMemUnitPtr;
                        mxMemUnitPtr++;
                        switch (responseId) {
                            case RESPONSE_ERROR: {
                                uint8_T errorId = *mxMemUnitPtr;
                                mxMemUnitPtr++;
                                if (errorId) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pilverification:PILError");
                                        prhs[1] = mxCreateDoubleScalar(errorId);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } /* if */
                                break;
                            }
                            case RESPONSE_PRINTF: {
                                uint8_T PRINTF_ERROR;
                                uint16_T PRINTF_SIZE;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                        prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } else {
                                    uint8_T *pPrintBuff;
                                    pPrintBuff = mxMemUnitPtr;
                                    if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
                                        mexPrintf("%s", pPrintBuff);
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + PRINTF_SIZE;
                                break;
                            }
                            case RESPONSE_FOPEN: {
                                uint16_T FOPEN_FID;
                                uint16_T FOPEN_NAME_SIZE;
                                targetIOFd_T *targetIOFdPtr;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                if (targetIOFdPtr != NULL) {
                                    /* check fid increments by 1 */
                                    if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
                                        targetIOFdPtr->fidOffset = FOPEN_FID;
                                        growTargetIOFd(S, targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
                                        if (targetIOFdPtr->Fd != NULL)  {
                                            uint8_T *pFopenBuff;
                                            targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
                                            pFopenBuff = mxMemUnitPtr;
                                            if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
                                                FILE * tmpFd = NULL;
                                                tmpFd = fopen((char *) pFopenBuff,"w");
                                                if (tmpFd != NULL) {
                                                    /* save the file descriptor */
                                                    targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
                                                } else {
                                                    {
                                                        mxArray * prhs[ 2 ];
                                                        prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
                                                        prhs[1] = mxCreateString((char *) pFopenBuff);
                                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                        return;
                                                    }
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } else {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
                                            prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + FOPEN_NAME_SIZE;
                                break;
                            }
                            case RESPONSE_FPRINTF: {
                                uint8_T FPRINTF_ERROR;
                                uint16_T FPRINTF_FID;
                                uint16_T FPRINTF_SIZE;
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                {
                                    uint8_T * simDataMemUnitPtr;
                                    simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;
                                    {
                                        size_t num_elements = 1;
                                        size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                        memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                        mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                    }
                                }
                                if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                        prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                } else {
                                    targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                    if (targetIOFdPtr != NULL) {
                                        if (targetIOFdPtr->size > FPRINTF_FID) {
                                            if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
                                                uint8_T *pFprintfBuff;
                                                pFprintfBuff = mxMemUnitPtr;
                                                if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
                                                    fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } /* if */
                                } /* if */
                                mxMemUnitPtr = mxMemUnitPtr + FPRINTF_SIZE;
                                break;
                            }
                            default: {
                                {
                                    mxArray * prhs[ 2 ];
                                    prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
                                    prhs[1] = mxCreateDoubleScalar(responseId);
                                    callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                    return;
                                }
                                break;
                            }
                        } /* switch */
                    } /* while */
                } /* if */
            } /* if */
        } /* if */
    }
}
#endif /* MDL_INITIALIZE_CONDITIONS */

#define MDL_SET_WORK_WIDTHS   /* Change to #undef to remove function */
#if defined(MDL_SET_WORK_WIDTHS)
static void mdlSetWorkWidths(SimStruct *S) {
}
#endif /* MDL_SET_WORK_WIDTHS */

static void mdlOutputs(SimStruct *S, int_T tid) {
    /* Singlerate scheduling */
    /* check for sample time hit associated with task 1 */
    if (ssIsSampleHit(S, ssGetInputPortSampleTimeIndex(S, 0), tid)) {
        {
            uint8_T * mxMemUnitPtr;
            mwSize dataInSize = 0;
            IOBuffer_T * IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
            if (IOBufferPtr != NULL) {
                growIOBuffer(S, IOBufferPtr, 132);
                if (IOBufferPtr->data != NULL) {
                    mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                    /* write command id */
                    *mxMemUnitPtr = 3;
                    mxMemUnitPtr++;
                    {
                        uint8_T * simDataMemUnitPtr;
                        uint32_T commandDataFcnidTID[2] = {0, 1};
                        simDataMemUnitPtr = (uint8_T *) &commandDataFcnidTID[0];
                        {
                            size_t num_elements = 2;
                            size_t bytes_consumed = num_elements * sizeof(uint32_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 0);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus1"), 0));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 0);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus1"), 1));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 0);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus1"), 2));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 0);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus1"), 3));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 0);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus1"), 4));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 1);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus2"), 0));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 1);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus2"), 1));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 1);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus2"), 2));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 1);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus2"), 3));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 1);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus2"), 4));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        slDataTypeAccess * dta = ssGetDataTypeAccess(S);
                        const char * bpath = ssGetPath(S);
                        {
                            simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 1);
                            simDataMemUnitPtr = (uint8_T*) (((char *) simDataMemUnitPtr) +
   dtaGetDataTypeElementOffset(dta, bpath, ssGetDataTypeId(S, "slBus2"), 5));
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(real64_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 2);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 3);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 4);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 5);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 6);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 7);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        uint8_T * simDataMemUnitPtr;
                        simDataMemUnitPtr = ( uint8_T *) ssGetInputPortSignal(S, 8);
                        {
                            size_t num_elements = 1;
                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                            memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                        }
                    }
                    {
                        int errorOccurred;
                        errorOccurred = commandDispatch(S, IOBufferPtr, 132, &dataInSize, 0);
                        if (errorOccurred) {
                            return;
                        } /* if */
                    }
                    if (dataInSize > 0) {
                        size_t dataInMemUnitSize = dataInSize;
                        uint8_T responseId;
                        uint8_T * mxMemUnitPtrEnd;
                        #define RESPONSE_ERROR 0
                        #define RESPONSE_OUTPUT_DATA 1
                        #define RESPONSE_PRINTF 3
                        #define RESPONSE_FOPEN 4
                        #define RESPONSE_FPRINTF 5
                        #define TARGET_IO_SUCCESS 0
                        mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                        mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                        while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                            /* read response id */
                            responseId = *mxMemUnitPtr;
                            mxMemUnitPtr++;
                            switch (responseId) {
                                case RESPONSE_ERROR: {
                                    uint8_T errorId = *mxMemUnitPtr;
                                    mxMemUnitPtr++;
                                    if (errorId) {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pilverification:PILError");
                                            prhs[1] = mxCreateDoubleScalar(errorId);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } /* if */
                                    break;
                                }
                                case RESPONSE_PRINTF: {
                                    uint8_T PRINTF_ERROR;
                                    uint16_T PRINTF_SIZE;
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                            prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } else {
                                        uint8_T *pPrintBuff;
                                        pPrintBuff = mxMemUnitPtr;
                                        if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
                                            mexPrintf("%s", pPrintBuff);
                                        } /* if */
                                    } /* if */
                                    mxMemUnitPtr = mxMemUnitPtr + PRINTF_SIZE;
                                    break;
                                }
                                case RESPONSE_FOPEN: {
                                    uint16_T FOPEN_FID;
                                    uint16_T FOPEN_NAME_SIZE;
                                    targetIOFd_T *targetIOFdPtr;
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                    if (targetIOFdPtr != NULL) {
                                        /* check fid increments by 1 */
                                        if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
                                            targetIOFdPtr->fidOffset = FOPEN_FID;
                                            growTargetIOFd(S, targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
                                            if (targetIOFdPtr->Fd != NULL)  {
                                                uint8_T *pFopenBuff;
                                                targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
                                                pFopenBuff = mxMemUnitPtr;
                                                if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
                                                    FILE * tmpFd = NULL;
                                                    tmpFd = fopen((char *) pFopenBuff,"w");
                                                    if (tmpFd != NULL) {
                                                        /* save the file descriptor */
                                                        targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
                                                    } else {
                                                        {
                                                            mxArray * prhs[ 2 ];
                                                            prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
                                                            prhs[1] = mxCreateString((char *) pFopenBuff);
                                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                            return;
                                                        }
                                                    } /* if */
                                                } /* if */
                                            } /* if */
                                        } else {
                                            {
                                                mxArray * prhs[ 2 ];
                                                prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
                                                prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
                                                callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                                return;
                                            }
                                        } /* if */
                                    } /* if */
                                    mxMemUnitPtr = mxMemUnitPtr + FOPEN_NAME_SIZE;
                                    break;
                                }
                                case RESPONSE_FPRINTF: {
                                    uint8_T FPRINTF_ERROR;
                                    uint16_T FPRINTF_FID;
                                    uint16_T FPRINTF_SIZE;
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                            prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                            return;
                                        }
                                    } else {
                                        targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                        if (targetIOFdPtr != NULL) {
                                            if (targetIOFdPtr->size > FPRINTF_FID) {
                                                if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
                                                    uint8_T *pFprintfBuff;
                                                    pFprintfBuff = mxMemUnitPtr;
                                                    if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
                                                        fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
                                                    } /* if */
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                    } /* if */
                                    mxMemUnitPtr = mxMemUnitPtr + FPRINTF_SIZE;
                                    break;
                                }
                                case RESPONSE_OUTPUT_DATA: {
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = ( uint8_T *) ssGetOutputPortSignal(S, 0);
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = ( uint8_T *) ssGetOutputPortSignal(S, 1);
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    {
                                        uint8_T * simDataMemUnitPtr;
                                        simDataMemUnitPtr = ( uint8_T *) ssGetOutputPortSignal(S, 2);
                                        {
                                            size_t num_elements = 1;
                                            size_t bytes_consumed = num_elements * sizeof(real64_T);
                                            memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                            mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                        }
                                    }
                                    break;
                                }
                                default: {
                                    {
                                        mxArray * prhs[ 2 ];
                                        prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
                                        prhs[1] = mxCreateDoubleScalar(responseId);
                                        callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 0);
                                        return;
                                    }
                                    break;
                                }
                            } /* switch */
                        } /* while */
                    } /* if */
                } /* if */
            } /* if */
        }
    } /* if */
}

static void mdlTerminate(SimStruct *S) {
    int isOkToTerminate;
    int commErrorOccurred = 0;

    {
        if (ssGetPWork(S) != NULL) {
            int * pCommErrorOccurred = (int *) ssGetPWorkValue(S, 3);
            if (pCommErrorOccurred != NULL) {
                commErrorOccurred = *pCommErrorOccurred;
            } /* if */
        } /* if */
    }

    {
        {
            mxArray *rhs[3];
            mxArray *lhs[1];
            char * simulinkBlockPath = getSimulinkBlockPath(S);
            if (simulinkBlockPath == NULL) {
                ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
                return;
            }
            rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
            rhs[ 1 ] = mxCreateDoubleScalar( 1 );
            rhs[ 2 ] = mxCreateString(simulinkBlockPath);
            callMATLAB(S,  1, lhs, 3, rhs, "rtw.pil.SILPILInterface.sfunctionIsOkToTerminateHook", 0);
            mxFree((void *) simulinkBlockPath);
            isOkToTerminate = (int) (*mxGetPr(lhs[0]));
            mxDestroyArray(lhs[0]);
        }
    }
    if (isOkToTerminate) {
        if (!commErrorOccurred) {
            {
                uint8_T * mxMemUnitPtr;
                mwSize dataInSize = 0;
                IOBuffer_T * IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
                if (IOBufferPtr != NULL) {
                    growIOBuffer(S, IOBufferPtr, 5);
                    if (IOBufferPtr->data != NULL) {
                        mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                        /* write command id */
                        *mxMemUnitPtr = 4;
                        mxMemUnitPtr++;
                        {
                            uint8_T * simDataMemUnitPtr;
                            uint32_T commandDataFcnid = 0;
                            simDataMemUnitPtr = (uint8_T *) &commandDataFcnid;
                            {
                                size_t num_elements = 1;
                                size_t bytes_consumed = num_elements * sizeof(uint32_T);
                                memcpy((void *) mxMemUnitPtr, (void *) simDataMemUnitPtr, bytes_consumed);
                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                            }
                        }
                        {
                            int errorOccurred;
                            errorOccurred = commandDispatch(S, IOBufferPtr, 5, &dataInSize, 1);
                            if (errorOccurred) {
                                /* continue */
                            } /* if */
                        }
                        if (dataInSize > 0) {
                            size_t dataInMemUnitSize = dataInSize;
                            uint8_T responseId;
                            uint8_T * mxMemUnitPtrEnd;
                            #define RESPONSE_ERROR 0
                            #define RESPONSE_PRINTF 3
                            #define RESPONSE_FOPEN 4
                            #define RESPONSE_FPRINTF 5
                            #define TARGET_IO_SUCCESS 0
                            mxMemUnitPtr = (uint8_T *) IOBufferPtr->data;
                            mxMemUnitPtrEnd = mxMemUnitPtr + dataInMemUnitSize - 1;
                            while (mxMemUnitPtr <= mxMemUnitPtrEnd) {
                                /* read response id */
                                responseId = *mxMemUnitPtr;
                                mxMemUnitPtr++;
                                switch (responseId) {
                                    case RESPONSE_ERROR: {
                                        uint8_T errorId = *mxMemUnitPtr;
                                        mxMemUnitPtr++;
                                        if (errorId) {
                                            {
                                                mxArray * prhs[ 2 ];
                                                prhs[0] = mxCreateString("PIL:pilverification:PILError");
                                                prhs[1] = mxCreateDoubleScalar(errorId);
                                                callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 1);
                                                /* continue */
                                            }
                                        } /* if */
                                        break;
                                    }
                                    case RESPONSE_PRINTF: {
                                        uint8_T PRINTF_ERROR;
                                        uint16_T PRINTF_SIZE;
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &PRINTF_ERROR;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &PRINTF_SIZE;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        if (PRINTF_ERROR != TARGET_IO_SUCCESS) {
                                            {
                                                mxArray * prhs[ 2 ];
                                                prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                                prhs[1] = mxCreateDoubleScalar(PRINTF_ERROR);
                                                callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 1);
                                                /* continue */
                                            }
                                        } else {
                                            uint8_T *pPrintBuff;
                                            pPrintBuff = mxMemUnitPtr;
                                            if (pPrintBuff[PRINTF_SIZE-1] == '\0') {
                                                mexPrintf("%s", pPrintBuff);
                                            } /* if */
                                        } /* if */
                                        mxMemUnitPtr = mxMemUnitPtr + PRINTF_SIZE;
                                        break;
                                    }
                                    case RESPONSE_FOPEN: {
                                        uint16_T FOPEN_FID;
                                        uint16_T FOPEN_NAME_SIZE;
                                        targetIOFd_T *targetIOFdPtr;
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &FOPEN_FID;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &FOPEN_NAME_SIZE;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                        if (targetIOFdPtr != NULL) {
                                            /* check fid increments by 1 */
                                            if (targetIOFdPtr->fidOffset + 1 == FOPEN_FID) {
                                                targetIOFdPtr->fidOffset = FOPEN_FID;
                                                growTargetIOFd(S, targetIOFdPtr, targetIOFdPtr->fidOffset + 1);
                                                if (targetIOFdPtr->Fd != NULL)  {
                                                    uint8_T *pFopenBuff;
                                                    targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = NULL;
                                                    pFopenBuff = mxMemUnitPtr;
                                                    if (pFopenBuff[FOPEN_NAME_SIZE-1] == '\0') {
                                                        FILE * tmpFd = NULL;
                                                        tmpFd = fopen((char *) pFopenBuff,"w");
                                                        if (tmpFd != NULL) {
                                                            /* save the file descriptor */
                                                            targetIOFdPtr->Fd[targetIOFdPtr->fidOffset] = tmpFd;
                                                        } else {
                                                            {
                                                                mxArray * prhs[ 2 ];
                                                                prhs[0] = mxCreateString("PIL:pil:TargetIOFopenError");
                                                                prhs[1] = mxCreateString((char *) pFopenBuff);
                                                                callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 1);
                                                                /* continue */
                                                            }
                                                        } /* if */
                                                    } /* if */
                                                } /* if */
                                            } else {
                                                {
                                                    mxArray * prhs[ 2 ];
                                                    prhs[0] = mxCreateString("PIL:pil:TargetIOFopenFidError");
                                                    prhs[1] = mxCreateDoubleScalar(FOPEN_FID);
                                                    callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 1);
                                                    /* continue */
                                                }
                                            } /* if */
                                        } /* if */
                                        mxMemUnitPtr = mxMemUnitPtr + FOPEN_NAME_SIZE;
                                        break;
                                    }
                                    case RESPONSE_FPRINTF: {
                                        uint8_T FPRINTF_ERROR;
                                        uint16_T FPRINTF_FID;
                                        uint16_T FPRINTF_SIZE;
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &FPRINTF_ERROR;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint8_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &FPRINTF_FID;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        {
                                            uint8_T * simDataMemUnitPtr;
                                            simDataMemUnitPtr = (uint8_T *) &FPRINTF_SIZE;
                                            {
                                                size_t num_elements = 1;
                                                size_t bytes_consumed = num_elements * sizeof(uint16_T);
                                                memcpy((void *) simDataMemUnitPtr, (void *) mxMemUnitPtr, bytes_consumed);
                                                mxMemUnitPtr+=(bytes_consumed/sizeof(*mxMemUnitPtr));
                                            }
                                        }
                                        if (FPRINTF_ERROR != TARGET_IO_SUCCESS) {
                                            {
                                                mxArray * prhs[ 2 ];
                                                prhs[0] = mxCreateString("PIL:pil:TargetIOError");
                                                prhs[1] = mxCreateDoubleScalar(FPRINTF_ERROR);
                                                callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 1);
                                                /* continue */
                                            }
                                        } else {
                                            targetIOFd_T * targetIOFdPtr = (targetIOFd_T *) ssGetPWorkValue(S, 2);
                                            if (targetIOFdPtr != NULL) {
                                                if (targetIOFdPtr->size > FPRINTF_FID) {
                                                    if (targetIOFdPtr->Fd[FPRINTF_FID] != NULL) {
                                                        uint8_T *pFprintfBuff;
                                                        pFprintfBuff = mxMemUnitPtr;
                                                        if (pFprintfBuff[FPRINTF_SIZE-1] == '\0') {
                                                            fprintf(targetIOFdPtr->Fd[FPRINTF_FID], "%s", pFprintfBuff);
                                                        } /* if */
                                                    } /* if */
                                                } /* if */
                                            } /* if */
                                        } /* if */
                                        mxMemUnitPtr = mxMemUnitPtr + FPRINTF_SIZE;
                                        break;
                                    }
                                    default: {
                                        {
                                            mxArray * prhs[ 2 ];
                                            prhs[0] = mxCreateString("PIL:pilverification:UnknownResponseId");
                                            prhs[1] = mxCreateDoubleScalar(responseId);
                                            callMATLAB(S,  0, NULL, 2, prhs, "DAStudio.error", 1);
                                            /* continue */
                                        }
                                        break;
                                    }
                                } /* switch */
                            } /* while */
                        } /* if */
                    } /* if */
                } /* if */
            }
        } /* if */
        closeAndFreeTargetIOFd(S);
        {
            {
                mxArray *rhs[3];
                char * simulinkBlockPath = getSimulinkBlockPath(S);
                if (simulinkBlockPath == NULL) {
                    ssSetErrorStatus(S, "ModelBlock SIL/PIL unexpected error: getSimulinkBlockPath returned NULL pointer. Check search string was found in ssGetPath.\n");
                    return;
                }
                rhs[ 0 ] = mxCreateString("@coder.connectivity.SimulinkInterface.getSILPILInterface");
                rhs[ 1 ] = mxCreateDoubleScalar( 1 );
                rhs[ 2 ] = mxCreateString(simulinkBlockPath);
                callMATLAB(S,  0, NULL, 3, rhs, "rtw.pil.SILPILInterface.sfunctionPILStopHook", 0);
                mxFree((void *) simulinkBlockPath);
            }
        }
    } /* if */
    if (ssGetPWork(S) != NULL) {
        IOBuffer_T * IOBufferPtr;
        rtIOStreamData * rtIOStreamDataPtr = (rtIOStreamData *) ssGetPWorkValue(S, 0);
        if (rtIOStreamDataPtr != NULL) {
            {
                int errorStatus = rtIOStreamUnloadLib(&rtIOStreamDataPtr->libH);
                if (errorStatus) {
                    ssSetErrorStatus( S,"rtIOStreamUnloadLib failed.");
                } /* if */
            }
            mxFree(rtIOStreamDataPtr->lib);
            mxDestroyArray(rtIOStreamDataPtr->MATLABObject);
            mxFree(rtIOStreamDataPtr);
            ssSetPWorkValue(S, 0, NULL);
        } /* if */
        IOBufferPtr = (IOBuffer_T *) ssGetPWorkValue(S, 1);
        if (IOBufferPtr != NULL) {
            mxFree(IOBufferPtr->data);
            mxFree(IOBufferPtr);
            ssSetPWorkValue(S, 1, NULL);
        } /* if */
        closeAndFreeTargetIOFd(S);
    } /* if */
}

#define MDL_ENABLE
#if defined(MDL_ENABLE)
static void mdlEnable(SimStruct *S) {
}
#endif /* MDL_ENABLE */

#define MDL_DISABLE
#if defined(MDL_DISABLE)
static void mdlDisable(SimStruct *S) {
}
#endif /* MDL_DISABLE */

/* Required S-function trailer */
#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c" /* MEX-file interface mechanism */
#include "fixedpoint.c"
#else
#error Assertion failed: file must be compiled as a MEX-file
#endif

