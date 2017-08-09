/**
* This file is generated by Overture's VDM-to-C code generator version 0.1.9-SNAPSHOT.
* Website: https://github.com/overturetool/vdm2c
*/

#include "World.h"
#include <stdio.h>
#include <string.h>


/* -------------------------------
 *
 * Memory management methods
 *
 --------------------------------- */

void World_free_fields(struct World *this)
{
		vdmFree(this->m_World_numFields);
		}

static void World_free(struct World *this)
{
	--this->_World_refs;
	if (this->_World_refs < 1)
	{
		World_free_fields(this);
		free(this);
	}
}


/* -------------------------------
 *
 * Member methods 
 *
 --------------------------------- */
 

 static  TVP _Z17fieldInitializer8EV()	{

TVP ret_7 = newInt(0)
;

return ret_7;
}



 static  TVP _Z17fieldInitializer7EV(WorldCLASS this)	{

TVP ret_8 = newInt(0)
;

return ret_8;
}


/* World.vdmrt 4:9 */
 static  void _Z3runEV(WorldCLASS this)	{
/* World.vdmrt 7:3 */
;
/* World.vdmrt 10:3 */
CALL_FUNC_PTR(World, World, this, CLASS_World__Z5blockEV);
}


/* World.vdmrt 13:2 */
 static  void _Z5blockEV(WorldCLASS this)	{
/* World.vdmrt 14:14 */
{
/*  Skip  */
}
;
}



 void World_const_init()	{

numFields_3 = _Z17fieldInitializer8EV();

return ;
}



 void World_const_shutdown()	{

vdmFree(numFields_3);

return ;
}



 void World_static_init()	{

return ;
}



 void World_static_shutdown()	{

return ;
}




/* -------------------------------
 *
 * VTable
 *
 --------------------------------- */
 
/*  VTable for this class  */
 static  struct VTable VTableArrayForWorld  [] ={

{0,0,((VirtualFunctionPointer) _Z3runEV),},
{0,0,((VirtualFunctionPointer) _Z5blockEV),},
{0,0,((VirtualFunctionPointer) _Z5WorldEV),},
				
}  ;

/*  Overload VTables  */


/* -------------------------------
 *
 * Internal memory constructor
 *
 --------------------------------- */
 
 
WorldCLASS World_Constructor(WorldCLASS this_ptr)
{

	if(this_ptr==NULL)
	{
		this_ptr = (WorldCLASS) malloc(sizeof(struct World));
	}

	if(this_ptr!=NULL)
	{
	
			
		/*  World init  */
		this_ptr->_World_id = CLASS_ID_World_ID;
		this_ptr->_World_refs = 0;
		this_ptr->_World_pVTable=VTableArrayForWorld;

				this_ptr->m_World_numFields= _Z17fieldInitializer7EV(this_ptr);
						}

	return this_ptr;
}

/*  Method for creating new "class"  */
static TVP new()
{
	WorldCLASS ptr=World_Constructor(NULL);

	return newTypeValue(VDM_CLASS, (TypedValueType)
			{	.ptr=newClassValue(ptr->_World_id, &ptr->_World_refs, (freeVdmClassFunction)&World_free, ptr)});
}



/* -------------------------------
 *
 * Public class constructors
 *
 --------------------------------- */ 
 

    /* World.vdmrt 1:7 */
 TVP _Z5WorldEV(WorldCLASS this)	{

TVP __buf = NULL
;

if ( this == NULL )
	
	{

__buf = new();

this = TO_CLASS_PTR(__buf, World);
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
		TVP numFields_3 =  NULL ;
	
