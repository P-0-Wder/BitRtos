#ifndef __MUTEX_H
#define __MUTEX_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VAR_NAME_2_STR(x) #x

#define Mutex_Def(na, func_name, init_val) \
	Mutex_Obj_TypeDef na##_locker = {      \
		.name = #na,                       \
		.func = func_name,                 \
		.status = init_val};

typedef enum
{
	mutex_operate_done,
	mutex_already_locked,
	mutex_locked_by_other,
	mutex_already_unlocked,
} mutex_error_code;

typedef enum
{
	mutex_locker_init = 0,
	mutex_locked,
	mutex_unlocked,
} mutex_lock_status;

//error report structure
typedef struct
{
	mutex_error_code code;
	char *desc;
} error_rpt_TypeDef;

typedef struct
{
	uint32_t lock_times;
	uint32_t unlock_times;
	uint32_t error_times;

	int32_t opr_dif;

} Mutex_Opt_Summary;

//mutex status
typedef struct
{
	Mutex_Opt_Summary smry;
	mutex_lock_status lock_status;
	uint32_t lock_by;
} Mutex_Status_TypeDef;

typedef struct
{
	char *name;

	Mutex_Status_TypeDef status;

	char *func;
	uint16_t line;
} Mutex_Obj_TypeDef;

typedef struct
{
	void (*init)(Mutex_Obj_TypeDef *obj, char *name);
	void (*reset)(Mutex_Obj_TypeDef *obj);
	error_rpt_TypeDef (*lock)(Mutex_Obj_TypeDef *obj, uint32_t func_addr, char *func, uint16_t line);
	error_rpt_TypeDef (*unlock)(Mutex_Obj_TypeDef *obj, uint32_t func_addr, char *func, uint16_t line);
	Mutex_Status_TypeDef (*status)(Mutex_Obj_TypeDef *obj);
} Mutex_Locker_TypeDef;

extern Mutex_Locker_TypeDef Locker;

#endif
