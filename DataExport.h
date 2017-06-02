#ifndef __DATAEXPORT_H__
#	define __DATAEXPORT_H__

#	include "DataModel.h"
#	include "DataImport.h"

#	ifdef _WIN32
#		include <io.h>
#	else
#		include <unistd.h>
#	endif

void ParamConf(int choice);

#endif
