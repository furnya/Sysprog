#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "../Tasks/TaskPool.h"
#include "./Monkey.h"
#include "./FCFS.h"
#include "./SJN.h"
#include "./RR.h"
#include "./MLF.h"

enum SCHEDULING_METHOD {
    FCFS = 0,
    SJN,
    RR,
    MLF,
    MONKEY
};

/**
 * @brief   Applies a scheduling method to a given task pool.
 * @param   task_pool   A Pointer to the task_pool, whose objects are to be scheduled.
 * @param   method      An Enum determining the scheduling method. Either FCFS, SJN, RR, MLF, MONKEY
 * @param   quantum     Only of interest if Round Robin scheduling is selected.
 **/
void schedule(TaskPool *task_pool, enum SCHEDULING_METHOD method, uint16_t scheduling_args);

#endif
