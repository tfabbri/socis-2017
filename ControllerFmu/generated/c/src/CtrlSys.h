/**
* This file is generated by Overture's VDM-to-C code generator version 0.1.8.
* Website: https://github.com/overturetool/vdm2c
*/

#ifndef CLASSES_CtrlSys_H_
#define CLASSES_CtrlSys_H_

#define VDM_CG

#include "Vdm.h"

/*  include types used in the class */
#include "HardwareInterface.h"
#include "Controller.h"
#include "CtrlSys.h"


/* -------------------------------
 *
 * Quotes
 *
 --------------------------------- */ 
 


/* -------------------------------
 *
 * values / global const
 *
 --------------------------------- */ 
 
extern TVP numFields_2;
extern TVP g_CtrlSys_hwi;
extern TVP g_CtrlSys_ctrl;


/* -------------------------------
 *
 * The class
 *
 --------------------------------- */ 
 

/*  class id  */
#define CLASS_ID_CtrlSys_ID 1

#define CtrlSysCLASS struct CtrlSys*

/*  The vtable ids  */
#define CLASS_CtrlSys__Z7CtrlSysEV 0

struct CtrlSys
{
	
/* Definition of Class: 'CtrlSys' */
	VDM_CLASS_BASE_DEFINITIONS(CtrlSys);
	 
	VDM_CLASS_FIELD_DEFINITION(CtrlSys,numFields);
	
};


/* -------------------------------
 *
 * Constructors
 *
 --------------------------------- */ 
 
  
  	/* CtrlSys.vdmrt 15:9 */
	TVP _Z7CtrlSysEV(CtrlSysCLASS this_); 


/* -------------------------------
 *
 * public access functions
 *
 --------------------------------- */ 
 
	void CtrlSys_const_init();
	void CtrlSys_const_shutdown();
	void CtrlSys_static_init();
	void CtrlSys_static_shutdown();


/* -------------------------------
 *
 * Internal
 *
 --------------------------------- */ 
 

void CtrlSys_free_fields(CtrlSysCLASS);
CtrlSysCLASS CtrlSys_Constructor(CtrlSysCLASS);



#endif /* CLASSES_CtrlSys_H_ */
