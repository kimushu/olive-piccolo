#ifndef __PRIV_TTH_ARCH_NIOS2_H__
#define __PRIV_TTH_ARCH_NIOS2_H__

#include <stdint.h>
#include <system.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Prototype for inline functions */
static inline void tth_crash(void) __attribute__((always_inline));
static inline int tth_cs_begin(void) __attribute__((always_inline));
static inline void tth_cs_end(int status) __attribute__((always_inline));
static inline void tth_cs_exec_switch(void) __attribute__((always_inline));

/* Crash system */
static inline void tth_crash(void)
{
  __builtin_wrctl(0, 0);
  for (;;)
  {
    __asm__("break");
  }
}

/*
 * Begin critical section
 */
static inline int tth_cs_begin(void)
{
  int status;
  status = __builtin_rdctl(0); /* status */
  __builtin_wrctl(0, status & ~(1u << 0));
  return status;
}

/*
 * End critical section
 */
static inline void tth_cs_end(int status)
{
  __builtin_wrctl(0, status);
}

/*
 * Execute thread switching
 */
static inline void tth_cs_exec_switch(void)
{
  /*
   * Issue "trap <imm5>" instruction
   * 24 means 24th alphabet "T" -- the first letter of TinyThreads.
   */
  __asm__ volatile("trap 24");
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* __PRIV_TTH_ARCH_NIOS2_H__ */
/* vim: set et sts=2 sw=2: */
