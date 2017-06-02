#include "DataImport.h"

int parseSysParam(const char sysParamPath[], \
		  SYSPARAM **sysparam_list)
{
	FILE *fp;
	int lineNum;
	if ((fp = fopen(sysParamPath, "r")) == NULL)
		return -1;
	fscanf(fp, "%d", &lineNum);
	fgetc(fp);
	*sysparam_list = (SYSPARAM *)malloc(lineNum * sizeof(SYSPARAM));
	char line[256];
	char *split = NULL;
	int i;
	for (i = 0; fgets(line, 256, fp) != NULL; i++) {
		if ((split = strchr(line, '=')) != NULL) {
			strncpy((*sysparam_list)[i].param_name, line, split - line);
			(*sysparam_list)[i].param_name[split - line] = '\0';
			strncpy((*sysparam_list)[i].param_value, split + 1, strlen(split + 1));
			(*sysparam_list)[i].param_value[strlen(split + 1) - 1] = '\0';
			printf("%s = %s\n", (*sysparam_list)[i].param_name, (*sysparam_list)[i].param_value);
		} else {
			free(*sysparam_list);
			fclose(fp);
			return -2;
		}
	}
	fclose(fp);
	if (lineNum != i)
		return -3;
	return lineNum;
}

int parseUserCall(const char userCallPath[], \
		  USERCALL **usercall_list)
{
	FILE *fp;
	int lineNum;
	if ((fp = fopen(userCallPath, "r")) == NULL)
		return -1;
	fscanf(fp, "%d", &lineNum);
	fgetc(fp);
	*usercall_list = (USERCALL *)malloc(lineNum * sizeof(USERCALL));
	for (int i = 0; i < lineNum; i++) {
		if (fscanf(fp, "%d,%d,%d", &((*usercall_list)[i].user_floor), &((*usercall_list)[i].user_target), &((*usercall_list)[i].call_time)) == 3) {
			if ((*usercall_list)[i].user_target - (*usercall_list)[i].user_floor > 0)
				(*usercall_list)[i].call_type = 'U';
			else if ((*usercall_list)[i].user_target - (*usercall_list)[i].user_floor < 0)
				(*usercall_list)[i].call_type = 'D';
			else
				(*usercall_list)[i].call_type = 'S';
		} else {
			free(*usercall_list);
			fclose(fp);
			return -2;
		}
	}
	return lineNum;
}
