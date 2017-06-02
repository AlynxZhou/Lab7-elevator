#include "ElevatorSimulation.h"

int SystemInit(void)
{
#ifdef _WIN32
	if (_access(SYSCONF, 04))
		return 0;
	if (_access(SYSCONF OS_SEP SYSPARAM_TXT, 04))
		return -1;
#else
	if (access(SYSCONF, R_OK))
		return 0;
	if (access(SYSCONF OS_SEP SYSPARAM_TXT, R_OK))
		return -1;
#endif
	if (fopen(SYSCONF OS_SEP SYSPARAM_TXT, "r") == NULL)
		return -2;
	sysparam_num = parseSysParam(SYSCONF OS_SEP SYSPARAM_TXT, &sysparam_list);
	switch (sysparam_num) {
	case -1:
		return -2;
	case -2:
		return -8;
	case -3:
		return -7;
	}
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name,  "UserRequestFilePath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
#ifdef _WIN32
				if (_access(sysparam_list[i].param_value, 04))
					return -3;
#else
				if (access(sysparam_list[i].param_value, R_OK)) {
					return -3;
				}
#endif
			}
		} else if (strcmp(sysparam_list[i].param_name,  "SimulationFilesPath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
#ifdef _WIN32
				if (_access(sysparam_list[i].param_value, 04))
					return -4;
#else
				if (access(sysparam_list[i].param_value, R_OK))
					return -4;
#endif
			}
		} else if (strcmp(sysparam_list[i].param_name,  "ElevatorHeight") == 0) {
			int height = 0;
			sscanf(sysparam_list[i].param_value, "%d", &height);
			if (height < 2)
				return -5;
		} else if (strcmp(sysparam_list[i].param_name,  "SimulateSpeed") == 0) {
			int delay = 0;
			sscanf(sysparam_list[i].param_value, "%d", &delay);
			if (delay <= 0 )
				return -6;
		} else {
			return -8;
		}
	}
	return 1;
}

int PrintMainMenu(void)
{
	system(CLEAR);
	int choice;
	printf("本程序为一个电梯仿真程序,仿真两部电梯的联动运行,提供如下功能:\n");
	printf("\t1. 指定用户指令文件,静默仿真(只输出仿真结果文件)\n");
	printf("\t2. 指定用户指令文件,动画仿真(不输出仿真结果文件)\n");
	printf("\t3. 指定用户指令文件,全面仿真(即显示动画,又输出结果文件)\n");
	printf("\t4. 指定历史仿真文件,动画形式回放\n");
	printf("\t5. 修改系统参数\n");
	printf("\t0. 退出\n");
	do {
		printf("请输入您要执行的功能序号（0，1，2，3, 4, 5）：");
		scanf("%d", &choice);
		getchar();
		printf("----------------------------------\n");
	} while (choice < 0 || choice > 5);
	return choice;
}

void ShowMenu(int menu_flag)
{
	int choice;
	switch (menu_flag) {
	case 1:
		printf("您当前处于“静默仿真”模块,请选择要进行的操作：\n");
		printf("\t1. 输入用户指令文件名称,开始静默仿真\n");
		printf("\t0. 返回上级菜单\n");
		do {
			printf("请输入您需要进行的操作序号：");
			scanf("%d", &choice);
			getchar();
			printf("----------------------------------\n");
		} while (choice < 0 || choice > 1);
		if (choice) {
			char filename[256];
			printf("输入文件名：");
			scanf("%s", filename);
			SilenceSimulate(filename);
		}
		break;
	case 2:
		printf("您当前处于“动画仿真”模块,请选择要进行的操作：\n");
		printf("\t1. 输入用户指令文件名称,开始动画仿真\n");
		printf("\t0. 返回上级菜单\n");
		do {
			printf("请输入您需要进行的操作序号：");
			scanf("%d", &choice);
			getchar();
			printf("----------------------------------\n");
		} while (choice < 0 || choice > 1);
		if (choice) {
			char filename[256];
			printf("输入文件名：");
			scanf("%s", filename);
			AnimateSimulation(filename);
		}
		break;
	case 3:
		printf("您当前处于“全面仿真”模块,请选择要进行的操作：\n");
		printf("\t1. 输入用户指令文件名称,开始全面仿真\n");
		printf("\t0. 返回上级菜单\n");
		do {
			printf("请输入您需要进行的操作序号：");
			scanf("%d", &choice);
			getchar();
			printf("----------------------------------\n");
		} while (choice < 0 || choice > 1);
		if (choice) {
			char filename[256];
			printf("输入文件名：");
			scanf("%s", filename);
			SilenceSimulate(filename);
			AnimateSimulation(filename);
		}
		break;
	case 4:
		printf("您当前处于“历史仿真文件回放”模块,请选择要进行的操作：\n");
		printf("\t1. 输入历史仿真文件名称,开始仿真回放\n");
		printf("\t0. 返回上级菜单\n");
		do {
			printf("请输入您需要进行的操作序号：");
			scanf("%d", &choice);
			getchar();
			printf("----------------------------------\n");
		} while (choice < 0 || choice > 1);
		if (choice) {
			char filename[256];
			printf("输入文件名：");
			scanf("%s", filename);
			HistorySimulate(filename);
		}
		break;
	case 5:
		printf("您当前处于“系统参数配置”模块,请选择要配置的参数：\n");
		printf("\t1. 配置用户指令文件存放目录\n");
		printf("\t2. 配置历史仿真文件存放目录\n");
		printf("\t3. 配置电梯总层数\n");
		printf("\t4. 配置电梯动画仿真时的延迟系数\n");
		printf("\t0. 返回上级菜单\n");
		do {
			printf("请输入您需要进行的操作序号：");
			scanf("%d", &choice);
			getchar();
			printf("----------------------------------\n");
		} while (choice < 0 || choice > 4);
		if (choice) {
			ParamConf(choice);
		}
		break;
	default:
		break;
	}
}

void InitSimulation(void)
{
	if (usercall_list != NULL)
		free(usercall_list);
	finish_call_num = 0;
	time = 0;
	status_change_flag = 0;
}

void InitElevator(ELEVATORSTATE *elevator)
{
	elevator->current_floor = 1;
	elevator->run_state = 'S';
	elevator->serve_state = 'P';
	if (elevator->current_usercall != NULL)
		free(elevator->current_usercall);
	elevator->current_usercall = NULL;
}

void SilenceSimulate(const char filename[])
{
	InitSimulation();
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name, "UserRequestFilePath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
				char fullpath[256];
				strncpy(fullpath, sysparam_list[i].param_value, sizeof(fullpath));
				strncat(fullpath, OS_SEP, sizeof(fullpath) - strlen(fullpath));
				strncat(fullpath, filename, sizeof(fullpath) - strlen(fullpath));
				usercall_num = parseUserCall(fullpath, &usercall_list);
			} else {
				usercall_num = parseUserCall(filename, &usercall_list);
			}
		}
	}
	if (usercall_num <= 0) {
		fprintf(stderr, "Parse error: %d.\n", usercall_num);
		return;
	}
	FILE *fp = NULL;
	char history_name[256];
	printf("输入仿真历史文件名：");
	scanf("%s", history_name);
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name, "SimulationFilesPath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
				char fullpath[256];
				strncpy(fullpath, sysparam_list[i].param_value, sizeof(fullpath));
				strncat(fullpath, OS_SEP, sizeof(fullpath) - strlen(fullpath));
				strncat(fullpath, history_name, sizeof(fullpath) - strlen(fullpath));
				fp = fopen(fullpath, "w");
			} else {
				fp = fopen(history_name, "w");
			}
		}
	}
	if (fp == NULL) {
		fprintf(stderr, "Cannot open file!\n");
		return;
	}
	fprintf(fp, "%d\n", usercall_num);
	for (int i = 0; i < usercall_num; i++)
		fprintf(fp, "%d,%d,%d\n", usercall_list[i].user_floor, usercall_list[i].user_target, usercall_list[i].call_time);
	fprintf(fp, "***********************************\n");
	fprintf(fp, "%d\n", sysparam_num);
	for (int i = 0; i < sysparam_num; i++)
		fprintf(fp, "%s=%s\n", sysparam_list[i].param_name, sysparam_list[i].param_value);
	fprintf(fp, "***********************************\n");
	ELEVATORSTATE *elevators[2] = {&elevator_a, &elevator_b};
	for (int i = 0; i < 2; i++) {
		InitElevator(elevators[i]);
	}
	while (finish_call_num < usercall_num || elevator_a.current_usercall != NULL || elevator_b.current_usercall != NULL) {
		int updated = 0;
		for (int i = 0; i < 2; i++) {
			if (elevators[i]->current_usercall == NULL && usercall_list[finish_call_num].call_time <= time && finish_call_num < usercall_num) {
				updated = 1;
				elevators[i]->current_usercall = &usercall_list[finish_call_num];
				finish_call_num++;
			}
			if (elevators[i]->current_usercall != NULL) {
				switch (elevators[i]->run_state) {
				case 'U':
					elevators[i]->current_floor++;
					break;
				case 'D':
					elevators[i]->current_floor--;
					break;
				case 'S':
					// Check whether the user/target floor is the same as current_floor.
					if (elevators[i]->serve_state == 'P') {
						if (elevators[i]->current_floor == elevators[i]->current_usercall->user_floor) {
							elevators[i]->serve_state = 'E';
							updated = 1;
						}
					}
					if (elevators[i]->serve_state == 'E') {
						if (elevators[i]->current_floor == elevators[i]->current_usercall->user_target) {
							elevators[i]->serve_state = 'P';
							elevators[i]->current_usercall = NULL;
							updated = 1;
						}
					}
				}
			}
			if (elevators[i]->current_usercall != NULL) {
				char new_run_state = 'S';
				switch (elevators[i]->serve_state) {
				case 'P':
					if (elevators[i]->current_floor < elevators[i]->current_usercall->user_floor) {
						new_run_state = 'U';
					} else if (elevators[i]->current_floor > elevators[i]->current_usercall->user_floor) {
						new_run_state = 'D';
					} else {
						new_run_state = 'S';
						elevators[i]->serve_state = 'E';
						updated = 1;
					}
					break;
				case 'E':
					if (elevators[i]->current_floor < elevators[i]->current_usercall->user_target) {
						new_run_state = 'U';
					} else if (elevators[i]->current_floor > elevators[i]->current_usercall->user_target) {
						new_run_state = 'D';
					} else {
						new_run_state = 'S';
						elevators[i]->serve_state = 'P';
						elevators[i]->current_usercall = NULL;
						updated = 1;
					}
					break;
				}
				if (elevators[i]->run_state != new_run_state) {
					elevators[i]->run_state = new_run_state;
					updated = 1;
				}
			}
		}
		USERCALL *current = NULL;
		for (int i = 0; i < usercall_num && usercall_list[i].call_time <= time; i++)
			current = &usercall_list[i];
		if (current != NULL)
			if (current->call_time == time)
				updated = 1;
		int start = finish_call_num;
		if (elevator_a.current_usercall != NULL)
			start--;
		if (elevator_b.current_usercall != NULL)
			start--;
		if (updated || !time) {
			fprintf(fp, "%d,%c,%d,%c,%c,%d,%c;", time, 'A', elevator_a.current_floor, elevator_a.run_state, 'B', elevator_b.current_floor, elevator_b.run_state);
			for (int i = start; i <= current - usercall_list; i++) {
				if (&usercall_list[i] == elevator_a.current_usercall)
					fprintf(fp, "<%d,%d,%d,%c,%c>", i + 1, usercall_list[i].user_floor, usercall_list[i].user_target, elevator_a.serve_state, 'A');
				else if (&usercall_list[i] == elevator_b.current_usercall)
					fprintf(fp, "<%d,%d,%d,%c,%c>", i + 1, usercall_list[i].user_floor, usercall_list[i].user_target, elevator_b.serve_state, 'B');
				else
					fprintf(fp, "<%d,%d,%d,%c,%c>", i + 1, usercall_list[i].user_floor, usercall_list[i].user_target, 'N', 'N');
				if (i != current - usercall_list)
					fprintf(fp, ",");
			}
			fprintf(fp, "\n");
		}
		time++;
	}
	fclose(fp);
}

void HistorySimulate(const char filename[])
{
	InitSimulation();
	FILE *fp = NULL;
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name, "SimulationFilesPath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
				char fullpath[256];
				strncpy(fullpath, sysparam_list[i].param_value, sizeof(fullpath));
				strncat(fullpath, OS_SEP, sizeof(fullpath) - strlen(fullpath));
				strncat(fullpath, filename, sizeof(fullpath) - strlen(fullpath));
				usercall_num = parseUserCall(fullpath, &usercall_list);
			} else {
				usercall_num = parseUserCall(filename, &usercall_list);
			}
		}
	}
	if (usercall_num <= 0) {
		fprintf(stderr, "Parse error: %d.\n", usercall_num);
		return;
	}
	for (int i = 0; i < sysparam_num; i++) {
		if (strcmp(sysparam_list[i].param_name, "SimulationFilesPath") == 0) {
			if (strcmp(sysparam_list[i].param_value, "*")) {
				char fullpath[256];
				strncpy(fullpath, sysparam_list[i].param_value, sizeof(fullpath));
				strncat(fullpath, OS_SEP, sizeof(fullpath) - strlen(fullpath));
				strncat(fullpath, filename, sizeof(fullpath) - strlen(fullpath));
				fp = fopen(fullpath, "r");
			} else {
				fp = fopen(filename, "r");
			}
		}
	}
	if (fp == NULL) {
		fprintf(stderr, "Cannot open file!\n");
		return;
	}
	char line[256];
	do {
		fgets(line, 256, fp);
	} while (line[0] != '*');
	int lineNum;
	fscanf(fp, "%d", &lineNum);
	fgetc(fp);
	char *split = NULL;
	for (int i = 0; i < lineNum; i++) {
		fgets(line, 256, fp);
		if ((split = strchr(line, '=')) != NULL) {
			strncpy(sysparam_list[i].param_name, line, split - line);
			sysparam_list[i].param_name[strlen(sysparam_list[i].param_name)] = '\0';
			strncpy(sysparam_list[i].param_value, split + 1, strlen(split));
			sysparam_list[i].param_value[strlen(sysparam_list[i].param_value) - 1] = '\0';
		} else {
			free(sysparam_list);
			fclose(fp);
			fprintf(stderr, "Parse error: %d.\n", usercall_num);
			return;
		}
	}
	fclose(fp);
	animate_route();
}
