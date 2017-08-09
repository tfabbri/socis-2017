/*
 * watertankFmu.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: kel
 */
#include "Fmu.h"
#include "FmuModel.h"

#define PERIODIC_GENERATED



#include <stdarg.h>
#include "Fmu.h"
#include "Vdm.h"

#include "Controller.h"
#include "CtrlSys.h"
#include "HardwareInterface.h"
#include "World.h"
#include "Port.h"
#include "IntPort.h"
#include "BoolPort.h"
#include "RealPort.h"
#include "StringPort.h"
#include "IO.h"

TVP sys = NULL;
fmi2Boolean syncOutAllowed = fmi2True;
fmi2Real maxStepSize = 0.0;


void syncInputsToModel(){
	{
		TVP newValue = newInt(fmiBuffer.intBuffer[5]);
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,mip);
		SET_FIELD(IntPort,IntPort,p,value,newValue);
		vdmFree(newValue);vdmFree(p);
	}
	{
		TVP newValue = newReal(fmiBuffer.realBuffer[6]);
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,mrp);
		SET_FIELD(RealPort,RealPort,p,value,newValue);
		vdmFree(newValue);vdmFree(p);
	}
	{
		TVP newValue = newBool(fmiBuffer.booleanBuffer[4]);
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,mbp);
		SET_FIELD(BoolPort,BoolPort,p,value,newValue);
		vdmFree(newValue);vdmFree(p);
	}
	{
		//TVP newValue = (fmiBuffer.[7]);
		//TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,msp);
		//SET_FIELD(StringPort,StringPort,p,value,newValue);
		//vdmFree(newValue);vdmFree(p);
	}
}
void syncOutputsToBuffers(){
	if(syncOutAllowed == fmi2False) return;

	{
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,cbp);
		TVP v = GET_FIELD(BoolPort,BoolPort,p,value);
		fmiBuffer.booleanBuffer[0]=v->value.boolVal;
		vdmFree(v);vdmFree(p);
	}
	{
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,cip);
		TVP v = GET_FIELD(IntPort,IntPort,p,value);
		fmiBuffer.intBuffer[1]=v->value.intVal;
		vdmFree(v);vdmFree(p);
	}
	{
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,csp);
		TVP v = GET_FIELD(StringPort,StringPort,p,value);
		//fmiBuffer.[3]=v->value.;
		vdmFree(v);vdmFree(p);
	}
	{
		TVP p = GET_FIELD(HardwareInterface,HardwareInterface,g_CtrlSys_hwi,crp);
		TVP v = GET_FIELD(RealPort,RealPort,p,value);
		fmiBuffer.realBuffer[2]=v->value.doubleVal;
		vdmFree(v);vdmFree(p);
	}
}
void periodic_taskg_CtrlSys_ctrl__Z4stepEV()
{
	CALL_FUNC(Controller, Controller, g_CtrlSys_ctrl, CLASS_Controller__Z4stepEV);
	g_fmiCallbackFunctions->logger((void*) 1, g_fmiInstanceName, fmi2OK, "logAll", "called &periodic_taskg_CtrlSys_ctrl__Z4stepEV\n");
}


struct PeriodicThreadStatus threads[] ={
{ 1.0E8, &periodic_taskg_CtrlSys_ctrl__Z4stepEV, 0 }
};


void systemInit()
{
	vdm_gc_init();

	int i;

	for(i = 0; i < PERIODIC_GENERATED_COUNT; i++) threads[i].period = threads[i].period / 1.0E9;

	Controller_const_init();
	CtrlSys_const_init();
	HardwareInterface_const_init();
	World_const_init();
	Port_const_init();
	IntPort_const_init();
	BoolPort_const_init();
	RealPort_const_init();
	StringPort_const_init();
	IO_const_init();

	Controller_static_init();
	CtrlSys_static_init();
	HardwareInterface_static_init();
	World_static_init();
	Port_static_init();
	IntPort_static_init();
	BoolPort_static_init();
	RealPort_static_init();
	StringPort_static_init();
	IO_static_init();

	sys = _Z7CtrlSysEV(NULL);

}


void systemDeInit()
{
	Controller_static_shutdown();
	CtrlSys_static_shutdown();
	HardwareInterface_static_shutdown();
	World_static_shutdown();
	Port_static_shutdown();
	IntPort_static_shutdown();
	BoolPort_static_shutdown();
	RealPort_static_shutdown();
	StringPort_static_shutdown();
	IO_static_shutdown();

	Controller_const_shutdown();
	CtrlSys_const_shutdown();
	HardwareInterface_const_shutdown();
	World_const_shutdown();
	Port_const_shutdown();
	IntPort_const_shutdown();
	BoolPort_const_shutdown();
	RealPort_const_shutdown();
	StringPort_const_shutdown();
	IO_const_shutdown();

	vdmFree(sys);

	vdm_gc_shutdown();
}



/*
* Both time value are given in seconds
*/
fmi2Status vdmStep(fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize)
{
	int i, j;
	int threadRunCount;

	/*  Call each thread the appropriate number of times.  */
	for(i = 0;  i < PERIODIC_GENERATED_COUNT; i++)
	{
		/*  Times align, sync took place last time.  */
		if(threads[i].lastExecuted >= currentCommunicationPoint)
		{
			/*  Can not do anything, still waiting for the last step's turn to come.  */
			if(threads[i].lastExecuted >= currentCommunicationPoint + communicationStepSize)
			{
				threadRunCount = 0;
				syncOutAllowed = fmi2False;
			}
			/*  Previous step will finish inside this step.
			*   At least one execution can be fit inside this step.
			*/
			else if(threads[i].lastExecuted + threads[i].period <= currentCommunicationPoint + communicationStepSize)
			{
				/*  Find number of executions to fit inside of step, allow sync.  */
				threadRunCount = (currentCommunicationPoint + communicationStepSize - threads[i].lastExecuted) / threads[i].period;
				syncOutAllowed = fmi2True;
			}
			/*  Can not execute, but can sync existing values at the end of this step.  */
			else 
			{
				threadRunCount = 0;
				syncOutAllowed = fmi2True;
			}
		}
		else
		{
			/*  Find number of executions to fit inside of step, allow sync because need to update regardless.  */
			threadRunCount = (currentCommunicationPoint + communicationStepSize - threads[i].lastExecuted) / threads[i].period;
			syncOutAllowed = fmi2True;

			/*  Period too long for this step so postpone until next step.  */
			if(threadRunCount == 0)
			{
				syncOutAllowed = fmi2False;
			}
		}		

		/*  Execute each thread the number of times that its period fits in the step size.  */
		for(j = 0; j < threadRunCount; j++)
		{
			threads[i].call();

			/*  Update the thread's last execution time.  */
			threads[i].lastExecuted += threads[i].period;
		}

		vdm_gc();
	}

	/* Calculate maximum step size for next step.  Cyclic controllers with no feedback do not have
	a limit on how large a step they can take.  To be considered in the future for controllers
	with feedback.
	*/
	maxStepSize = INT_MAX * 1.0;

	/*  g_fmiCallbackFunctions->logger((void*) 1, g_fmiInstanceName, fmi2OK, "logDebug", "NOW:  %f, TP: %f, LE:  %f, STEP:  %f, SYNC:  %d, RUNS:  %d\n", currentCommunicationPoint, threads[0].period, threads[0].lastExecuted, communicationStepSize, syncOutAllowed, threadRunCount);  */

	return fmi2OK;
}

void systemMain()
{
	TVP world = _Z5WorldEV(NULL);
	CALL_FUNC(World, World, world, CLASS_World__Z3runEV);
	vdmFree(world);
}
