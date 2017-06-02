#ifndef __DATAIMPORT_H__
#	define __DATAIMPORT_H__

#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>
#	include "DataModel.h"

int parseSysParam(const char sysParamPath[], \
		  SYSPARAM **sysparam_list);

int parseUserCall(const char userCallPath[], \
		  USERCALL **usercall_list);

#endif
