.extern handleSupervisorTrap
.global supervisorTrap
.align 4
.global incrementSEPC

supervisorTrap:
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

incrementSEPC:
    csrr t0, sepc
    addi t0, t0, 4
    csrw sepc, t0
    ret



