#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_SJN_H
#define SCHEDULE_SJN_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the SJN-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 */
void schedule_SJN(const TaskPool *task_pool);

#endif
