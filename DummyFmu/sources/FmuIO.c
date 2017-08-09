/*
 * FmuIoc.c
 *
 *  Created on: Mar 4, 2016
 *      Author: kel
 */

/*
 * FmuIO.cpp
 *
 *  Created on: Mar 4, 2016
 *      Author: kel
 */
#include "Vdm.h"

#include "VdmModelFeatures.h"

#ifdef CUSTOM_IO

#ifndef NO_IO

#include "Fmu.h"
#include "PrettyPrint.h"


TVP vdm_IO_freadval(TVP filename)
{
	/*  TODO.  */
	return NULL;
}



TVP vdm_IO_fwriteval(TVP filename, TVP val, TVP fdir)
{
	/*  TODO.  */
	return NULL;
}



TVP vdm_IO_writeval(TVP val)
{
	/*  TODO.  */
	return NULL;
}



void vdm_IO_printf(TVP format, TVP args)
{
	/*  TODO.  */
	return;
}

void vdm_IO_println(TVP arg)
{
	char* str;
	int i;
	int isstring;

	/*  TODO:  A quick hack to print a sequence of chars as a legible string.  */
	if(arg->type == VDM_SEQ)
	{
		isstring = 1;
		UNWRAP_COLLECTION(col, arg);
		for(i = 0; i < col->size; i++)
		{
			if(col->value[i]->type != VDM_CHAR)
			{
				isstring = 0;
				break;
			}
		}

		if(isstring)
		{
			str = unpackString(arg);
		}
		else
		{
			str = toString(arg);
		}
	}
	else
	{
		str = toString(arg);
	}
	/*  comp, instancename, status, category  */
	/*  fmi2ComponentEnvironment, fmi2String, fmi2Status, fmi2String, fmi2String, ...  */
	if(g_fmiCallbackFunctions!=NULL)
	{
		g_fmiCallbackFunctions->logger((void*) 1,g_fmiInstanceName,fmi2OK,"logAll","%s\n", str);
	}
	else
	{
		printf("%s\n", str);
	}

	free(str);

	return;
}


void vdm_IO_print(TVP arg)
{
	char* str;

	int i;
	int isstring;

	/*  TODO:  A quick hack to print a sequence of chars as a legible string.  */
	if(arg->type == VDM_SEQ)
	{
		isstring = 1;
		UNWRAP_COLLECTION(col, arg);
		for(i = 0; i < col->size; i++)
		{
			if(col->value[i]->type != VDM_CHAR)
			{
				isstring = 0;
				break;
			}
		}

		if(isstring)
		{
			str = unpackString(arg);
		}
		else
		{
			str = toString(arg);
		}
	}
	else
	{
		str = toString(arg);
	}

	if(g_fmiCallbackFunctions!=NULL)
	{
		g_fmiCallbackFunctions->logger((void*) 1,g_fmiInstanceName,fmi2OK,"logAll","%s", str);
	}
	else
	{
		printf("%s\n", str);
	}
	free(str);

	return;
}


TVP vdm_IO_ferror()
{
	/*  TODO.  */

	return NULL;
}

TVP vdm_IO_fecho(TVP filename, TVP text, TVP fdir)
{
	/*  TODO.  */

	return NULL;
}
TVP vdm_IO_echo(TVP text)
{
	int a;

	/*  TODO:  Ensure a string is passed.  */
	char *str = unpackString(text);

	if(g_fmiCallbackFunctions!=NULL)
	{
		/*  g_fmiCallbackFunctions->logger((void*) 1,g_fmiInstanceName,fmi2OK,"logAll","%s", str);  */
	}
	else
	{
		printf("%s", str);
	}

	free(str);

	return NULL;
}


#endif /*NO_IO*/

#endif /*CUSTOM_IO*/
