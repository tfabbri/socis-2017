/*
 * fmi.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: parallels
 */

#define FMI_COSIMULATION

#include "Fmu.h"

#include <string.h>

bool terminateSystemThread = false;
bool isTerminated = false;
struct FmiBuffer fmiBuffer;
const fmi2CallbackFunctions *g_fmiCallbackFunctions;
const char* g_fmiInstanceName;
extern char* resourcesLocation;
extern fmi2Real maxStepSize;


/* ---------------------------------------------------------------------------
*  FMI functions
*  ---------------------------------------------------------------------------
*/
fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,
		fmi2String fmuResourceLocation, const fmi2CallbackFunctions *functions, fmi2Boolean visible,
		fmi2Boolean loggingOn)
{
	char *tmpInstanceName;
	
	g_fmiCallbackFunctions = functions;
	
	if(strcmp(fmuGUID, _FMU_GUID) != 0)
	{
		g_fmiCallbackFunctions->logger((void*) 1,g_fmiInstanceName,fmi2Error,"logError","%s\n", "GUID mismatch.");
		return NULL;
	}

	tmpInstanceName = (char*)malloc(strlen(instanceName) + 1);
	strcpy(tmpInstanceName, instanceName);
	g_fmiInstanceName = tmpInstanceName;

	resourcesLocation = (char*)calloc(strlen(fmuResourceLocation) + 1, sizeof(char));
	strcpy(resourcesLocation, fmuResourceLocation);
	
	systemInit();

	return (void*) 1;
}

fmi2Status fmi2SetupExperiment(fmi2Component c, fmi2Boolean toleranceDefined, fmi2Real tolerance,
		fmi2Real startTime, fmi2Boolean stopTimeDefined, fmi2Real stopTime)
{
	return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c)
{
	return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c)
{	
	return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c)
{
	return fmi2OK;
}

fmi2Status fmi2Reset(fmi2Component c)
{
	return fmi2OK;
}

void fmi2FreeInstance(fmi2Component c)
{
	systemDeInit();
}

/* ---------------------------------------------------------------------------
*  FMI functions: class methods not depending of a specific model instance
*  ---------------------------------------------------------------------------
*/

const char* fmi2GetVersion()
{
	return fmi2Version;
}

const char* fmi2GetTypesPlatform()
{
	return fmi2TypesPlatform;
}

/* ---------------------------------------------------------------------------
*  FMI functions: logging control, setters and getters for Real, Integer,
*  Boolean, String
*  ---------------------------------------------------------------------------
*/

fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCategories,
		const fmi2String categories[])
{
	return fmi2OK;
}

fmi2Status fmi2GetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[])
{
	int i;

	for (i = 0; i < nvr; i++)
	{
		fmi2ValueReference vRef = vr[i];
		value[i] = fmiBuffer.realBuffer[vRef];
	}
	return fmi2OK;
}

fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[])
{
	int i;

	for (i = 0; i < nvr; i++)
	{
		fmi2ValueReference vRef = vr[i];
		value[i] = fmiBuffer.intBuffer[vRef];
	}
	return fmi2OK;
}

fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Boolean value[])
{
	int i;

	for (i = 0; i < nvr; i++)
	{
		fmi2ValueReference vRef = vr[i];
		value[i] = fmiBuffer.booleanBuffer[vRef];
	}

	return fmi2OK;
}

fmi2Status fmi2GetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2String value[])
{
	return fmi2OK;
}

fmi2Status fmi2SetReal(fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[])
{
	int i;

	for (i = 0; i < nvr; i++)
	{
		fmi2ValueReference vRef = vr[i];
		fmiBuffer.realBuffer[vRef] = value[i];
		/*  printf("FMI real id=%d set to: %f\n", vRef, fmiBuffer.realBuffer[vRef]);  */
	}

	return fmi2OK;
}

fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
		const fmi2Integer value[])
{
	int i;

	for (i = 0; i < nvr; i++)
	{
		fmi2ValueReference vRef = vr[i];
		fmiBuffer.intBuffer[vRef] = value[i];
		/*  printf("FMI real id=%d set to: %f\n", vRef, fmiBuffer.intBuffer[vRef]);  */
	}

	return fmi2OK;
}

fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
		const fmi2Boolean value[])
{
	int i;

	for (i = 0; i < nvr; i++)
	{
		fmi2ValueReference vRef = vr[i];
		fmiBuffer.booleanBuffer[vRef] = value[i];
		/*  printf("FMI real id=%d set to: %f\n", vRef, fmiBuffer.boolBuffer[vRef]);  */
	}

	return fmi2OK;
}

fmi2Status fmi2SetString(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
		const fmi2String value[])
{
	return fmi2OK;
}

fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate* FMUstate)
{
	return fmi2OK;
}
fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate FMUstate)
{
	return fmi2OK;
}
fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate* FMUstate)
{
	return fmi2OK;
}
fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate FMUstate, size_t *size)
{
	return fmi2OK;
}
fmi2Status fmi2SerializeFMUstate(fmi2Component c, fmi2FMUstate FMUstate, fmi2Byte serializedState[],
		size_t size)
{
	return fmi2OK;
}
fmi2Status fmi2DeSerializeFMUstate(fmi2Component c, const fmi2Byte serializedState[], size_t size,
		fmi2FMUstate* FMUstate)
{
	return fmi2OK;
}

fmi2Status fmi2GetDirectionalDerivative(fmi2Component c, const fmi2ValueReference vUnknown_ref[],
		size_t nUnknown, const fmi2ValueReference vKnown_ref[], size_t nKnown, const fmi2Real dvKnown[],
		fmi2Real dvUnknown[])
{
	return fmi2OK;
}

/* ---------------------------------------------------------------------------
*  Functions for FMI for Co-Simulation
*  ---------------------------------------------------------------------------
*/
#ifdef FMI_COSIMULATION
/* Simulating the slave */
fmi2Status fmi2SetRealInputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
		const fmi2Integer order[], const fmi2Real value[])
{
	return fmi2OK;
}

fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c, const fmi2ValueReference vr[], size_t nvr,
		const fmi2Integer order[], fmi2Real value[])
{
	return fmi2OK;
}

fmi2Status fmi2CancelStep(fmi2Component c)
{
	return fmi2OK;
}

fmi2Status fmi2DoStep(fmi2Component c, fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize,
		fmi2Boolean noSetFMUStatePriorToCurrentPoint)
{

	//syncInputsToModel();
	//stepStatus = vdmStep(currentCommunicationPoint, communicationStepSize);
	//syncOutputsToBuffers();
    fmiBuffer.realBuffer[2] = 0;
    fmiBuffer.booleanBuffer[3] = false;
    fmiBuffer.intBuffer[1] = -10;
	return fmi2OK;
}

fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status *value)
{
	return fmi2OK;
}

fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real *value)
{
	return fmi2OK;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer *value)
{
	return fmi2OK;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean *value)
{
	return fmi2OK;
}

fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String *value)
{
	return fmi2OK;
}

/* INTO cps specific*/
fmi2Status fmi2GetMaxStepsize(fmi2Component c, fmi2Real* size)
{	
	*size = maxStepSize;
	return fmi2OK;
}



/* ---------------------------------------------------------------------------
*  Functions for FMI2 for Model Exchange
*  ---------------------------------------------------------------------------
*/
#else
/* Enter and exit the different modes */
fmi2Status fmi2EnterEventMode(fmi2Component c)
{
	return fmi2Error;
}

fmi2Status fmi2NewDiscreteStates(fmi2Component c, fmi2EventInfo *eventInfo)
{
	return fmi2Error;
}

fmi2Status fmi2EnterContinuousTimeMode(fmi2Component c)
{
	return fmi2Error;
}

fmi2Status fmi2CompletedIntegratorStep(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint,
		fmi2Boolean *enterEventMode, fmi2Boolean *terminateSimulation)
{
	return fmi2Error;
}

/* Providing independent variables and re-initialization of caching */
fmi2Status fmi2SetTime(fmi2Component c, fmi2Real time)
{
	return fmi2Error;
}

fmi2Status fmi2SetContinuousStates(fmi2Component c, const fmi2Real x[], size_t nx)
{
	return fmi2Error;
}

/* Evaluation of the model equations */
fmi2Status fmi2GetDerivatives(fmi2Component c, fmi2Real derivatives[], size_t nx)
{
	return fmi2Error;
}

fmi2Status fmi2GetEventIndicators(fmi2Component c, fmi2Real eventIndicators[], size_t ni)
{
	return fmi2Error;
}

fmi2Status fmi2GetContinuousStates(fmi2Component c, fmi2Real states[], size_t nx)
{
	return fmi2Error;
}

fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component c, fmi2Real x_nominal[], size_t nx)
{
	return fmi2Error;
}
#endif /*  Model Exchange  */
