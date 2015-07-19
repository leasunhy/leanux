#ifndef _LEANUX_SCHED_H
#define _LEANUX_SCHED_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <leanux/leanux.h>
#include <leanux/mm.h>

enum process_status_t {
    RUNNING,
    READY,
    BLOCKED
};

enum block_event_t {
    WNONE,
    WCHILD
};

struct process_context_t {
    uint32_t esp;
};

struct process_t {
    uint32_t id;
    uint32_t parent_id;
    struct process_context_t context;
    PDE *page_dir;
    enum process_status_t status;
    enum block_event_t block_event;
    uint32_t page_no;

    struct process_t *next;
};

struct process_list_t {
    struct process_t *head, *tail;
    size_t size;
};

void sched_init();
void force_switch();

void sched_loop();

void sched_enqueue(struct process_list_t *q, struct process_t *process);

void switch_pd_stack_regs_and_jmp(PDE *pd, uint32_t esp, uint32_t eip);

void init_process(struct process_t *proc);
bool add_process(struct process_t *proc);

int sys_fork(int32_t _1, int32_t _2, int32_t _3);
int _sys_fork(int32_t _1, int32_t _2, int32_t _3);
void sys_fork_end();

int sys_shared_fork(int32_t _1, int32_t _2, int32_t _3);
int _sys_shared_fork(int32_t _1, int32_t _2, int32_t _3);
void sys_shared_fork_end();

int sys_exit(int32_t code, int32_t _2, int32_t _3);
int sys_waitpid(int32_t pid, int32_t status, int32_t options);

extern volatile struct process_t *current_process;
extern struct process_list_t ready_list;
extern struct process_t *process_list[MAX_PROC_NUM];

extern uint32_t stored_esp;

#endif

