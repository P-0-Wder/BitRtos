/*
* coder:  8_B!T0
*
* bref:   contain this part in task.c and runtime.c
*         then u can create runtime_event and task_event
*
*         every time we call the event operation function we need to use the mutex function
*         use mutex to insure the operation will not occured data access confict
*
*         if some event are in the processing and another event triggerd at the same time (trigged by some kernal interrupt)
*         the operation on the Triggerd_Event_List may occured the var access confict
*         so we need the mutex lock to process the Event operation one at time
*
*         CAUTION:
*         THIS PART ONLY CAN BE USED IN SYSTEM EVENT PROCESS
*         DO NOT REGISTE CUSTOM EVENT IN APP CODE OR ELSE IT MAY BLOCK THE SYSTEM RUNNING
*/

#include "event_func.h"
#include "mutex.h"

static uint16_t Reg_Event_Num = 0;
static bool OSEvent_Init_State = false;
static Mutex_Def(event, NULL, mutex_locker_init);

typedef struct
{
    bool empty;
    item_obj *fst_item_ptr;
    item_obj *lst_item_ptr;
    list_obj Event_List;
} Triggerd_Event_TypeDef;

static Triggerd_Event_TypeDef Triggerd_Event_List = {
    .empty = true,
    .fst_item_ptr = NULL,
    .lst_item_ptr = NULL,
    .Event_List = {
        .prv = NULL,
        .data = NULL,
        .nxt = NULL}};

bool Event_Enable(void)
{
    if (Reg_Event_Num > 0)
    {
        OSEvent_Init_State = true;
        return true;
    }

    return false;
}

event_regs_TypeDef Event_Regsiter(event_proc_type proc_type, module_event_id module_id, uint8_t event_id, char *desc, void *arg, Event_CallBack_Func callback)
{
    event_regs_TypeDef state;
    item_obj *new_evt_item = NULL;
    event_block_s *new_evt_block = NULL;

    state.item_hdl = 0;
    state.block_hdl = 0;
    state.error_id = event_no_error;

    new_evt_item = (item_obj *)malloc(sizeof(item_obj));
    if (new_evt_item == NULL)
    {
        state.item_hdl = 0;
        state.error_id = evebt_item_create_error;
        return state;
    }

    new_evt_block = (event_block_s *)malloc(sizeof(event_block_s));
    if (new_evt_block == NULL)
    {
        state.block_hdl = 0;
        state.error_id = event_block_create_error;
        return state;
    }

    new_evt_block->arg = arg;
    new_evt_block->callback = callback;
    new_evt_block->descr = desc;
    new_evt_block->module_id = module_id;
    new_evt_block->evt_id = event_id;
    new_evt_block->proc_type = proc_type;
    new_evt_block->triggered_status = false;
    RunTime_Reset(&new_evt_block->trg_time);

    List_ItemInit(new_evt_item, new_evt_block);
    state.item_hdl = *&new_evt_item;
    state.block_hdl = *&new_evt_block;

    Reg_Event_Num++;
    return state;
}

//when trigger the event. insert current event item into the Triggerd_Event_List
//if the process type is not process immediately then event callback function while
//execute in systick handler else it will execute the event callback function directily
event_err_code_id Event_Trigger(SYSTEM_RunTime os_runtime, event_item_handler evt_itm)
{
    event_block_s *evt_trg = NULL;
    error_rpt_TypeDef locker_error;

    if (OSEvent_Init_State)
    {
        if (evt_itm != 0)
        {
            evt_trg = ((event_block_s *)(((item_obj *)evt_itm)->data));
            evt_trg->trg_time = os_runtime;

            if (evt_trg->proc_type == evt_proc_Immd)
            {
                if (evt_trg->callback != NULL)
                {
                    evt_trg->callback(evt_trg->arg);

                    return event_no_error;
                }
            }
            else
            {
                //use mutex lock current operation
                locker_error = Locker.lock(&event_locker, Event_Trigger, __FUNCTION__, __LINE__);

                if (locker_error.code == mutex_operate_done)
                {
                    //if lock process successed

                    //insert event item in the last pos of the Triggerd_Event_List
                    //wait to the execution
                    if (Triggerd_Event_List.empty)
                    {
                        Triggerd_Event_List.empty = false;
                        List_Init(&Triggerd_Event_List.Event_List, (item_obj *)evt_itm, DEFAULT_ARRANGE_MODE, DEFAULT_COMPARE_CALLBACK);
                        Triggerd_Event_List.fst_item_ptr = evt_itm;
                    }
                    else
                    {
                        List_Insert_Item(Triggerd_Event_List.lst_item_ptr, evt_itm);
                    }

                    //the last item pointer in the list
                    //for this line only to make insert function more effcienttly
                    Triggerd_Event_List.lst_item_ptr = evt_itm;

                    //unlock the mutec locker for Triggerd_Event_List can be operate by other function
                    Locker.unlock(&event_locker, Event_Trigger, __FUNCTION__, __LINE__);
                    return event_no_error;
                }
                else
                {
                    //push current operation into stand by list
                    //wait to event poll
                }
            }
        }
    }
    else
        return event_uninit;

    return event_item_error;
}

event_err_code_id Event_RmItem(event_regs_TypeDef reg_evt)
{
    error_rpt_TypeDef locker_state;

    if (OSEvent_Init_State)
    {
        if (Reg_Event_Num > 0)
        {
            //lock the process down below
            locker_state = Locker.lock(&event_locker, Event_RmItem, __FUNCTION__, __LINE__);

            if (locker_state.code == mutex_operate_done)
            {
                //if target event item in the Triggerd_Event_List
                //we need to remove that item from the list first
                //then clear memory space
                List_Delete_Item(((item_obj *)reg_evt.item_hdl), NULL);

                if (reg_evt.block_hdl != 0)
                {
                    free((event_block_s *)reg_evt.block_hdl);
                }
                else
                    return event_block_error;

                if (reg_evt.item_hdl != 0)
                {
                    free((item_obj *)reg_evt.item_hdl);
                }
                else
                    return event_item_error;

                Reg_Event_Num--;
                return event_no_error;
            }
            else
            {
                //push current process into stand by list
                //wait to event poll
            }
        }

        return event_no_registed_things;
    }

    return event_uninit;
}

static event_err_code_id Event_ProcStandBy_Operation()
{
}

event_err_code_id Event_Poll(void)
{
    error_rpt_TypeDef locker_state;
    item_obj *event_item_tmp = NULL;
    event_block_s *event_ctl_blk = NULL;

    if (OSEvent_Init_State)
    {
        if (!Triggerd_Event_List.empty)
        {
            if (Triggerd_Event_List.fst_item_ptr->prv != NULL)
            {
                //then set first item in Triggerd_Event_List function error
                return event_proc_fstitm_error;
            }
            else
            {
                locker_state = Locker.lock(&event_locker, Event_Poll, __FUNCTION__, __LINE__);

                if (locker_state.code == mutex_operate_done)
                {

                    event_item_tmp = List_PopFirst(&Triggerd_Event_List.Event_List);

                    if (event_item_tmp == NULL)
                    {
                        Triggerd_Event_List.empty = true;
                        Triggerd_Event_List.fst_item_ptr = NULL;
                        Triggerd_Event_List.lst_item_ptr = NULL;

                        return event_list_empty;
                    }

                    if (Triggerd_Event_List.Event_List.prv == NULL)
                    {
                        Triggerd_Event_List.fst_item_ptr = &Triggerd_Event_List;
                    }
                    else
                        return event_popfirst_error;

                    //convert item data into event_block_s structure
                    //then run callback function
                    event_ctl_blk = (event_block_s *)(event_item_tmp->data);
                    if (event_ctl_blk->callback != NULL)
                    {
                        event_ctl_blk->prc_time = Get_Cur_Runtime();
                        event_ctl_blk->callback(event_ctl_blk->arg);
                    }

                    //unlock the process
                    Locker.unlock(&event_locker, Event_Poll, __FUNCTION__, __LINE__);
                }
                else
                {
                    //can not doing event poll process now
                    //cuse Triggerd_Event_List is in using by other operation
                    //so try latter
                }
            }
        }
        else
            return event_list_obj_error;
    }
    else
        return event_uninit;

    return event_no_error;
}
