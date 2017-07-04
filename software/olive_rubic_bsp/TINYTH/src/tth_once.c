#include <priv/tth_core.h>
#if (TTHREAD_ENABLE_ONCE != 0)

#if (TTHREAD_ENABLE_MUTEX == 0)
#error "TTHREAD_ENABLE_ONCE must be enabled with TTHREAD_ENABLE_MUTEX"
#endif

/*
 * [POSIX.1-2001]
 * Ensure only once initialization call
 */
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
  int result;

  if (once_control->__priv.done)
  {
    return 0;
  }

  result = pthread_mutex_lock(&once_control->__priv.mutex);
  if (result != 0)
  {
    return result;
  }

  if (!once_control->__priv.done)
  {
    (*init_routine)();

    once_control->__priv.done = 1;
  }

  return pthread_mutex_unlock(&once_control->__priv.mutex);
}

#endif  /* TTHREAD_ENABLE_ONCE */
/* vim: set et sts=2 sw=2: */
