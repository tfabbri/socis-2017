/**
* This file is generated by Overture's VDM-to-C code generator version 0.1.9-SNAPSHOT.
* Website: https://github.com/overturetool/vdm2c
*/

#include "Port.h"
#include <stdio.h>
#include <string.h>


/* -------------------------------
 *
 * Memory management methods
 *
 --------------------------------- */

void Port_free_fields(struct Port *this)
{
		vdmFree(this->m_Port_numFields);
		}

static void Port_free(struct Port *this)
{
	--this->_Port_refs;
	if (this->_Port_refs < 1)
	{
		Port_free_fields(this);
		free(this);
	}
}


/* -------------------------------
 *
 * Member methods 
 *
 --------------------------------- */
 

 static  TVP _Z18fieldInitializer10EV()	{

TVP ret_9 = newInt(0)
;

return ret_9;
}



 static  TVP _Z17fieldInitializer9EV(PortCLASS this)	{

TVP ret_10 = newInt(0)
;

return ret_10;
}


/* Fmi.vdmrt 28:9 */
 static  void _Z8setValueE5X1QCBR(PortCLASS this, TVP v)	{

return ;
}


/* Fmi.vdmrt 31:14 */
 static  TVP _Z8getValueEV(PortCLASS this)	{

return NULL;
}



 void Port_const_init()	{

numFields_4 = _Z18fieldInitializer10EV();

return ;
}



 void Port_const_shutdown()	{

vdmFree(numFields_4);

return ;
}



 void Port_static_init()	{

return ;
}



 void Port_static_shutdown()	{

return ;
}




/* -------------------------------
 *
 * VTable
 *
 --------------------------------- */
 
/*  VTable for this class  */
 static  struct VTable VTableArrayForPort  [] ={

{0,0,((VirtualFunctionPointer) _Z8setValueE5X1QCBR),},
{0,0,((VirtualFunctionPointer) _Z8getValueEV),},
{0,0,((VirtualFunctionPointer) _Z4PortEV),},
				
}  ;

/*  Overload VTables  */


/* -------------------------------
 *
 * Internal memory constructor
 *
 --------------------------------- */
 
 
PortCLASS Port_Constructor(PortCLASS this_ptr)
{

	if(this_ptr==NULL)
	{
		this_ptr = (PortCLASS) malloc(sizeof(struct Port));
	}

	if(this_ptr!=NULL)
	{
	
			
		/*  Port init  */
		this_ptr->_Port_id = CLASS_ID_Port_ID;
		this_ptr->_Port_refs = 0;
		this_ptr->_Port_pVTable=VTableArrayForPort;

				this_ptr->m_Port_numFields= _Z17fieldInitializer9EV(this_ptr);
						}

	return this_ptr;
}

/*  Method for creating new "class"  */
static TVP new()
{
	PortCLASS ptr=Port_Constructor(NULL);

	return newTypeValue(VDM_CLASS, (TypedValueType)
			{	.ptr=newClassValue(ptr->_Port_id, &ptr->_Port_refs, (freeVdmClassFunction)&Port_free, ptr)});
}



/* -------------------------------
 *
 * Public class constructors
 *
 --------------------------------- */ 
 

    /* Fmi.vdmrt 20:7 */
 TVP _Z4PortEV(PortCLASS this)	{

TVP __buf = NULL
;

if ( this == NULL )
	
	{

__buf = new();

this = TO_CLASS_PTR(__buf, Port);
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
		TVP numFields_4 =  NULL ;
	
