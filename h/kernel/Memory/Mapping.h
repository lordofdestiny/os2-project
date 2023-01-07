#include "./slab.h"
#include "./MemoryManager.h"

/* Physical memory layout
 * qemu -machine virt is set up like this,
 * based on qemu's hw/riscv/virt.c:
 *
 * 0000'1000 -- boot ROM, provided by qemu
 * 0200'0000 -- CLINT
 * 0C00'0000 -- PLIC
 * 1000'0000 -- uart0
 * 1000'1000 -- virtio disk
 * 8000'0000 -- boot ROM jumps here in machine mode
 *             -kernel loads the kernel here
 *
 * unused RAM after 80000000 KERNBASE
 * begin is at 8000'0000, kernel code starts here
 * kernel_text_start - same as begin
 * kernel_text_end
 *
 * krenel_data_start - next page after kernel_text_end
 * kernel_data_end
 *
 * user_text_start - next page after kernel_data_end
 * user_text_end
 *
 * user_data_start - next page afte user_text_start
 * user_data_end
 *
 * HEAP STARTS AFTER user_data_end
 * UNTIL 8700'0000
 *
 * 8700'0000
 * 8800'0000 PHYSTOP, end of kernel memory
 */

extern char begin[];

extern char kernel_text_start[];
extern char kernel_text_end[];

extern char kernel_data_start[];
extern char kernel_data_end[];

extern char kernel_end[];

extern char syscall_text_start[];
extern char syscall_text_end[];

extern char syscall_data_start[];
extern char syscall_data_end[];

extern char user_start[];

extern char user_code_start[];
extern char user_code_end[];

extern char user_data_start[];
extern char user_data_end[];

extern char user_end[];

extern char end[];

/* Required in order to allow the kernel
 * to acces the memory space of the xv6
 * platofm on top of which it is built
 * */
#define UART0 0x10000000L
#define UART0_IRQ 10
#define VIRTIO0 0x10001000
#define VIRTIO0_IRQ 1
#define PLIC 0x0c000000L

#define KERNBASE (uint64)0x80000000ull
#define PHYSTOP (uint64)0x88000000ull
#define MEM_SIZE (PHYSTOP- KERNBASE)

#define PGROUNDUP(sz)  (((sz)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PAGE_SIZE-1))

 // PTE control bits
#define PTE_V (1L << 0) // valid
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4) // user can access
#define PTE_G (1 << 5)

/*
 * Tramslation between physical address and PTE entry
 * - PA2PTE strips page offsets and shifts the page number
 * so that it fits into PPN bits of PTE
 * - PTE2PA stripts bit flags from PTE and shifts the page
 * number so that it is now valid physical address
 **/
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)
#define PTE2PA(pte) (((pte) >> 10) << 12)

 // Extracts lower 10 bits of the PTE
#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// Virtual address mode - 39bit VA is used
#define SATP_V39 (8ull << 60)
// Prepares root page table to be written into SATP
#define MAKE_SATP(pagetable) (SATP_V39 | (((uint64)pagetable)>>12))

/*
 * extract the three 9-bit page table indices from a virtual address.
 * PXSHIFT makes a mask for extracting VPN[level]
 * PX extracts PVN out of the virtual address
 **/
#define PXMASK          0x1FF // 9 bits
#define PXSHIFT(level)  (PAGE_ORDER+(9*(level)))
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

 // one beyond the highest possible virtual address.
 // MAXVA is actually one bit less than the max allowed by
 // Sv39, to avoid having to sign-extend virtual addresses
 // that have the high bit set.
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

namespace kernel::memory
{
    using pagetable_t = uint64*;
    using pte_t = uint64;
    using pde_t = uint64;


    void* kpgalloc();
    void kpgfree(void* pg);
    void* memset(void* dst, int c, size_t n);
    int kvmdmap(pagetable_t kpgbl, MemorySection section, int perm);
    int kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm);
    int mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm);
    pte_t* walk(pagetable_t pagetable, uint64 va, int alloc);
    int convert_user(pagetable_t pagetable, uint64 va, int user);
    pagetable_t kvmmake();
    pagetable_t uvmmake();
} // kernel::memory