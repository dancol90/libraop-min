#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void* pthread_t;
typedef void* pthread_mutex_t;

int pthread_mutex_trylock(pthread_mutex_t* mutex);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_mutex_init(pthread_mutex_t* mutex, const void* attr);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
int pthread_create(pthread_t* thread, const void* attr, void* (*start_routine) (void*), void* arg);
int pthread_join(pthread_t thread, void** retval);

#ifdef __cplusplus
}
#endif