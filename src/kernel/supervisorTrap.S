.extern __kernelStack

.extern __kernel_vt
.extern __user_vt

# Thread::runningThread
.extern __runningThread

.set pcOffsetThread, 0x00
.set sstatusOffsetThread, 0x08
.set registersOffsetThread, 0x10

# void kernel::TrapHandlers::supervisorTrapHandle();
.extern __supervisorTrapHandle__
.type __supervisorTrapHandle__, @function

# void kernel::TrapHandlers::supervisorTrap()
.global __supervisorTrap__
.type __supervisorTrap__, @function
.align 4
__supervisorTrap__:
    csrw sscratch, t6
    ld t6, __runningThread
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
        sd x\index, registersOffsetThread + \index * 8 (t6)
    .endr
    csrr t5, sscratch
    sd t5, registersOffsetThread + 31 * 8(t6)
    
    csrr t5, sepc
    sd t5, pcOffsetThread(t6)
    csrr t5, sstatus
    sd t5, sstatusOffsetThread(t6)
    ld sp, __kernelStack

    # calculate mask for SPP bit
    li t0, 1
    slli t0, t0, 8
    # read sstatus
    csrr t1, sstatus
    # mask off all but SPP
    and t0, t0, t1
    li t1, 0
    #// if it is not set we are in privileged mode and we don't swap tables
    bne t0, t1, handle
    // if we are in user mode we need to swap page tables
    ld t0, __kernel_vt
    sfence.vma zero, zero
    csrw satp,t0
    sfence.vma zero, zero

handle:
    call __supervisorTrapHandle__

    ld t6, __runningThread
    ld t5, pcOffsetThread(t6)
    csrw sepc, t5
    ld t5, sstatusOffsetThread(t6)
    csrw sstatus, t5

    # calculate mask for SPP bit
    li t0, 1
    slli t0, t0, 8
    # read sstatus
    csrr t1, sstatus
    # mask off all but SPP
    and t0, t0, t1
    li t1, 0
    #// if it is not set we are in privileged mode and we don't swap tables
    bne t0, t1, goback
    // if we are in user mode we need to swap page tables
    ld t0, __user_vt
    sfence.vma zero, zero
    csrw satp,t0
    sfence.vma zero, zero
    

goback:  
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, registersOffsetThread + \index * 8 (t6)
    .endr

    sret