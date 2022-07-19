//
// Created by os on 7/16/22.
//

#ifndef PROJECT_REGISTERUTILS_H
#define PROJECT_REGISTERUTILS_H

#define WRITE_TO_REGISTER(reg, variable) asm volatile("mv " #reg ", %0"::"r"((variable)))
#define READ_FROM_REGISTER(reg, variable) asm volatile("mv %0, "#reg:"=r"((variable)))

#define WRITE_TO_SYS_REGISTER(reg, variable) asm volatile("csrw " #reg ", %0"::"r"((variable)))
#define READ_FROM_SYS_REGISTER(reg, variable)  asm volatile("csrr %0, "#reg:"=r"((variable)))

#define SYS_REGISTER_SET_BITS(reg, mask) asm volatile("csrs "#reg ", %0"::"r"((mask)))
#define SYS_REGISTER_CLEAR_BITS(reg, mask) asm volatile("csrc "#reg ", %0"::"r"((mask)))

#endif //PROJECT_REGISTERUTILS_H

