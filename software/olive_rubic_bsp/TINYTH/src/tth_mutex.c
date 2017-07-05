#include <priv/tth_core.h>
#include <priv/tth_mutex.h>
#if (TTHREAD_ENABLE_MUTEX != 0)

/*
 * [POSIX.1-2001]
 * Destroy a mutex
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
  if (mutex->__priv.waiter || mutex->__priv.owner)
  {
    return EBUSY;
  }

  return 0;
}

/*
 * [POSIX.1-2001]
 * Initialize a mutex
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
  static const pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;

  *mutex = init_mutex;
  (void)attr;

  return 0;
}

/*
 * [POSIX.1-2001]
 * Lock a mutex
 */
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
  int lock = tth_cs_begin();
  int result = tth_cs_mutex_lock(mutex, 1);
  tth_cs_end(lock);
  return result;
}

/*
 * [POSIX.1-2001]
 * Try to lock a mutex
 */
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
  int lock = tth_cs_begin();
  int result = tth_cs_mutex_lock(mutex, 0);
  tth_cs_end(lock);
  return result;
}

/*
 * [POSIX.1-2001]
 * Unlock a mutex
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
  int lock = tth_cs_begin();
  int result = tth_cs_mutex_unlock(mutex);
  if (result == 0)
  {
    tth_cs_switch();
  }
  tth_cs_end(lock);
  return result;
}

#endif  /* TTHREAD_ENABLE_MUTEX */
/* vim: set et sts=2 sw=2: */
