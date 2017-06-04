#include "Scheduling.h"

void schedule(TaskPool *task_pool, enum SCHEDULING_METHOD method, uint16_t args) {
    switch (method) {
        case FCFS:
            schedule_FCFS(task_pool);
            break;
        case SJN:
            schedule_SJN(task_pool);
            break;
        case RR:
            schedule_RR(task_pool, args);
            break;
        case MLF:
            schedule_MLF(task_pool, args);
            break;
        case MONKEY:
            schedule_Monkey(task_pool);
            break;
    }
}
