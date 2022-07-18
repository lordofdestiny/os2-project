# void kernel::supervisorTrapHandle();
.extern _ZN6kernel11TrapHandler20supervisorTrapHandleEv
.set supervisorTrapHandle, _ZN6kernel11TrapHandler20supervisorTrapHandleEv
.type _ZN6kernel11TrapHandler20supervisorTrapHandleEv, @function


.align 16
.space 0x100 * 8
systemStack:

# TCB::runningThread
.extern _ZN6kernel3TCB13runningThreadE
.set runningThread, _ZN6kernel3TCB13runningThreadE
.set offsetTCBpc, 0x00
.set offsetTCBsstatus, 0x08
.set offsetTCBregisters, 0x10

# void kernel::supervisorTrap()
.global _ZN6kernel11TrapHandler14supervisorTrapEv
.type _ZN6kernel11TrapHandler14supervisorTrapEv, @function
.align 4
_ZN6kernel11TrapHandler14supervisorTrapEv:
    csrw sscratch, t6
    ld t6, runningThread
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30
        sd x\index, offsetTCBregisters + \index * 8 (t6)
    .endr
    csrr t5, sscratch
    sd t5, offsetTCBregisters + 31 * 8(t6)
    csrr t5, sepc
    sd t5, offsetTCBpc(t6)
    csrr t5, sstatus
    sd t5, offsetTCBsstatus(t6)
    la sp, systemStack

    call supervisorTrapHandle

    ld t6, runningThread
    ld t5, offsetTCBpc(t6)
    csrw sepc, t5
    ld t5, offsetTCBsstatus(t6)
    csrw sstatus, t5
    .irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, offsetTCBregisters + \index * 8 (t6)
    .endr

    sret


