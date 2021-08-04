#include "mutex.h"
#include "stm32f4xx.h"

static void Mutex_Init(Mutex_Obj_TypeDef *obj, char *name);
static void Mutex_Reset(Mutex_Obj_TypeDef *obj);
static error_rpt_TypeDef Mutex_Lock(Mutex_Obj_TypeDef *obj, uint32_t func_addr, char *func, uint16_t line);
static error_rpt_TypeDef Mutex_UnLock(Mutex_Obj_TypeDef *obj, uint32_t func_addr, char *func, uint16_t line);
static Mutex_Status_TypeDef Mutext_Status(Mutex_Obj_TypeDef *obj);

Mutex_Locker_TypeDef Locker = {.init = Mutex_Init,
							   .reset = Mutex_Reset,
							   .lock = Mutex_Lock,
							   .unlock = Mutex_UnLock,
							   .status = Mutext_Status};

static void Mutex_Init(Mutex_Obj_TypeDef *obj, char *name)
{
	obj->name = name;

	obj->status.lock_status = mutex_locker_init;
	obj->status.lock_by = 0;

	obj->status.smry.lock_times = 0;
	obj->status.smry.unlock_times = 0;

	obj->line = 0;
	obj->func = NULL;
}

static void Mutex_Reset(Mutex_Obj_TypeDef *obj)
{
	obj->name = NULL;

	obj->status.lock_status = mutex_locker_init;
	obj->status.lock_by = 0;

	obj->status.smry.lock_times = 0;
	obj->status.smry.unlock_times = 0;

	obj->line = 0;
	obj->func = NULL;
}

static error_rpt_TypeDef Mutex_Lock(Mutex_Obj_TypeDef *obj, uint32_t func_addr, char *func, uint16_t line)
{
	error_rpt_TypeDef error;

	error.code = mutex_operate_done;
	error.desc = NULL;

	if ((obj->status.lock_status == mutex_unlocked) || (obj->status.lock_status == mutex_locker_init))
	{
		__ASM("CPSID I");
		while (true)
		{
			obj->func = NULL;

			obj->status.lock_status = mutex_locked;
			obj->status.lock_by = func_addr;

			obj->status.smry.lock_times++;

			obj->line = line;
			obj->func = func;

			break;
		}
		__ASM("CPSIE I");
	}
	else
	{
		if (func_addr == obj->status.lock_by)
		{
			error.code = mutex_already_locked;
			error.desc = "already locked";
		}
		else
		{
			obj->status.smry.error_times++;
			error.desc = strcat(func, " try lock the locker but locker already locked by ");
			error.desc = strcat(error.desc, obj->func);
			error.code = mutex_locked_by_other;
		}
	}

	return error;
}

static error_rpt_TypeDef Mutex_UnLock(Mutex_Obj_TypeDef *obj, uint32_t func_addr, char *func, uint16_t line)
{
	error_rpt_TypeDef error;

	error.code = mutex_operate_done;
	error.desc = NULL;

	if (obj->status.lock_status != mutex_unlocked)
	{
		if (obj->status.lock_by == func_addr)
		{
			__ASM("CPSID I");
			while (true)
			{
				obj->func = NULL;

				obj->status.lock_status = mutex_unlocked;
				obj->status.lock_by = 0;

				obj->status.smry.unlock_times++;

				obj->line = line;
				obj->func = func;

				break;
			}
			__ASM("CPSIE I");
		}
		else
		{
			error.code = mutex_locked_by_other;
			obj->status.smry.error_times++;

			error.desc = strcat(func, " try unlock the locker but locker locked by ");
			error.desc = strcat(error.desc, obj->func);
		}
	}
	else
	{
		error.code = mutex_already_unlocked;
		obj->status.smry.error_times++;

		error.desc = strcat("locker already unlocked by ", obj->func);
	}

	return error;
}

static Mutex_Status_TypeDef Mutext_Status(Mutex_Obj_TypeDef *obj)
{
	obj->status.smry.opr_dif = obj->status.smry.lock_times - obj->status.smry.unlock_times;
	//if obj->status.smry.opr_dif > 0
	//then lock operation times > unlock operations times
	//if obj->status.smry.opr_dif < 0
	//then lock operation times < unlock operations times
	//if obj->status.smry.opr_dif = 0
	////then lock operation times = unlock operations times
	return obj->status;
}
