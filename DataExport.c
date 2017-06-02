#include "DataExport.h"

void ParamConf(int choice)
{
	sysparam_num = parseSysParam(SYSCONF OS_SEP SYSPARAM_TXT, &sysparam_list);
	switch (choice) {
	case 1:
		for (int i = 0; i < sysparam_num; i++) {
			if (strcmp(sysparam_list[i].param_name, "UserRequestFilePath") == 0) {
				printf("输入新的用户请求文件目录：");
				scanf("%s", sysparam_list[i].param_value);
			}
		}
		break;
	case 2:
		for (int i = 0; i < sysparam_num; i++) {
			if (strcmp(sysparam_list[i].param_name, "SimulationFilesPath") == 0) {
				printf("输入新的仿真输出文件目录：");
				scanf("%s", sysparam_list[i].param_value);
			}
		}
		break;
	case 3:
		for (int i = 0; i < sysparam_num; i++) {
			if (strcmp(sysparam_list[i].param_name, "ElevatorHeight") == 0) {
				printf("输入新的楼层数：");
				scanf("%s", sysparam_list[i].param_value);
			}
		}
		break;
	case 4:
		for (int i = 0; i < sysparam_num; i++) {
			if (strcmp(sysparam_list[i].param_name, "SimulateSpeed") == 0) {
				printf("输入新的动画延迟时间：");
				scanf("%s", sysparam_list[i].param_value);
			}
		}
		break;
	default:
		break;
	}
	FILE *fp = fopen(SYSCONF OS_SEP SYSPARAM_TXT, "w");
	fprintf(fp, "%d\n", sysparam_num);
	for (int i = 0; i < sysparam_num; i++) {
		fprintf(fp, "%s=%s\n", sysparam_list[i].param_name, sysparam_list[i].param_value);
	}
	fclose(fp);
}
