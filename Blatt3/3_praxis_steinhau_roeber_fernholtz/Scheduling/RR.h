#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_RR_H
#define SCHEDULE_RR_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the RR-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 * @param   quantum     A number determining the quantum which is given to a thread each cycle
 */
void schedule_RR(const TaskPool *task_pool, uint16_t quantum);

#endif
