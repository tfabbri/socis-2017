/* This file was generated automatically: DO NOT MODIFY IT ! */

/* Declaration of the functions that have to be provided by the user */

#ifndef __USER_CODE_H_fmi_interface__
#define __USER_CODE_H_fmi_interface__

#include "C_ASN1_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

void fmi_interface_startup();

void fmi_interface_PI_Cyclic_Call();

extern void fmi_interface_RI_Controller_Function(const asn1SccFmiInteger *,
                                                 const asn1SccFmiBoolean *,
                                                 const asn1SccFmiReal *,
                                                 asn1SccFmiInteger *,
                                                 asn1SccFmiBoolean *,
                                                 asn1SccFmiReal *);

#ifdef __cplusplus
}
#endif


#endif
