#ifndef __ELEVATORSIMULATION_H__
#	define __ELEVATORSIMULATION_H__

#	include "DataModel.h"
#	include "DataExport.h"
#	include "DataImport.h"
#	include "ScreenSimulation.h"

#	ifdef _WIN32
#		include <io.h>
#	else
#		include <unistd.h>
#	endif

int SystemInit(void);
int PrintMainMenu(void);
void ShowMenu(int menu_flag);
void InitSimulation(void);
void InitElevator(ELEVATORSTATE *elevator);
void SilenceSimulate(const char filename[]);
void HistorySimulate(const char filename[]);

#endif
