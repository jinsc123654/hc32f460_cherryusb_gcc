/******************************************************************************
 * HC32F460xE.ld - Linker script for HC32F460 Device
 * FLASH limit: 32KB
 * FLASH_ARRAY: 480KB (可放置固定数组)
 * RAM: 188KB
 * RET_RAM: 4KB
 *****************************************************************************/

/* OTP definitions (固定) */
__OTP_DATA_START = 0x03000C00;
__OTP_DATA_SIZE  = 64;
__OTP_LOCK_START = 0x03000FC0;
__OTP_LOCK_SIZE  = 4;

/* 内存布局说明 */
MEMORY
{
    /* 主FLASH，用于程序代码和只读数据，限制32KB */
    FLASH       (rx)  : ORIGIN = 0x00000000, LENGTH = 32K

    /* 后续FLASH区域，可放置数组等固定数据 */
    FLASH_ARRAY (rx)  : ORIGIN = 0x00008000, LENGTH = 480K

    /* OTP 数据段 */
    OTP_DATA    (rx)  : ORIGIN = __OTP_DATA_START, LENGTH = __OTP_DATA_SIZE

    /* OTP 锁定段 */
    OTP_LOCK    (rx)  : ORIGIN = __OTP_LOCK_START, LENGTH = __OTP_LOCK_SIZE

    /* 内部 SRAM 主段 */
    RAM         (rwx) : ORIGIN = 0x1FFF8000, LENGTH = 188K

    /* RET_RAM 用于高速运行数据 */
    RET_RAM     (rwx) : ORIGIN = 0x200F0000, LENGTH = 4K
}

/* 复位入口 */
ENTRY(Reset_Handler)

SECTIONS
{
    /* 中断向量表 */
    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
        . = ALIGN(4);
    } >FLASH

    /* ICG 配置段 */
    .icg_sec 0x00000400 :
    {
        KEEP(*(.icg_sec))
    } >FLASH

    /* 代码段 */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        KEEP(*(.init))
        KEEP(*(.fini))
        . = ALIGN(4);
    } >FLASH

    /* 只读数据段 */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /* ARM 异常表 */
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } >FLASH
    __exidx_end = .;

    /* 初始化数组 */
    .preinit_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(4);
    } >FLASH

    .init_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(4);
    } >FLASH

    .fini_array :
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(4);
    } >FLASH

    /* 程序结束地址 */
    __etext = ALIGN(4);

    /* OTP 数据段 */
    .otp_data_sec :
    {
        KEEP(*(.otp_data_sec))
    } >OTP_DATA

    .otp_lock_sec :
    {
        KEEP(*(.otp_lock_sec))
    } >OTP_LOCK

    /* RAM 数据段 */
    .data : AT (__etext)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(vtable)
        *(.data)
        *(.data*)
        . = ALIGN(4);
        *(.ramfunc)
        *(.ramfunc*)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM

    __etext_ret_ram = __etext + ALIGN(SIZEOF(.data),4);

    /* RET_RAM 数据段 */
    .ret_ram_data : AT (__etext_ret_ram)
    {
        . = ALIGN(4);
        __data_start_ret_ram__ = .;
        *(.ret_ram_data)
        *(.ret_ram_data*)
        . = ALIGN(4);
        __data_end_ret_ram__ = .;
    } >RET_RAM

    /* FLASH_ARRAY 子段，用于固定数组 */
    .flash_array_sub 0x00008000 :
    {
        KEEP(*(.flash_array))
    } >FLASH_ARRAY

    /* BSS 段 */
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
        . = ALIGN(4);
        *(.noinit*)
        . = ALIGN(4);
    } >RAM

    /* RET_RAM BSS 段 */
    .ret_ram_bss :
    {
        . = ALIGN(4);
        __bss_start_ret_ram__ = .;
        *(.ret_ram_bss)
        *(.ret_ram_bss*)
        . = ALIGN(4);
        __bss_end_ret_ram__ = .;
    } >RET_RAM

    /* 堆栈段 */
    .heap_stack (COPY) :
    {
        . = ALIGN(8);
        __end__ = .;
        PROVIDE(end = .);
        PROVIDE(_end = .);
        *(.heap*)
        . = ALIGN(8);
        __HeapLimit = .;
        __StackLimit = .;
        *(.stack*)
        . = ALIGN(8);
        __StackTop = .;
    } >RAM

    /* 库文件丢弃 */
    /DISCARD/ :
    {
        libc.a (*)
        libm.a (*)
        libgcc.a (*)
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }

    PROVIDE(_stack = __StackTop);
    PROVIDE(_Min_Heap_Size = __HeapLimit - __HeapBase);
    PROVIDE(_Min_Stack_Size = __StackTop - __StackLimit);

    __RamEnd = ORIGIN(RAM) + LENGTH(RAM);
    ASSERT(__StackTop <= __RamEnd, "region RAM overflowed with stack")
}
