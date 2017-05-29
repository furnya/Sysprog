#include "../Tasks/TaskPool.h"
#include <stdio.h>

#ifndef SCHEDULE_FCFS_H
#define SCHEDULE_FCFS_H

/**
 * @brief   Schedules the Tasks in the TaskPool via the FCFS-Method
 * @param   task_pool   A pointer to the TaskPool whose Tasks are to be scheduled.
 */
void schedule_FCFS(const TaskPool *task_pool);

#endif
