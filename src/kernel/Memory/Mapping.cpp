#include "../../../h/kernel/Memory/Mapping.h"
#include "../../../h/kernel/Memory/BuddyAllocator.h"

namespace kernel::memory
{
    void* memset(void* dst, int c, size_t n)
    {
        auto cdst = (char*)dst;
        for (size_t i = 0; i < n; cdst[i++] = c);
        return dst;
    }

    pagetable_t kvmmake()
    {
        auto pagetable = (pagetable_t)page_alloc(0);
        memset(pagetable, 0, PAGE_SIZE);

        /* Map memory need for peripherals */
        kvmmap(pagetable, UART0, UART0, PAGE_SIZE, PTE_R | PTE_W);
        kvmmap(pagetable, VIRTIO0, VIRTIO0, PAGE_SIZE, PTE_R | PTE_W);
        kvmmap(pagetable, PLIC, PLIC, 0x400000, PTE_R | PTE_W);

        kvmdmap(pagetable, KernelCode(), PTE_R | PTE_X);
        kvmdmap(pagetable, SysCallCode(), PTE_R | PTE_X); // Hmmm?
        kvmdmap(pagetable, UserCode(), PTE_R | PTE_X); // Hmmm?

        kvmdmap(pagetable, KernelData(), PTE_R | PTE_W);
        kvmdmap(pagetable, SysCallData(), PTE_R | PTE_W);
        kvmdmap(pagetable, UserData(), PTE_R | PTE_W);

        kvmdmap(pagetable, UserHeap(), PTE_R | PTE_W); // switch to on demand paging
        kvmdmap(pagetable, KernelHeap(), PTE_R | PTE_W);

        return pagetable;
    }

    pagetable_t uvmmake()
    {
        auto pagetable = (pagetable_t)page_alloc(0);
        memset(pagetable, 0, PAGE_SIZE);

        // need to map page with trap handler to separate page
        // so that supervisor can use it in interrupt
        kvmdmap(pagetable, SysCallCode(), PTE_R | PTE_X | PTE_U);
        kvmdmap(pagetable, UserCode(), PTE_R | PTE_X | PTE_U);

        kvmdmap(pagetable, SysCallData(), PTE_R | PTE_W | PTE_U);
        kvmdmap(pagetable, UserData(), PTE_R | PTE_W | PTE_U);
        kvmdmap(pagetable, UserHeap(), PTE_R | PTE_W | PTE_U); // switch to on demand paging    
        // Need to map user stack to separate page as well

        return pagetable;
    }

    int kvmdmap(pagetable_t kpgbl, MemorySection section, int perm)
    {
        return mappages(kpgbl,
            (uint64)section.start,
            section.size(),
            (uint64)section.start,
            perm) != 0;
    }


    int kvmmap(pagetable_t kpgtbl, uint64 va, uint64 pa, uint64 sz, int perm)
    {
        return mappages(kpgtbl, va, sz, pa, perm) != 0;
    }

    // Create PTEs for virtual addresses starting at va that refer to
    // physical addresses starting at pa. va and size might not
    // be page-aligned. Returns 0 on success, -1 if walk() couldn't
    // allocate a needed page-table page.
    int
        mappages(pagetable_t pagetable, uint64 va, uint64 size, uint64 pa, int perm)
    {
        if (size == 0) return -1;

        pte_t* pte;
        uint64 a = PGROUNDDOWN(va);
        uint64 last = PGROUNDDOWN(va + size - 1);

        while (true)
        {
            // Nadji page table entry za trenutnu adresu
            // Ako nije pronadjeana - nije uspelo alociranje
            // Neuspeva mapiranje stranica
            pte = walk(pagetable, a, 1);
            if (pte == 0) return -1;
            // Ako stranica na datom nivou nije validna ne uspeva
            // Mapiranje
            if (*pte & PTE_V) return -1;

            // Prevedi fizicku adresu u page table entry
            *pte = PA2PTE(pa) | perm | PTE_V;

            if (a == last) break;

            a += PAGE_SIZE;
            pa += PAGE_SIZE;
        }
        return 0;
    }

    // Return the address of the PTE in page table pagetable
// that corresponds to virtual address va.  If alloc!=0,
// create any required page-table pages.
//
// The risc-v Sv39 scheme has three levels of page-table
// pages. A page-table page contains 512 64-bit PTEs.
// A 64-bit virtual address is split into five fields:
//   39..63 -- must be zero.
//   30..38 -- 9 bits of level-2 index.
//   21..29 -- 9 bits of level-1 index.
//   12..20 -- 9 bits of level-0 index.
//    0..11 -- 12 bits of byte offset within the page.
    pte_t* walk(pagetable_t pagetable, uint64 va, int alloc)
    {
        if (va >= MAXVA) return nullptr;

        for (int level = 2; level > 0; level--)
        {
            // Iz VA izvuci VPN[level]
            pte_t* pte = &pagetable[PX(level, va)];
            // Ako je list
            if (*pte & PTE_V)
            {
                pagetable = (pagetable_t)PTE2PA(*pte); // izvdoj fizicku adersu strane
            }
            else // Nije list
            {
                // Ako je pozvato za alokaciju probaj alocirati stranu
                if (!alloc || (pagetable = (pde_t*)page_alloc(0)) == 0)
                    return 0;
                // ako je alokacija uspesna, inicijalizuj je
                memset(pagetable, 0, PAGE_SIZE);
                // Upisi u mesto u tabeli pokazivac na
                // Alociranu stranicu i postavi ti je kao validnu
                *pte = PA2PTE(pagetable) | PTE_V;
            }
            // Nastavi sa prolascima po stranici sledeceg nivoa
        }
        // Vrati pokazivac na ulaz u kome se nalazi VPN[0]
        return &pagetable[PX(0, va)];
    }

    int convert_user(pagetable_t pagetable, uint64 va, int user)
    {
        if (va >= MAXVA) return -1;
        for (int level = 2; level >= 0; level--)
        {
            pte_t* pte = &pagetable[PX(level, va)];
            if (~*pte & PTE_V) return -1;

            *pte = (*pte & ~(1 << PTE_U)) | (user << PTE_U);

            pagetable = (pagetable_t)PTE2PA(*pte);
        }
        return 0;
    }


} // kernel::memory