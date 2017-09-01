/* User code: This file will not be overwritten by TASTE. */

#include "controller.h"

void controller_startup()
{
    /* Write your initialization code here,
       but do not make any call to a required interface. */
}

void controller_PI_Controller_Function(const asn1SccFmiInteger *IN_mip,
                                       const asn1SccFmiBoolean * IN_mbp,
                                       const asn1SccFmiReal *IN_mrp,
                                       asn1SccFmiInteger *OUT_cip,
                                       asn1SccFmiBoolean * OUT_cbp, 
                                       asn1SccFmiReal *OUT_crp)
{
    /* Write your code here! */
    *OUT_cip = *IN_mip;
    *OUT_crp = *IN_mrp;
    *OUT_cbp = *IN_mbp;
}

