#include <thread>
#include <mutex>

#include "pthread.h"

extern "C"
{

  int pthread_mutex_trylock(pthread_mutex_t* mutex)
  {
    return reinterpret_cast<std::mutex*>(*mutex)->try_lock() ? 0 : 1;
  }

  int pthread_mutex_lock(pthread_mutex_t* mutex)
  {
    if (!mutex)
      return 1;

    reinterpret_cast<std::mutex*>(*mutex)->lock();
    return 0;
  }

  int pthread_mutex_unlock(pthread_mutex_t* mutex)
  {
    if (!mutex)
      return 1;
    reinterpret_cast<std::mutex*>(*mutex)->unlock();
    return 0;
  }

  int pthread_mutex_init(pthread_mutex_t* mutex, const void* attr)
  {
    if (!mutex)
      return 1;
    *mutex = new std::mutex();
  }

  int pthread_mutex_destroy(pthread_mutex_t* mutex)
  {
    if (!mutex)
      return 1;
    delete reinterpret_cast<std::mutex*>(*mutex);
  }


  int pthread_create(pthread_t* thread, const void* attr, void* (*start_routine) (void*), void* arg)
  {
    if (!thread)
      return 1;

    *thread = new std::thread([start_routine, arg]() { start_routine(arg); });
    return 0;
  }

  int pthread_join(pthread_t thread, void** retval)
  {
    if (!thread)
      return 1;

    std::thread* cppthread = reinterpret_cast<std::thread*>(thread);
    if (cppthread->joinable())
      cppthread->join();

    return 0;
  }

}