//
// Created by os on 7/16/22.
//

#ifndef PROJECT_REGISTERUTILS_H
#define PROJECT_REGISTERUTILS_H

#define REGISTER_WRITE(reg, variable) asm volatile("mv " #reg ", %0"::"r"((variable)))
#define REGISTER_READ(reg, variable) asm volatile("mv %0, "#reg:"=r"((variable)))

#define SREGISTER_WRITE(reg, variable) asm volatile("csrw " #reg ", %0"::"r"((variable)))
#define SREGISTER_READ(reg, variable)  asm volatile("csrr %0, "#reg:"=r"((variable)))

#define SREGISTER_SET_BITS(reg, mask) asm volatile("csrs "#reg ", %0"::"r"((mask)))
#define SREGISTER_CLEAR_BITS(reg, mask) asm volatile("csrc "#reg ", %0"::"r"((mask)))

#endif //PROJECT_REGISTERUTILS_H

