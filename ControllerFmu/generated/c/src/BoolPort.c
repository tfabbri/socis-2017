/**
* This file is generated by Overture's VDM-to-C code generator version 0.1.8.
* Website: https://github.com/overturetool/vdm2c
*/

#include "BoolPort.h"
#include <stdio.h>
#include <string.h>


/* -------------------------------
 *
 * Memory management methods
 *
 --------------------------------- */

void BoolPort_free_fields(struct BoolPort *this)
{
		vdmFree(this->m_BoolPort_numFields);
			vdmFree(this->m_BoolPort_value);
	}

static void BoolPort_free(struct BoolPort *this)
{
	--this->_BoolPort_refs;
	if (this->_BoolPort_refs < 1)
	{
		BoolPort_free_fields(this);
		free(this);
	}
}


/* -------------------------------
 *
 * Member methods 
 *
 --------------------------------- */
 

 static  TVP _Z18fieldInitializer24EV(BoolPortCLASS this)	{
/* Fmi.vdmrt 56:15 */
TVP ret_24 = newBool(false)
;

return ret_24;
}



 static  TVP _Z18fieldInitializer23EV(BoolPortCLASS this)	{

TVP ret_25 = newInt(1)
;

return ret_25;
}


/* Fmi.vdmrt 62:9 */
 static  void _Z8setValueEB(BoolPortCLASS this, TVP v)	{
/* Fmi.vdmrt 63:24 */
TVP field_tmp_3 = vdmCloneGC(v, NULL)
;

SET_FIELD_PTR(BoolPort, BoolPort, this, value, field_tmp_3);

vdmFree(field_tmp_3);
}


/* Fmi.vdmrt 65:14 */
 static  TVP _Z8getValueEV(BoolPortCLASS this)	{
/* Fmi.vdmrt 66:23 */
TVP ret_26 = GET_FIELD_PTR_GC(BoolPort, BoolPort, this, value)
;
/* Fmi.vdmrt 66:16 */
return ret_26;
}



 void BoolPort_const_init()	{

return ;
}



 void BoolPort_const_shutdown()	{

return ;
}



 void BoolPort_static_init()	{

return ;
}



 void BoolPort_static_shutdown()	{

return ;
}


/* Fmi.vdmrt 31:14 */
 static  TVP Port__Z8getValueEV(PortCLASS base)	{

BoolPortCLASS this = CLASS_DOWNCAST(Port, BoolPort, base)
;

return _Z8getValueEV(this);
}




/* -------------------------------
 *
 * VTable
 *
 --------------------------------- */
 
/*  VTable for this class  */
 static  struct VTable VTableArrayForBoolPort  [] ={

{0,0,((VirtualFunctionPointer) _Z8BoolPortEB),},
{0,0,((VirtualFunctionPointer) _Z8setValueEB),},
{0,0,((VirtualFunctionPointer) _Z8getValueEV),},
{0,0,((VirtualFunctionPointer) _Z8BoolPortEV),},
				
}  ;

/*  Overload VTables  */
 static  struct VTable g_VTableArrayForBoolPort_Override_Port  [3]  ;


/* -------------------------------
 *
 * Internal memory constructor
 *
 --------------------------------- */
 
 
BoolPortCLASS BoolPort_Constructor(BoolPortCLASS this_ptr)
{

	if(this_ptr==NULL)
	{
		this_ptr = (BoolPortCLASS) malloc(sizeof(struct BoolPort));
	}

	if(this_ptr!=NULL)
	{
	
			
		/*  init base Port  */
		Port_Constructor((PortCLASS)CLASS_CAST(this_ptr,BoolPort,Port));

					if(! g_VTableArrayForBoolPort_Override_Port[0].pFunc)
			{
				/*  obtain original Port VTable  */
				/*  This will require rework for multiple inheritance, specifically the "... [0].getVtable..."  */
				memcpy( g_VTableArrayForBoolPort_Override_Port, this_ptr->_Port_pVTable, sizeof( struct VTable) * 3);
				
				/*  install overrides  */
									g_VTableArrayForBoolPort_Override_Port[CLASS_Port__Z8getValueEV].pFunc = (VirtualFunctionPointer)Port__Z8getValueEV; /*  override  */
					
							}
			this_ptr->_Port_pVTable = g_VTableArrayForBoolPort_Override_Port;
				
		
		/*  BoolPort init  */
		this_ptr->_BoolPort_id = CLASS_ID_BoolPort_ID;
		this_ptr->_BoolPort_refs = 0;
		this_ptr->_BoolPort_pVTable=VTableArrayForBoolPort;

				this_ptr->m_BoolPort_numFields= _Z18fieldInitializer23EV(this_ptr);
						this_ptr->m_BoolPort_value= _Z18fieldInitializer24EV(this_ptr);
			}

	return this_ptr;
}

/*  Method for creating new "class"  */
static TVP new()
{
	BoolPortCLASS ptr=BoolPort_Constructor(NULL);

	return newTypeValue(VDM_CLASS, (TypedValueType)
			{	.ptr=newClassValue(ptr->_BoolPort_id, &ptr->_BoolPort_refs, (freeVdmClassFunction)&BoolPort_free, ptr)});
}



/* -------------------------------
 *
 * Public class constructors
 *
 --------------------------------- */ 
 

    /* Fmi.vdmrt 59:9 */
 TVP _Z8BoolPortEB(BoolPortCLASS this, TVP v)	{

TVP __buf = NULL
;

if ( this == NULL )
	
	{

__buf = new();

this = TO_CLASS_PTR(__buf, BoolPort);
}
;
/* Fmi.vdmrt 60:15 */
CALL_FUNC_PTR(BoolPort, BoolPort, this, CLASS_BoolPort__Z8setValueEB, v);

return __buf;
}


    /* Fmi.vdmrt 53:7 */
 TVP _Z8BoolPortEV(BoolPortCLASS this)	{

TVP __buf = NULL
;

if ( this == NULL )
	
	{

__buf = new();

this = TO_CLASS_PTR(__buf, BoolPort);
}
;

return __buf;
}




/* -------------------------------
 *
 * Global class fields
 *
 --------------------------------- */
 
/*  initialize globals - this is done last since they are declared in the header but uses init functions which are printet in any order  */
		
