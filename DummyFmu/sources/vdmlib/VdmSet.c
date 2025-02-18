/*
 * #%~
 * The VDM to C Code Generator
 * %%
 * Copyright (C) 2015 - 2016 Overture
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program.  If not, see
 * <http:XXXwww.gnu.org/licenses/gpl-3.0.html>.
 * #~%
 */

/*
 * VdmSet.c
 *
 *  Created on: Dec 7, 2015
 *      Author: kel
 */
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <assert.h>
#include "VdmSet.h"

#ifndef NO_SETS

#define ASSERT_CHECK(s) assert(s->type == VDM_SET && "Value is not a set")
#define DEFAULT_SET_COMP_BUFFER 2
#define DEFAULT_SET_COMP_BUFFER_STEPSIZE 10



/* Utility functions.  */
/* ------------------------------------------------  */
static void vdmSetAdd(TVP* value, int* index, TVP newValue)
{
	int i;
	bool found = false;

	for (i = 0; i < *index; i++)
	{
		found |= equals(value[i],newValue);
	}

	if(!found)
	{
		value[*index] = newValue;
		*index = (*index) + 1;
	}
	/* This value is a duplicate, so must not leave it lying around.  */
	else
		vdmFree(newValue);
}
/* End utility functions  */
/* ------------------------------------------------  */



TVP newSetWithValues(size_t size, TVP* elements)
{
	int i;
	int count = 0;
	int bufsize = DEFAULT_SET_COMP_BUFFER;
	TVP* value = (TVP*)calloc(bufsize, sizeof(TVP));
	assert(value != NULL);

	for (i = 0; i < size; i++)
	{
		TVP v = vdmClone(elements[i]); /*  set binding  */

		if(count >= bufsize)
		{
			/* buffer too small add memory chunk  */
			bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;
			value = (TVP*)realloc(value, bufsize * sizeof(TVP));
			assert(value != NULL);
		}
		vdmSetAdd(value, &count,v);
	}

	TVP res = newCollectionWithValues(count, VDM_SET, value);

	for(i = 0; i < count; i++)
	{
		vdmFree(value[i]);
	}
	free(value);
	return res;
}


TVP newSetWithValuesGC(size_t size, TVP* elements, TVP *from)
{
	int i;
	int count = 0;
	int bufsize = DEFAULT_SET_COMP_BUFFER;
	TVP* value = (TVP*)calloc(bufsize, sizeof(TVP));
	assert(value != NULL);

	for (i = 0; i < size; i++)
	{
		TVP v = vdmClone(elements[i]); /*  set binding  */

		if(count >= bufsize)
		{
			/* buffer too small add memory chunk  */
			bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;
			value = (TVP*)realloc(value, bufsize * sizeof(TVP));
			assert(value != NULL);
		}
		vdmSetAdd(value, &count,v);
	}

	TVP res = newCollectionWithValuesGC(count, VDM_SET, value, from);

	for(i = 0; i < count; i++)
	{
		vdmFree(value[i]);
	}
	free(value);
	return res;
}



TVP newSetVar(size_t size, ...)
{
	int i;

	va_list ap;
	va_start(ap, size);

	int count = 0;

	int bufsize = DEFAULT_SET_COMP_BUFFER;
	TVP* value = (TVP*) calloc(bufsize, sizeof(TVP));
	assert(value != NULL);

	for (i = 0; i < size; i++)
	{
		TVP arg = va_arg(ap, TVP);

		TVP v= vdmClone(arg);

		/* TODO:  Check whether element exists.  */


		if(count>=bufsize)
		{
			/* buffer too small add memory chunk  */
			bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;
			value = (TVP*)realloc(value, bufsize * sizeof(TVP));
			assert(value != NULL);
		}
		vdmSetAdd(value, &count, v);
	}

	va_end(ap);

	TVP res = newCollectionWithValues(count, VDM_SET, value);
	for(i = 0; i < count; i++)
		vdmFree(value[i]);
	free(value);
	return res;
}


TVP newSetVarGC(size_t size, TVP *from, ...)
{
	int i;

	va_list ap;
	va_start(ap, from);

	int count = 0;

	int bufsize = DEFAULT_SET_COMP_BUFFER;
	TVP* value = (TVP*) calloc(bufsize, sizeof(TVP));
	assert(value != NULL);

	for (i = 0; i < size; i++)
	{
		TVP arg = va_arg(ap, TVP);
		TVP v = vdmClone(arg);

		if(count>=bufsize)
		{
			/* buffer too small add memory chunk  */
			bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;
			value = (TVP*)realloc(value, bufsize * sizeof(TVP));
			assert(value != NULL);
		}
		vdmSetAdd(value,&count,v);
	}

	va_end(ap);

	TVP res = newCollectionWithValuesGC(count, VDM_SET, value, from);
	for(i = 0; i < count; i++)
	{
		vdmFree(value[i]);
	}
	free(value);
	return res;
}




/* Just like newSetVar, but with memory preallocated to an expected  */
/* result set size.  */
TVP newSetVarToGrow(size_t size, size_t expected_size, ...)
{
	int i;

	va_list ap;
	va_start(ap, expected_size);

	int count = 0;

	int bufsize = expected_size;  /* DEFAULT_SET_COMP_BUFFER;  */
	TVP* value = (TVP*) calloc(bufsize, sizeof(TVP));
	assert(value != NULL);

	TVP arg;
	TVP v;

	for (i = 0; i < size; i++)
	{
		arg = va_arg(ap, TVP);
		v = vdmClone(arg); /*  set binding  */


		/* Extra security measure.  Will only be true if size >= expected_size.  */
		if(count>=bufsize)
		{
			/* buffer too small add memory chunk  */
			bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;
			value = (TVP*)realloc(value, bufsize * sizeof(TVP));
			assert(value != NULL);
		}
		vdmSetAdd(value,&count,v);
	}

	va_end(ap);

	TVP res = newCollectionWithValues(count, VDM_SET, value);
	free(value);
	return res;
}

TVP newSetVarToGrowGC(size_t size, size_t expected_size, TVP *from, ...)
{
	va_list ap;
	va_start(ap, from);

	int i;
	int count = 0;

	int bufsize = expected_size;  /* DEFAULT_SET_COMP_BUFFER;  */
	TVP* value = (TVP*) calloc(bufsize, sizeof(TVP));
	assert(value != NULL);

	TVP arg;
	TVP v;

	for (i = 0; i < size; i++)
	{
		arg = va_arg(ap, TVP);
		v = vdmClone(arg); /*  set binding  */


		/* Extra security measure.  Will only be true if size >= expected_size.  */
		if(count>=bufsize)
		{
			/* buffer too small add memory chunk  */
			bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;
			value = (TVP*)realloc(value, bufsize * sizeof(TVP));
			assert(value != NULL);
		}
		vdmSetAdd(value,&count,v);
	}

	va_end(ap);

	TVP res = newCollectionWithValuesGC(count, VDM_SET, value, from);
	free(value);
	return res;
}



/* What to return?  */
void vdmSetGrow(TVP set, TVP element)
{
/* 	int bufsize = DEFAULT_SET_COMP_BUFFER;  */

	UNWRAP_COLLECTION(col, set);
	int size = col->size;

/* 	if(col->size >= bufsize)  */
/* 	{  */
/* 		buffer too small add memory chunk  */
/* 		bufsize += DEFAULT_SET_COMP_BUFFER_STEPSIZE;  */
/* 		col->value = (TVP*)realloc(col->value, bufsize * sizeof(TVP));  */
/* 	}  */

	col->value = (TVP *)realloc(col->value, (size + 1) * sizeof(TVP));
	assert(col->value != NULL);

	vdmSetAdd(col->value, &(col->size), vdmClone(element));
}



void vdmSetFit(TVP set)
{
	UNWRAP_COLLECTION(col, set);

	/* Assumes that more memory is allocated in the col->value array than there are elements.  */
	col->value = (TVP*)realloc(col->value, col->size * sizeof(TVP));
	assert(col->value != NULL);
}



TVP vdmSetEnumerateSetOfInts(int lower, int upper)
{
	TVP* theset;
	int count;
	int i;

	/* Wrong parameter types.  */
	/* 	if(lower->type != VDM_INT || upper->type != VDM_INT)  */
	/* 	{  */
	/* 		return NULL;  */
	/* 	}  */

	/* For faster access.  */
	/* 	l = lower->value.intVal;  */
	/* 	u = upper->value.intVal;  */

	/* Some special cases.  */
	if (upper < lower)
	{
		return NULL;
	}

	if(lower == upper)
	{
		return newSetVar(1, newInt(upper));
	}

	/* The common case.  */
	theset = (TVP*)calloc(upper - lower + 1, sizeof(TVP));
	assert(theset != NULL);
	count = 0;

	for (i = lower; i <= upper; i++)
	{
		vdmSetAdd(theset, &count, newInt(i));
	}

	TVP res = newCollectionWithValues(count, VDM_SET, theset);

	for(i = 0; i < count; i++)
	{
		vdmFree(theset[i]);
	}
	free(theset);
	return res;
}


TVP vdmSetElementAt(TVP set, int loc)
{
	UNWRAP_COLLECTION(col, set);

	if(loc >= col->size)
	{
		return NULL;
	}

	return vdmClone(col->value[loc]);
}

TVP vdmSetElementAtGC(TVP set, int loc, TVP *from)
{
	UNWRAP_COLLECTION(col, set);

	if(loc >= col->size)
	{
		return NULL;
	}

	return vdmCloneGC(col->value[loc], from);
}



TVP vdmSetMemberOf(TVP set, TVP element)
{
	int i;

	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col,set);

	bool found = false;

	for (i = 0; i < col->size; i++)
	{
		found|= equals(col->value[i],element);
	}

	return newBool(found);
}


TVP vdmSetMemberOfGC(TVP set, TVP element, TVP *from)
{
	int i;

	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col,set);

	bool found = false;

	for (i = 0; i < col->size; i++)
	{
		found|= equals(col->value[i],element);
	}

	return newBoolGC(found, from);
}



TVP vdmSetNotMemberOf(TVP set, TVP element)
{
	TVP res;
	bool resval;

	ASSERT_CHECK(set);

	res = vdmSetMemberOf(set, element);
	resval = res->value.boolVal;

	vdmFree(res);

	return newBool(!resval);
}

TVP vdmSetNotMemberOfGC(TVP set, TVP element, TVP *from)
{
	TVP res;
	bool resval;

	ASSERT_CHECK(set);

	res = vdmSetMemberOf(set, element);
	resval = res->value.boolVal;

	vdmFree(res);

	return newBoolGC(!resval, from);
}



TVP vdmSetUnion(TVP set1, TVP set2)
{
	TVP *newvalues;
	TVP resset;
	int i;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);
	/* col1 and col2 of type struct Collection*.  */

	/* This can not be done because col1 and col2 are not valid at the time when  */
	/* newcol1 and newcol2 are declared, hence the bogus length of one of them.  */
	/* TVP newcol1[col1->size];  */
	/* TVP newcol2[col2->size];  */
	/* Something like the following is fine because memory layout is not involved.  */
	/* int a = col1->size;  */

	/* newcol1 = (TVP*)malloc(col1->size * sizeof(TVP));  */
	/* newcol2 = (TVP*)malloc(col2->size * sizeof(TVP));  */

	newvalues = (TVP*)malloc((col1->size + col2->size) * sizeof(TVP));
	assert(newvalues != NULL);
	for(i = 0; i < col1->size; i++)
	{
		newvalues[i] = vdmClone((col1->value)[i]);
	}

	for(i = col1->size; i < (col1-> size + col2->size); i++)
	{
		newvalues[i] = vdmClone((col2->value)[i - col1->size]);
	}

	resset = newSetWithValues(col1->size + col2->size, newvalues);

	for(i = 0; i < col1->size + col2->size; i++)
		vdmFree(newvalues[i]);
	free(newvalues);

	return resset;
}

TVP vdmSetUnionGC(TVP set1, TVP set2, TVP *from)
{
	int i;

	TVP *newvalues;
	TVP resset;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);
	/* col1 and col2 of type struct Collection*.  */

	/* This can not be done because col1 and col2 are not valid at the time when  */
	/* newcol1 and newcol2 are declared, hence the bogus length of one of them.  */
	/* TVP newcol1[col1->size];  */
	/* TVP newcol2[col2->size];  */
	/* Something like the following is fine because memory layout is not involved.  */
	/* int a = col1->size;  */

	/* newcol1 = (TVP*)malloc(col1->size * sizeof(TVP));  */
	/* newcol2 = (TVP*)malloc(col2->size * sizeof(TVP));  */

	newvalues = (TVP*)malloc((col1->size + col2->size) * sizeof(TVP));
	assert(newvalues != NULL);
	for(i = 0; i < col1->size; i++)
	{
		newvalues[i] = vdmClone((col1->value)[i]);
	}

	for(i = col1->size; i < (col1-> size + col2->size); i++)
	{
		newvalues[i] = vdmClone((col2->value)[i - col1->size]);
	}

	resset = newSetWithValuesGC(col1->size + col2->size, newvalues, from);

	for(i = 0; i < col1->size + col2->size; i++)
		vdmFree(newvalues[i]);
	free(newvalues);

	return resset;
}



TVP vdmSetInter(TVP set1, TVP set2)
{
	int i;
	TVP inter;
	TVP tmpset1;
	TVP tmpset2;
	TVP res;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1->size == 0 || col2->size ==0)
	{
		return newSetWithValues(0, NULL);
	}

	inter = newSetWithValues(0, NULL);

	for(i = 0; i < col1->size; i++)
	{
		res = vdmSetMemberOf(set2, (col1->value)[i]);

		if(res->value.boolVal)
		{
			/* add to intersection set  */
			tmpset1 = newSetVar(1, col1->value[i]);
			tmpset2 = vdmSetUnion(inter, tmpset1);
			vdmFree(inter);
			inter = tmpset2;
			vdmFree(tmpset1);
		}
		vdmFree(res);
	}

	return inter;
}


TVP vdmSetInterGC(TVP set1, TVP set2, TVP *from)
{
	int i;
	TVP inter;
	TVP tmpset1;
	TVP tmpset2;
	TVP res;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1->size == 0 || col2->size ==0)
	{
		return newSetWithValuesGC(0, NULL, from);
	}

	inter = newSetWithValuesGC(0, NULL, from);

	for(i = 0; i < col1->size; i++)
	{
		res = vdmSetMemberOf(set2, (col1->value)[i]);

		if(res->value.boolVal)
		{
			/* add to intersection set  */
			tmpset1 = newSetVar(1, col1->value[i]);
			tmpset2 = vdmSetUnion(inter, tmpset1);
			vdmFree(inter);
			inter = tmpset2;
			vdmFree(tmpset1);
		}
		vdmFree(res);
	}

	return inter;
}




TVP vdmSetDifference(TVP set1, TVP set2)
{
	int i;
	TVP tmpset1;
	TVP tmpset2;
	TVP resultset;
	TVP res;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1->size == 0 || col2->size == 0)
	{
		return set1;
	}

	resultset = newSetWithValues(0, NULL);

	for(i = 0; i < col1->size; i++)
	{
		res = vdmSetNotMemberOf(set2, (col1->value)[i]);
		if(res->value.boolVal)
		{
			tmpset1 = newSetVar(1, (col1->value)[i]);
			tmpset2 = vdmSetUnion(resultset, tmpset1);
			vdmFree(resultset);
			resultset = tmpset2;
			vdmFree(tmpset1);
		}
		vdmFree(res);
	}

	return resultset;
}



TVP vdmSetDifferenceGC(TVP set1, TVP set2, TVP *from)
{
	int i;
	TVP tmpset1;
	TVP tmpset2;
	TVP resultset;
	TVP res;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1->size == 0 || col2->size == 0)
	{
		return set1;
	}

	resultset = newSetWithValuesGC(0, NULL, from);

	for(i = 0; i < col1->size; i++)
	{
		res = vdmSetNotMemberOf(set2, (col1->value)[i]);
		if(res->value.boolVal)
		{
			tmpset1 = newSetVar(1, (col1->value)[i]);
			tmpset2 = vdmSetUnion(resultset, tmpset1);
			vdmFree(resultset);
			resultset = tmpset2;
			vdmFree(tmpset1);
		}
		vdmFree(res);
	}

	return resultset;
}


TVP vdmSetSubset(TVP set1, TVP set2)
{
	int i;
	TVP res;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1-> size > col2->size)
	{
		return newBool(false);
	}

	for(i = 0; i < col1->size; i++)
	{

		res = vdmSetMemberOf(set2, (col1->value)[i]);
		if(!res->value.boolVal)
		{
			free(res);
			return newBool(false);
		}
		free(res);
	}

	return newBool(true);
}



TVP vdmSetSubsetGC(TVP set1, TVP set2, TVP *from)
{
	int i;
	TVP res;

	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1-> size > col2->size)
	{
		return newBoolGC(false, from);
	}

	for(i = 0; i < col1->size; i++)
	{

		res = vdmSetMemberOf(set2, (col1->value)[i]);
		if(!res->value.boolVal)
		{
			free(res);
			return newBoolGC(false, from);
		}
		free(res);
	}

	return newBoolGC(true, from);
}



TVP vdmSetProperSubset(TVP set1, TVP set2)
{
	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1->size >= col2->size)
	{
		return newBool(false);
	}

	return vdmSetSubset(set1, set2);
}


TVP vdmSetProperSubsetGC(TVP set1, TVP set2, TVP *from)
{
	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	UNWRAP_COLLECTION(col1, set1);
	UNWRAP_COLLECTION(col2, set2);

	if(col1->size >= col2->size)
	{
		return newBoolGC(false, from);
	}

	return vdmSetSubsetGC(set1, set2, from);
}



TVP vdmSetEquals(TVP set1, TVP set2)
{
	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	TVP subset12Res;
	TVP subset21Res;
	bool subsetRes;

	/* Check mutual inclusion.  */
	subset12Res = vdmSetSubset(set1, set2);
	subset21Res = vdmSetSubset(set2, set1);

	subsetRes = subset12Res->value.boolVal && subset21Res->value.boolVal;
	vdmFree(subset12Res);
	vdmFree(subset21Res);

	return newBool(subsetRes);
}


TVP vdmSetNotEquals(TVP set1, TVP set2)
{
	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	return vdmInEquals(set1, set2);
}


TVP vdmSetNotEqualsGC(TVP set1, TVP set2, TVP *from)
{
	ASSERT_CHECK(set1);
	ASSERT_CHECK(set2);

	return vdmInEqualsGC(set1, set2, from);
}



TVP vdmSetCard(TVP set)
{
	ASSERT_CHECK(set);
	UNWRAP_COLLECTION(col, set);

	return newInt(col->size);
}


TVP vdmSetCardGC(TVP set, TVP *from)
{
	ASSERT_CHECK(set);
	UNWRAP_COLLECTION(col, set);

	return newIntGC(col->size, from);
}



TVP vdmSetDunion(TVP set)
{
	int i;
	TVP unionset;
	TVP set1;

	/* Preliminary checks.  */
	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col, set);
	for(i = 0; i < col->size; i++)
	{
		ASSERT_CHECK((col->value)[i]);
	}

	/* Initialize final set.  */
	unionset = newSetVar(0, NULL);

	/* Build union set.  */
	for(i = 0; i < col->size; i++)
	{
		set1 = vdmSetUnion(unionset, (col->value)[i]);
		vdmFree(unionset);
		unionset = set1;
	}

	return unionset;
}


TVP vdmSetDunionGC(TVP set, TVP *from)
{
	int i;
	TVP unionset;
	TVP set1;

	/* Preliminary checks.  */
	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col, set);
	for(i = 0; i < col->size; i++)
	{
		ASSERT_CHECK((col->value)[i]);
	}

	/* Initialize final set.  */
	unionset = newSetVarGC(0, NULL, from);

	/* Build union set.  */
	for(i = 0; i < col->size; i++)
	{
		set1 = vdmSetUnion(unionset, (col->value)[i]);
		vdmFree(unionset);
		unionset = set1;
	}

	return unionset;
}



TVP vdmSetDinter(TVP set)
{
	int i;
	TVP interset;
	TVP set1;

	/* Preliminary checks.  */
	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col, set);
	for(i = 0; i < col->size; i++)
	{
		ASSERT_CHECK((col->value)[i]);
	}

	/* Initialize final set.  */
	interset = vdmClone((col->value)[0]);

	/* Build intersection set.  */
	for(i = 1; i < col->size; i++)
	{
		set1 = vdmSetInter(interset, (col->value)[i]);
		vdmFree(interset);
		interset = set1;
	}

	return interset;
}


TVP vdmSetDinterGC(TVP set, TVP *from)
{
	int i;
	TVP interset;
	TVP set1;

	/* Preliminary checks.  */
	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col, set);
	for(i = 0; i < col->size; i++)
	{
		ASSERT_CHECK((col->value)[i]);
	}

	/* Initialize final set.  */
	interset = vdmClone((col->value)[0]);

	/* Build intersection set.  */
	for(i = 1; i < col->size; i++)
	{
		set1 = vdmSetInterGC(interset, (col->value)[i], from);
		vdmFree(interset);
		interset = set1;
	}

	return interset;
}



TVP vdmSetPower(TVP set)
{
	int i, j;
	TVP set1;
	TVP set2;
	TVP set3;
	TVP powerset;
	struct Collection *powercol;
	int powercolsize;

	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col, set);

	/* Initialize powerset to contain the empty set.  */
	powerset = newSetVar(0, NULL);

	set1 = newSetVar(0, NULL);
	set2 = newSetVar(1, set1);
	vdmFree(set1);
	set1 = vdmSetUnion(powerset, set2);
	vdmFree(set2);
	vdmFree(powerset);
	powerset = set1;

	for(i = 0; i < col->size; i++)
	{
		powercolsize = ((struct Collection*)powerset->value.ptr)->size;
		for(j = 0; j < powercolsize; j++)
		{
			powercol = (struct Collection*)powerset->value.ptr;

			set1 = newSetVar(1, (col->value)[i]);
			set2 = vdmSetUnion((powercol->value)[j], set1);
			vdmFree(set1);
			set1 = newSetVar(1, set2);
			vdmFree(set2);
			set3 = vdmSetUnion(powerset, set1);
			vdmFree(set1);
			vdmFree(powerset);
			powerset = set3;
		}
	}

	return powerset;
}


TVP vdmSetPowerGC(TVP set, TVP *from)
{
	int i, j;
	TVP set1;
	TVP set2;
	TVP set3;
	TVP powerset;
	struct Collection *powercol;
	int powercolsize;

	ASSERT_CHECK(set);

	UNWRAP_COLLECTION(col, set);

	/* Initialize powerset to contain the empty set.  */
	powerset = newSetVar(0, NULL);

	set1 = newSetVar(0, NULL);
	set2 = newSetVar(1, set1);
	vdmFree(set1);
	set1 = vdmSetUnion(powerset, set2);
	vdmFree(set2);
	vdmFree(powerset);
	powerset = set1;

	for(i = 0; i < col->size; i++)
	{
		powercolsize = ((struct Collection*)powerset->value.ptr)->size;
		for(j = 0; j < powercolsize; j++)
		{
			powercol = (struct Collection*)powerset->value.ptr;

			set1 = newSetVar(1, (col->value)[i]);
			set2 = vdmSetUnion((powercol->value)[j], set1);
			vdmFree(set1);
			set1 = newSetVar(1, set2);
			vdmFree(set2);
			set3 = vdmSetUnionGC(powerset, set1, from);
			vdmFree(set1);
			vdmFree(powerset);
			powerset = set3;
		}
	}

	return powerset;
}

#endif /* NO_SETS */
