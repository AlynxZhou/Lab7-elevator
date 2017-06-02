#ifndef __DATAMODEL_H__
#	define __DATAMODEL_H__

#	define MAX_PARAM_ARRAY_LEN 256

#	define SYSCONF "SysConf"
#	define SYSPARAM_TXT "SysParam.txt"

#	ifdef _WIN32
#		define OS_SEP "\\"
#		define CLEAR "cls"
#	else
#		define OS_SEP "/"
#		define CLEAR "clear"
#	endif

#	include <stdio.h>
#	include <stdlib.h>

typedef struct UserCall {
	int user_floor;		// 用户所在楼层
	int user_target;	// 用户目标楼层
	int call_time;		// 用户请求时刻
	char call_type;		// 用户指令类型, ́U ́表示上行指令, ́D ́表示下行指令
} USERCALL;

typedef struct SysParam {
	//参数名
	char param_name[MAX_PARAM_ARRAY_LEN];
	//参数值
	char param_value[MAX_PARAM_ARRAY_LEN];
} SYSPARAM;

typedef struct elevatorstate {
	int current_floor;	// 电梯当前所处楼层
	char run_state;		// 电梯运行状态
	char serve_state;
	USERCALL *current_usercall;	// 电梯响应的用户指令队列
} ELEVATORSTATE;

extern SYSPARAM *sysparam_list;
extern int sysparam_num;
extern USERCALL *usercall_list;
extern int usercall_num;
extern ELEVATORSTATE elevator_a;
extern ELEVATORSTATE elevator_b;
extern int finish_call_num;
extern int time;
extern int status_change_flag;

#endif
