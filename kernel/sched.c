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

struct process_t *kernel_proc;

/*next_pid is the pid to be assigned for the next process*/
uint32_t next_pid;

/* force a switch when current time slice ends */
void force_switch() {
    if (!current_process && ready_list.size == 0)
        return;
    if (current_process) {
        if (ready_list.size == 0)
            return;
        current_process->status = READY;
        sched_enqueue(&ready_list, current_process);
        /*__asm__ __volatile__("mov %0, esp" : "=r"(current_process->context.esp));*/
        current_process->context.esp = stored_esp;
    }
    current_process = sched_serve(&ready_list);
    current_process->status = RUNNING;
    switch_pd(current_process->page_dir);
    __asm__ __volatile__("mov esp, %0" : : "r"(current_process->context.esp));
}

void sched_init() {
    disable_interrupt();

    memory_set(process_list, 0, sizeof(process_list));
    next_pid = 0;
    current_process = NULL;
    ready_list.head = NULL;
    ready_list.size = 0;
    /* time slice is 100ms */
    register_timer_event(&force_switch, 10, TIMER_CONSTANT);
    init_process_queue(&ready_list);

    enable_interrupt();
}

void init_process(struct process_t *proc) {
    memory_set(proc, 0, sizeof(struct process_t));

    proc->id = next_pid++;
    proc->next = NULL;
    proc->status = READY;

    uint32_t pd_no = alloc_page(0, 1023);
    proc->page_dir = (PDE *)(pd_no << 12);
    mm_mmap(kernel_pd, pd_no, pd_no, true, false, true);
    memory_set(proc->page_dir, 0, 4096);
    proc->page_dir[0] = kernel_pd[0];

    proc->page_no = alloc_page(1024, num_of_page);
    mm_mmap(kernel_pd, proc->page_no, proc->page_no, true, false, true);
    mm_mmap(proc->page_dir, USER_PROG_ADDR >> 12, proc->page_no,
            true, false, true);

    uint32_t stack_no = alloc_page(1024, num_of_page);
    mm_mmap(proc->page_dir, 0xFFFFF, stack_no, false, true, true);

    /* prepare the stack content */
    PDE *cur_pd = current_pd();
    switch_pd(proc->page_dir);
    uint32_t *proc_stack = (uint32_t *)0xFFFFFFFC;
    *proc_stack = 0x202;  /*eflags*/
    *--proc_stack = 0x8;  /*cs*/
    *--proc_stack = USER_PROG_ADDR;  /*eip*/
    *--proc_stack = 0x0;  /*eax*/
    *--proc_stack = 0x0;  /*ecx*/
    *--proc_stack = 0x0;  /*edx*/
    *--proc_stack = 0x0;  /*ebx*/
    *--proc_stack = 0xFFFFFFFC - 4 * 2;  /*esp*/
    *--proc_stack = 0xFFFFF000;  /*ebp*/
    *--proc_stack = 0x0;  /*esi*/
    *--proc_stack = 0x0;  /*edi*/
    *--proc_stack = (uint32_t)&int_20_timer_end;

    switch_pd(cur_pd);
    proc->context.esp = (uint32_t)proc_stack;
}

int _sys_fork(int32_t _1, int32_t _2, int32_t _3) {
    disable_interrupt();

    uint32_t proc_struct_page_no = alloc_page(1024, num_of_page);
    mm_mmap(current_process->page_dir,
            proc_struct_page_no, proc_struct_page_no, true, false, true);
    struct process_t *proc = (struct process_t *)(proc_struct_page_no << 12);

    memory_set(proc, 0, sizeof(struct process_t));

    proc->id = next_pid++;
    proc->parent_id = current_process->id;
    proc->next = NULL;
    proc->status = READY;

    proc->page_dir = copy_pd_and_pts(current_process->page_dir);

    proc->page_no = alloc_page(1024, num_of_page);
    mm_mmap(current_process->page_dir, 0x700, proc->page_no,
            true, false, true);
    mm_mmap(proc->page_dir, USER_PROG_ADDR >> 12, proc->page_no,
            true, false, true);
    memory_copy((void *)(0x700 << 12), (void *)(USER_PROG_ADDR), 4096);

    uint32_t stack_no = alloc_page(1024, num_of_page);
    mm_mmap(current_process->page_dir, 0xFFFFE, stack_no, true, false, true);
    mm_mmap(proc->page_dir, 0xFFFFF, stack_no, true, false, true);
    memory_copy((void *)0xFFFFE000, (void *)0xFFFFF000, 4096);

    /* prepare the stack content */
    uint32_t stack_int_frame = 0xFFFFEFFF - (0xFFFFFFFF - stored_esp);
    uint32_t *proc_stack = (uint32_t *)stack_int_frame;
    *--proc_stack = 0x202;  /*eflags*/
    *--proc_stack = 0x8;  /*cs*/
    *--proc_stack = (uint32_t)&sys_fork_end;  /*eip*/
    *--proc_stack = 0;  /*eax, stores the pid of the child*/
    *--proc_stack = 0x0;  /*ecx*/
    *--proc_stack = 0x0;  /*edx*/
    *--proc_stack = 0x0;  /*ebx*/
    *--proc_stack = stored_esp - 4 * 3;  /*esp*/
    *--proc_stack = 0xFFFFF000;  /*ebp*/
    *--proc_stack = 0x0;  /*esi*/
    *--proc_stack = 0x0;  /*edi*/
    *--proc_stack = (uint32_t)&int_20_timer_end;

    proc->context.esp = ((uint32_t)proc_stack - 0xFFFFE000 + 0xFFFFF000);

    mm_mmap(proc->page_dir, proc_struct_page_no, proc_struct_page_no,
            true, false, true);
    add_process(proc);

    enable_interrupt();
    return proc->id;
}

bool add_process(struct process_t *proc) {
    if (process_list[proc->id])
        return false;
    process_list[proc->id] = proc;
    sched_enqueue(&ready_list, proc);
    return true;
}

void init_process_queue(struct process_list_t *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void sched_enqueue(struct process_list_t *q, struct process_t *process) {
    if (q->size == 0)
        q->head = process;
    else
        q->tail->next = process;
    q->tail = process;
    process->next = NULL;
    q->size += 1;
}

struct process_t *sched_serve(struct process_list_t *q) {
    struct process_t *res = NULL;
    if (q->size) {
        res = q->head;
        q->head = res->next;
        res->next = NULL;
        --q->size;
    }
    return res;
}

__attribute__((noreturn))
int sys_exit(int32_t code, int32_t _2, int32_t _3) {
    /* TODO: free page constructs */
    current_process = NULL;
    enable_interrupt();
    while (1)
        ;
}

