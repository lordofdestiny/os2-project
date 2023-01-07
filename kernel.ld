OUTPUT_ARCH( "riscv" )
ENTRY( _entry )

SECTIONS
{
  /*
   * ensure that entry.S / _entry is at 0x80000000,
   * where qemu's -kernel jumps.
   */
  . = 0x80000000;

  PROVIDE(begin = .);
  .text : {
      PROVIDE(kernel_text_start = .);
    *(.entry_os) . = ALIGN(0x1000);
    PROVIDE(kerel_text_start = .);
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o ) *(.text .text*)
    . = ALIGN(0x1000);
  }
  PROVIDE(kernel_text_end = .);

  PROVIDE(kernel_data_start = . );
  .rodata : {
    . = ALIGN(16);
    /* do not need to distinguish this from .rodata */
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o ) *(.srodata .srodata.*) 
    . = ALIGN(16);
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o ) *(.rodata .rodata.*)
  }

  .data : {
    . = ALIGN(16);
    /* do not need to distinguish this from .data */
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o ) *(.sdata .sdata.*) 
    . = ALIGN(16);
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o ) *(.data .data.*)
  }

  .bss : {
    . = ALIGN(16);
   /* do not need to distinguish this from .bss */
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o )  *(.sbss .sbss.*) 
    . = ALIGN(16);
    EXCLUDE_FILE (*tests/*.o */syscall_c.o */syscall_cpp.o ) *(.bss .bss.*)
  }
  /* Aling user code to next page */
  . = ALIGN(0x1000) ;
  PROVIDE(kernel_data_end = . );
  PROVIDE(kernel_end = . );


  PROVIDE(syscall_start = .);
  .text.syscall : {
    . = ALIGN(0x1000);
    PROVIDE(syscall_text_start = .);
    EXCLUDE_FILE (*tests/*.o) *(.text .text*)
    . = ALIGN(0x1000);
  }
  PROVIDE(syscall_text_end = .);

  PROVIDE(syscall_data_start = . );
  .rodata.syscall : {
    . = ALIGN(16);
    /* do not need to distinguish this from .rodata */
    EXCLUDE_FILE (*tests/*.o) *(.srodata .srodata.*) 
    . = ALIGN(16);
    EXCLUDE_FILE (*tests/*.o) *(.rodata .rodata.*)
  }

  .data.syscall : {
    . = ALIGN(16);
    /* do not need to distinguish this from .data */
    EXCLUDE_FILE (*tests/*.o) *(.sdata .sdata.* ) 
    . = ALIGN(16);
    EXCLUDE_FILE (*tests/*.o) *(.data .data.* )
  }

  .bss.syscall : {
    . = ALIGN(16);
   /* do not need to distinguish this from .bss */
   EXCLUDE_FILE (*tests/*.o)  *(.sbss .sbss.* ) 
    . = ALIGN(16);
    EXCLUDE_FILE (*tests/*.o) *(.bss .bss.* )
  }
  . = ALIGN(0x1000) ;
  PROVIDE(syscall_data_end = . );
  PROVIDE(syscall_end = .);
  

  PROVIDE(user_start = . );
  .text.user : {
    PROVIDE(user_code_start = . );
    *(*/tests/*.o)  *(.text .text*)
    . = ALIGN(0x1000) ;
    PROVIDE(user_code_end = . );
  }

  PROVIDE(user_data_start = . );
  .rodata.user : {
    . = ALIGN(16);
    *(*/tests/*.o) *(.srodata .srodata.*)
    . = ALIGN(16);
    *(*/tests/*.o) *(.rodata .rodata.*)
  }
  
  .data.user : {
    . = ALIGN(16);
    *(*/tests/*.o) *(.sdata .sdata.*)
    . = ALIGN(16);
    *(*/tests/*.o) *(.data .data.*)
  }

  .bss.user :{
    . = ALIGN(16);
    *(*/tests/*.o) *(.sbss .sbss.*)
    . = ALIGN(16);
    *(*/tests/*.o) *(.bss .bss.*)
  }
  . = ALIGN(0x1000);
  PROVIDE(user_data_end = .);
  PROVIDE(user_end = .);

  PROVIDE(end = .);
}
