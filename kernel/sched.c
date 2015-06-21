#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/leanux.h>
#include <leanux/mm.h>
#include <leanux/sched.h>

struct process_t *current_process;
struct process_list_t ready_list;
struct process_t *process_list[MAX_PROC_NUM];

void init_sched() {
    current_process = NULL;
    ready_list.head = NULL;
    ready_list.size = 0;
}





