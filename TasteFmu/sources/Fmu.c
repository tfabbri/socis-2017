/*
 * fmi.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: parallels
 */

#define FMI_COSIMULATION

#include "Fmu.h"

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <common.h>
#include <signal.h>
#include <limits.h>


bool terminateSystemThread = false;
bool isTerminated = false;
struct FmiBuffer fmiBuffer;
const fmi2CallbackFunctions *g_fmiCallbackFunctions;
const char* g_fmiInstanceName;
static const char* resourcesLocation; // AbsolutePath of the resource location directory

/* ---------------------------------------------------------------------------
 *  TASTE interface variables
 * ---------------------------------------------------------------------------
 */

#define N_TASTE_PI 1
sem_t* semaphore;  // Mutex on the shared memory
struct shm_struct *shm_memory;
int shm_memory_fd;
int taste_id_process;
int taste_log_fd;
static const char taste_log_path[] = "./taste-output.log";
static const char taste_app_path[] = "binary.c/binaries/x86_partition";


/* ---------------------------------------------------------------------------
*  FMI functions
*  ---------------------------------------------------------------------------
*/

fmi2Component fmi2Instantiate(fmi2String instanceName, fmi2Type fmuType, fmi2String fmuGUID,
		fmi2String fmuResourcesLocation, const fmi2CallbackFunctions *functions, fmi2Boolean visible,
		fmi2Boolean loggingOn)
{

    char *tmpInstanceName;
    char *tmpResourcesLocation;
    
    if(strcmp(fmuGUID, _FMU_GUID) != 0)
    {
	    g_fmiCallbackFunctions->logger((void*) 1,g_fmiInstanceName,fmi2Error,"logError","%s\n", "GUID mismatch.");
	    return NULL;
    }

    tmpInstanceName = (char*)malloc(strlen(instanceName) + 1);
    strcpy(tmpInstanceName, instanceName);
    g_fmiInstanceName = tmpInstanceName;

    tmpResourcesLocation = (char*)malloc(strlen(fmuResourcesLocation) + 1);
    strcpy(tmpResourcesLocation, fmuResourcesLocation);
    resourcesLocation = tmpResourcesLocation;
    char *taste_app_abs_path;
    printf("%s\n", resourcesLocation);
    printf("%s\n", taste_app_path);
    taste_app_abs_path = g_build_path("/", resourcesLocation, taste_app_path, NULL);
    printf("%s\n", taste_app_abs_path);
    taste_app_abs_path = taste_app_abs_path + 5; //removing the 'file:' from the path
    printf("%s\n", taste_app_abs_path);

    //resourcesLocation = (char*)calloc(strlen(fmuResourceLocation) + 1, sizeof(char));
    //strcpy(resourcesLocation, fmuResourceLocation);
    
    semaphore = sem_open(SEM_NAME, O_CREAT, SEM_PERMS, 1);

    if (semaphore == SEM_FAILED) {
        perror("Sem_open(3) error");
        exit(1);
    }

    if((shm_memory_fd = shm_open(SHM_NAME,(O_CREAT|O_RDWR), 0666)) == -1){
        perror("Shm get error"); 
        exit(1);
    }
    ftruncate(shm_memory_fd, sizeof(struct shm_struct));
    shm_memory = (struct shm_struct *) mmap(0, sizeof(struct shm_struct), PROT_WRITE, MAP_SHARED, shm_memory_fd, 0);

    taste_id_process = fork();

    if(taste_id_process == 0){
        // Child process - Execute the TASTE app
        //pid_taste_child = getpid();
        printf("Running the TASTE APP as child process\n");
        // Redirect the output of taste app to log file
        taste_log_fd = open( taste_log_path, O_RDWR | O_CREAT | S_IRWXU | S_IRUSR | S_IWUSR);
        dup2(taste_log_fd, 1);
        dup2(taste_log_fd, 2);
        close(taste_log_fd);
        execl(taste_app_abs_path, taste_app_abs_path, NULL);
        perror("EXECL failed");
        exit(127);
    }

    if(taste_id_process > 0){
        // Parent process - Do Nothing and continue its running
        printf("Running the FMI interface parent process\n");
        printf("taste_id_process %d\n", taste_id_process);
    }

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
    printf("Freeing the instance %d", taste_id_process);
    kill(taste_id_process, SIGKILL);
    sem_unlink(SEM_NAME);
    shm_unlink(SHM_NAME);
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
    //Synchronize Inputs
    fmi2Boolean syncOutAllowed = fmi2True;

    sem_wait(semaphore);
    shm_memory->mbp = fmiBuffer.booleanBuffer[3]; 
    shm_memory->mip = fmiBuffer.intBuffer[4];
    shm_memory->mrp = fmiBuffer.realBuffer[5];
    sem_post(semaphore);

    int i, j, threadRunCount;
    for(i=0; i<N_TASTE_PI; i++){
        if(threads[i].lastExecuted >= currentCommunicationPoint){
        
        
        }
    
    
    }

    
    //Synchronize Outputs
    if(syncOutAllowed == fmi2True){ 
        sem_wait(semaphore);
        fmiBuffer.booleanBuffer[0] = shm_memory->cbp;
        fmiBuffer.intBuffer[1] = shm_memory->cip;
        fmiBuffer.realBuffer[2] = shm_memory->crp;
        sem_post(semaphore);
    }
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
	*size = INT_MAX * 1.0;
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
