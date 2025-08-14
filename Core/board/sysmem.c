/* Includes */
#include <stdint.h>
#include <errno.h>
#include <stddef.h>

/* 当前堆顶指针 */
static uint8_t *__sbrk_heap_end = NULL;

/**
 * @brief MCU 端 _sbrk 函数，用于 malloc 等函数分配内存
 *
 * 堆从链接脚本中定义的 __HeapBase 开始，最大不能超过 __HeapLimit
 *
 * linker script 中需要定义：
 *   __HeapBase  - 堆起始地址
 *   __HeapLimit - 堆最大地址
 *
 * @param incr 分配内存大小（字节）
 * @return 分配成功返回内存起始地址，失败返回 (void*)-1 并设置 errno
 */
void *_sbrk(ptrdiff_t incr)
{
  extern uint8_t __HeapBase;   /* 链接脚本定义的堆起始 */
  extern uint8_t __HeapLimit;  /* 链接脚本定义的堆最大地址 */

  uint8_t *prev_heap_end;

  /* 第一次调用初始化堆顶 */
  if (__sbrk_heap_end == NULL)
  {
    __sbrk_heap_end = &__HeapBase;
  }

  /* 检查是否超出堆上限 */
  if ((uintptr_t)(__sbrk_heap_end + incr) > (uintptr_t)&__HeapLimit)
  {
    errno = ENOMEM; /* 内存不足 */
    return (void *)-1;
  }

  /* 分配内存 */
  prev_heap_end = __sbrk_heap_end;
  __sbrk_heap_end += incr;

  return (void *)prev_heap_end;
}
