/*
 * fmu.h
 *
 *  Created on: Mar 3, 2016
 *      Author: kel
 */

#ifndef FMU_H_
#define FMU_H_

#define BOOL_COUNT 8
#define REAL_COUNT 8
#define INT_COUNT 8

#include "fmi2Functions.h"
#include "FmuGUID.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>

struct FmiBuffer{
	 fmi2Boolean booleanBuffer[BOOL_COUNT];
	 fmi2Real realBuffer[REAL_COUNT];
	 fmi2Integer intBuffer[INT_COUNT];
};

struct PeriodicThreadStatus
{
	fmi2Real period;
	void (*call)(void);
	fmi2Real lastExecuted;
};

extern struct FmiBuffer fmiBuffer;
extern const fmi2CallbackFunctions *g_fmiCallbackFunctions;
extern const char* g_fmiInstanceName;

extern struct PeriodicThreadStatus threads[];

void syncInputsToModel();
void syncOutputsToBuffers();
void systemInit();
void systemDeInit();
void systemMain();


fmi2Status vdmStep(fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize);

#endif /* FMU_H_ */
