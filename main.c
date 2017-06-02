#include "DataModel.h"
#include "DataImport.h"
#include "ElevatorSimulation.h"

SYSPARAM *sysparam_list = NULL;
int sysparam_num;
USERCALL *usercall_list = NULL;
int usercall_num;
ELEVATORSTATE elevator_a;
ELEVATORSTATE elevator_b;
int finish_call_num;
int time;
int status_change_flag;
int main(int argc, char *argv[])
{
	switch (SystemInit()) {
	case 0:
		fprintf(stderr, "Cannot read SysConf dir!\n");
		exit(EXIT_FAILURE);
		break;
	case -1:
		fprintf(stderr, "Cannot read SysParam.txt in the SysConf dir!\n");
		exit(EXIT_FAILURE);
		break;
	case -2:
		fprintf(stderr, "Cannot open SysParam.txt!\n");
		exit(EXIT_FAILURE);
		break;
	case -3:
		fprintf(stderr, "Cannot find custom order dir!\n");
		exit(EXIT_FAILURE);
		break;
	case -4:
		fprintf(stderr, "Cannot find custom history dir!\n");
		exit(EXIT_FAILURE);
		break;
	case -5:
		fprintf(stderr, "Elevator floor parameter error!\n");
		exit(EXIT_FAILURE);
		break;
	case -6:
		fprintf(stderr, "Elevator animate delay parameter error!\n");
		exit(EXIT_FAILURE);
		break;
	case -7:
		fprintf(stderr, "Illegal parameter number!\n");
		exit(EXIT_FAILURE);
		break;
	case -8:
		fprintf(stderr, "No avalible parameter found!\n");
		exit(EXIT_FAILURE);
		break;
	default:
		break;
	}
	int choice;
	while (choice = PrintMainMenu())
		ShowMenu(choice);
	return 0;
}
