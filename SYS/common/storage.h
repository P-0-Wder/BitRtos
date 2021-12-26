#ifndef __STORAGE_H
#define __STORAGE_H

#include <stdbool.h>

#define External_StorageDevice_Num 0

#define User_ParamAddr
#define Sys_ParamAddr

typedef enum
{
    Internal_Storage_Dev = 0,
    External_Storage_Dev,
} Storage_Interface_List;

typedef enum
{
    Param_System = 0,
    Param_User,
} Param_Type;

typedef enum
{
    Param_Level_Normal,
    Param_Level_Important,
    Param_Level_Fatal,
} Param_Protect_Level;

typedef enum
{
    Storage_NoError = 0,
    Storage_Error_Addr,
    Storage_Error_Section,
} Storage_ErrorCode_List;

#endif