# void kernel::supervisorTrapHandle();
.extern _ZN6kernel11TrapHandler20supervisorTrapHandleEv
.set supervisorTrapHandle, _ZN6kernel11TrapHandler20supervisorTrapHandleEv
.type _ZN6kernel11TrapHandler20supervisorTrapHandleEv, @function


.align 16
.space 0x100 * 8
kernel_stack:

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
    # Store registers
    addi sp, sp, -256
    .irp index 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
         sd x\index, \index * 8(sp)
    .endr

    csrw sscratch,sp
    call supervisorTrapHandle

    # Restore registers
    .irp index 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
         ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret


