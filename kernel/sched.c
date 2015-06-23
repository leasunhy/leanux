#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/leanux.h>
#include <leanux/interrupt.h>
#include <leanux/mm.h>
#include <leanux/timer.h>
#include <leanux/sched.h>
#include <lib/utils.h>

struct process_t *current_process;
struct process_list_t ready_list;
struct process_t *process_list[MAX_PROC_NUM];

struct process_t sched_proc;

void init_process_queue(struct process_list_t *q);
void sched_enqueue(struct process_list_t *q, struct process_t *process);
struct process_t *sched_serve(struct process_list_t *q);

/* force a switch when current time slice ends */
/* TODO: the return value is stored in original stack. maybe mem leak? */
/*__attribute__((noreturn))*/
void force_switch() {
    if (!current_process)
        return;
    current_process->status = READY;
    sched_enqueue(&ready_list, current_process);
    current_process = sched_serve(&ready_list);
    current_process->status = RUNNING;
    /*switch_pd_stack_regs_and_jmp(current_process->page_dir,*/
            /*current_process->context.esp, current_process->context.eip);*/
    switch_pd(current_process->page_dir);
    __asm__ __volatile__("mov esp, %0" : : "r"(current_process->context.esp));
}

void sched_init() {
    disable_interrupt();
    current_process = NULL;
    ready_list.head = NULL;
    ready_list.size = 0;
    register_timer_event(&force_switch, 10, TIMER_CONSTANT);
    init_process_queue(&ready_list);
    enable_interrupt();
}

void init_process_queue(struct process_list_t *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void sched_enqueue(struct process_list_t *q, struct process_t *process) {
    q->tail->next = process;
    q->tail = process;
    process->next = NULL;
    q->size += 1;
}

struct process_t *sched_serve(struct process_list_t *q) {
    struct process_t *res = q->head;
    q->head = res->next;
    res->next = NULL;
}


