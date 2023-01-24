/*
 * File: xil_interface.c
 *
 * SIL generated interface for code: "Autopilot"
 *
 */

#include "xil_interface.h"
#include "Autopilot.h"
/* Functions with a C call interface */
#ifdef __cplusplus
extern "C" {
#endif
#include "xil_data_stream.h"
#ifdef __cplusplus
}
#endif

/* interface data */
slBus1 AirData;

/* interface data */
slBus2 Inertial;

/* interface data */
boolean_T APEng;

/* interface data */
boolean_T HDGMode;

/* interface data */
boolean_T ALTMode;

/* interface data */
real_T HDGRef;

/* interface data */
real_T TurnKnob;

/* interface data */
real_T ALTRef;

/* interface data */
real_T PitchWheel;

/* interface data */
real_T AileronCmd;

/* interface data */
real_T ElevatorCmd;

/* interface data */
real_T RudderCmd;

/* interface data */
rtDW_Autopilot localDW;

static XILIOData xil_fcnid0_task1_u[19];

static XILIOData xil_fcnid0_task1_y[4];

static XILIOData xil_fcnid0_init_y[4];


/* In-the-Loop Interface functions - see xil_interface.h */

XIL_INTERFACE_ERROR_CODE xilProcessParams(uint32_T xilFcnId) {
    /* Single In-the-Loop Component */
    if (xilFcnId != 0) {
       return XIL_INTERFACE_UNKNOWN_FCNID;
    }
    return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilGetDataTypeInfo(void) {
    {
        /* send response id code */
        MemUnit_T memUnitData = XIL_RESPONSE_TYPE_SIZE;
        if (xilWriteData(&memUnitData, sizeof(memUnitData)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }
        /* send type id */
        memUnitData = 0;
        if (xilWriteData(&memUnitData, sizeof(memUnitData)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }
        /* PIL_DOUBLE_SIZE should only be already defined for MathWorks testing */
        #ifndef PIL_DOUBLE_SIZE
            #define PIL_DOUBLE_SIZE sizeof(double)
        #endif
        /* send size in bytes */
        memUnitData = (MemUnit_T) PIL_DOUBLE_SIZE;
        #ifndef HOST_WORD_ADDRESSABLE_TESTING
        /* convert MemUnits to bytes */
        memUnitData *= MEM_UNIT_BYTES;
        #endif
        if (xilWriteData(&memUnitData, sizeof(memUnitData)) != XIL_DATA_STREAM_SUCCESS) {
            return XIL_INTERFACE_COMMS_FAILURE;
        }
    }
    return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilInitialize(uint32_T xilFcnId) {
    XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;
    /* initialize output storage owned by In-the-Loop */
    {
        AileronCmd = 0;
    }
    {
        ElevatorCmd = 0;
    }
    {
        RudderCmd = 0;
    }
    /* Single In-the-Loop Component */
    if (xilFcnId == 0) {
        Autopilot_initialize(&(localDW));
        Autopilot_Start(&(localDW));
    }
    else {
        errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
    }
    return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilInitializeConditions(uint32_T xilFcnId) {
    XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;
    /* Single In-the-Loop Component */
    if (xilFcnId == 0) {
        /* No Initialize Conditions Function to Call */
    }
    else {
        errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
    }
    return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilGetUIOData(uint32_T xilFcnId, XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData ** xilIOData) {
    XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;
    *xilIOData = 0;
    /* Single In-the-Loop Component */
    if (xilFcnId != 0) {
       errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
       return errorCode;
    }
    switch(xilCommandType) {
        case XIL_STEP_COMMAND: {
            static int initComplete = 0;
            if (!initComplete) {
                uint32_T tableIdx = 0;
                {
                    void * dataAddress = (void *) &(AirData.altRate);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(AirData.alpha);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(AirData.beta);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(AirData.airspeed);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(AirData.alt);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(Inertial.phi);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(Inertial.theta);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(Inertial.psi);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(Inertial.p);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(Inertial.q);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(Inertial.r);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(APEng);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(boolean_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(HDGMode);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(boolean_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(ALTMode);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(boolean_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(HDGRef);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(TurnKnob);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(ALTRef);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(PitchWheel);
                    xil_fcnid0_task1_u[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                xil_fcnid0_task1_u[tableIdx].memUnitLength = 0;
                xil_fcnid0_task1_u[tableIdx++].address = (MemUnit_T *) 0;
                initComplete = 1;
            }
            *xilIOData = &xil_fcnid0_task1_u[0];
            break;
        }
        default:
           errorCode = XIL_INTERFACE_UNKNOWN_TID;
           break;
    }
    UNUSED_PARAMETER(xilCommandIdx);
    return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilOutput(uint32_T xilFcnId, uint32_T xilTID) {
    /* Single In-the-Loop Component */
    if (xilFcnId != 0) {
       return XIL_INTERFACE_UNKNOWN_FCNID;
    }
    switch(xilTID) {
        case 1:
            Autopilot(&(AirData), &(Inertial), &(APEng), &(HDGMode), &(ALTMode), &(HDGRef), &(TurnKnob), &(ALTRef), &(PitchWheel), &(AileronCmd), &(ElevatorCmd), &(RudderCmd), &(localDW));
            break;
        default:
           return XIL_INTERFACE_UNKNOWN_TID;
    }
    return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilUpdate(uint32_T xilFcnId, uint32_T xilTID) {
    /* Single In-the-Loop Component */
    if (xilFcnId != 0) {
       return XIL_INTERFACE_UNKNOWN_FCNID;
    }
    /* No Update Function */
    UNUSED_PARAMETER(xilTID);
    return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilGetYIOData(uint32_T xilFcnId, XIL_COMMAND_TYPE_ENUM xilCommandType, uint32_T xilCommandIdx, XILIOData ** xilIOData) {
    XIL_INTERFACE_ERROR_CODE errorCode = XIL_INTERFACE_SUCCESS;
    /* Single In-the-Loop Component */
    *xilIOData = 0;
    if (xilFcnId != 0) {
       errorCode = XIL_INTERFACE_UNKNOWN_FCNID;
       return errorCode;
    }
    switch(xilCommandType) {
        case XIL_INITIALIZE_COMMAND: {
            static int initComplete = 0;
            if (!initComplete) {
                uint32_T tableIdx = 0;
                {
                    void * dataAddress = (void *) &(AileronCmd);
                    xil_fcnid0_init_y[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_init_y[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(ElevatorCmd);
                    xil_fcnid0_init_y[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_init_y[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(RudderCmd);
                    xil_fcnid0_init_y[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_init_y[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                xil_fcnid0_init_y[tableIdx].memUnitLength = 0;
                xil_fcnid0_init_y[tableIdx++].address = (MemUnit_T *) 0;
                initComplete = 1;
            }
            *xilIOData = &xil_fcnid0_init_y[0];
            break;
        }
        case XIL_STEP_COMMAND: {
            static int initComplete = 0;
            if (!initComplete) {
                uint32_T tableIdx = 0;
                {
                    void * dataAddress = (void *) &(AileronCmd);
                    xil_fcnid0_task1_y[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(ElevatorCmd);
                    xil_fcnid0_task1_y[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                {
                    void * dataAddress = (void *) &(RudderCmd);
                    xil_fcnid0_task1_y[tableIdx].memUnitLength = 1 * sizeof(real_T);
                    xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) dataAddress;
                }
                xil_fcnid0_task1_y[tableIdx].memUnitLength = 0;
                xil_fcnid0_task1_y[tableIdx++].address = (MemUnit_T *) 0;
                initComplete = 1;
            }
            *xilIOData = &xil_fcnid0_task1_y[0];
            break;
        }
        default:
           errorCode = XIL_INTERFACE_UNKNOWN_TID;
           break;
    }
    UNUSED_PARAMETER(xilCommandIdx);
    return errorCode;
}

XIL_INTERFACE_ERROR_CODE xilTerminate(uint32_T xilFcnId) {
   /* Single In-the-Loop Component */
   if (xilFcnId != 0) {
      return XIL_INTERFACE_UNKNOWN_FCNID;
   }
   /* No Terminate Function to Call */

   return XIL_INTERFACE_SUCCESS;
}

XIL_INTERFACE_ERROR_CODE xilEnable(uint32_T xilFcnId, uint32_T xilTID) {
    /* Single In-the-Loop Component */
    if (xilFcnId != 0) {
       return XIL_INTERFACE_UNKNOWN_FCNID;
    }
    UNUSED_PARAMETER(xilTID);
    /* No Enable Function - this function should never be called */
    return XIL_INTERFACE_UNKNOWN_TID;
}

XIL_INTERFACE_ERROR_CODE xilDisable(uint32_T xilFcnId, uint32_T xilTID) {
    /* Single In-the-Loop Component */
    if (xilFcnId != 0) {
       return XIL_INTERFACE_UNKNOWN_FCNID;
    }
    UNUSED_PARAMETER(xilTID);
    /* No Disable Function - this function should never be called */
    return XIL_INTERFACE_UNKNOWN_TID;
}

