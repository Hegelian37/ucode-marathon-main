/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_internal.h"

/* System independent thread management routines for SDL */

#include "SDL_thread_c.h"
#include "SDL_systhread.h"
#include "../SDL_error_c.h"

SDL_TLSID SDL_CreateTLS(void)
{
    static SDL_AtomicInt SDL_tls_id;
    return (SDL_TLSID)(SDL_AtomicIncRef(&SDL_tls_id) + 1);
}

void *SDL_GetTLS(SDL_TLSID id)
{
    SDL_TLSData *storage;

    storage = SDL_SYS_GetTLSData(void);
    if (!storage || id == 0 || id > storage->limit) {
        return NULL;
    }
    return storage->array[id - 1].data;
}

int SDL_SetTLS(SDL_TLSID id, const void *value, void(SDLCALL *destructor)(void *))
{
    SDL_TLSData *storage;

    if (id == 0) {
        return SDL_InvalidParamError("id");
    }

    storage = SDL_SYS_GetTLSData(void);
    if (!storage || (id > storage->limit)) {
        unsigned int i, oldlimit, newlimit;
        SDL_TLSData *new_storage;

        oldlimit = storage ? storage->limit : 0;
        newlimit = (id + TLS_ALLOC_CHUNKSIZE);
        new_storage = (SDL_TLSData *)SDL_realloc(storage, sizeof(*storage) + (newlimit - 1) * sizeof(storage->array[0]));
        if (!new_storage) {
            SDL_OutOfMemory(void);
            return -1;
        }
        storage = new_storage;
        storage->limit = newlimit;
        for (i = oldlimit; i < newlimit; ++i) {
            storage->array[i].data = NULL;
            storage->array[i].destructor = NULL;
        }
        if (SDL_SYS_SetTLSData(storage) != 0) {
            return -1;
        }
    }

    storage->array[id - 1].data = SDL_const_cast(void *, value);
    storage->array[id - 1].destructor = destructor;
    return 0;
}

void SDL_CleanupTLS(void)
{
    SDL_TLSData *storage;

    storage = SDL_SYS_GetTLSData(void);
    if (storage) {
        unsigned int i;
        for (i = 0; i < storage->limit; ++i) {
            if (storage->array[i].destructor) {
                storage->array[i].destructor(storage->array[i].data);
            }
        }
        SDL_SYS_SetTLSData(NULL);
        SDL_free(storage);
    }
}

/* This is a generic implementation of thread-local storage which doesn't
   require additional OS support.

   It is not especially efficient and doesn't clean up thread-local storage
   as threads exit.  If there is a real OS that doesn't support thread-local
   storage this implementation should be improved to be production quality.
*/

typedef struct SDL_TLSEntry
{
    SDL_ThreadID thread;
    SDL_TLSData *storage;
    struct SDL_TLSEntry *next;
} SDL_TLSEntry;

static SDL_Mutex *SDL_generic_TLS_mutex;
static SDL_TLSEntry *SDL_generic_TLS;

SDL_TLSData *SDL_Generic_GetTLSData(void)
{
    SDL_ThreadID thread = SDL_GetCurrentThreadID(void);
    SDL_TLSEntry *entry;
    SDL_TLSData *storage = NULL;

#ifndef SDL_THREADS_DISABLED
    if (!SDL_generic_TLS_mutex) {
        static SDL_SpinLock tls_lock;
        SDL_LockSpinlock(&tls_lock);
        if (!SDL_generic_TLS_mutex) {
            SDL_Mutex *mutex = SDL_CreateMutex(void);
            SDL_MemoryBarrierRelease(void);
            SDL_generic_TLS_mutex = mutex;
            if (!SDL_generic_TLS_mutex) {
                SDL_UnlockSpinlock(&tls_lock);
                return NULL;
            }
        }
        SDL_UnlockSpinlock(&tls_lock);
    }
    SDL_MemoryBarrierAcquire(void);
    SDL_LockMutex(SDL_generic_TLS_mutex);
#endif /* SDL_THREADS_DISABLED */

    for (entry = SDL_generic_TLS; entry; entry = entry->next) {
        if (entry->thread == thread) {
            storage = entry->storage;
            break;
        }
    }
#ifndef SDL_THREADS_DISABLED
    SDL_UnlockMutex(SDL_generic_TLS_mutex);
#endif

    return storage;
}

int SDL_Generic_SetTLSData(SDL_TLSData *data)
{
    SDL_ThreadID thread = SDL_GetCurrentThreadID(void);
    SDL_TLSEntry *prev, *entry;

    /* SDL_Generic_GetTLSData(void) is always called first, so we can assume SDL_generic_TLS_mutex */
    SDL_LockMutex(SDL_generic_TLS_mutex);
    prev = NULL;
    for (entry = SDL_generic_TLS; entry; entry = entry->next) {
        if (entry->thread == thread) {
            if (data) {
                entry->storage = data;
            } else {
                if (prev) {
                    prev->next = entry->next;
                } else {
                    SDL_generic_TLS = entry->next;
                }
                SDL_free(entry);
            }
            break;
        }
        prev = entry;
    }
    if (!entry) {
        entry = (SDL_TLSEntry *)SDL_malloc(sizeof(*entry));
        if (entry) {
            entry->thread = thread;
            entry->storage = data;
            entry->next = SDL_generic_TLS;
            SDL_generic_TLS = entry;
        }
    }
    SDL_UnlockMutex(SDL_generic_TLS_mutex);

    return entry ? 0 : -1;
}

/* Non-thread-safe global error variable */
static SDL_error *SDL_GetStaticErrBuf(void)
{
    static SDL_error SDL_global_error;
    static char SDL_global_error_str[128];
    SDL_global_error.str = SDL_global_error_str;
    SDL_global_error.len = sizeof(SDL_global_error_str);
    return &SDL_global_error;
}

#ifndef SDL_THREADS_DISABLED
static void SDLCALL SDL_FreeErrBuf(void *data)
{
    SDL_error *errbuf = (SDL_error *)data;

    if (errbuf->str) {
        errbuf->free_func(errbuf->str);
    }
    errbuf->free_func(errbuf);
}
#endif

/* Routine to get the thread-specific error variable */
SDL_error *SDL_GetErrBuf(SDL_bool create)
{
#ifdef SDL_THREADS_DISABLED
    return SDL_GetStaticErrBuf(void);
#else
    static SDL_SpinLock tls_lock;
    static SDL_bool tls_being_created;
    static SDL_TLSID tls_errbuf;
    const SDL_error *ALLOCATION_IN_PROGRESS = (SDL_error *)-1;
    SDL_error *errbuf;

    if (!tls_errbuf && !create) {
        return NULL;
    }

    /* tls_being_created is there simply to prevent recursion if SDL_CreateTLS(void) fails.
       It also means it's possible for another thread to also use SDL_global_errbuf,
       but that's very unlikely and hopefully won't cause issues.
     */
    if (!tls_errbuf && !tls_being_created) {
        SDL_LockSpinlock(&tls_lock);
        if (!tls_errbuf) {
            SDL_TLSID slot;
            tls_being_created = SDL_TRUE;
            slot = SDL_CreateTLS(void);
            tls_being_created = SDL_FALSE;
            SDL_MemoryBarrierRelease(void);
            tls_errbuf = slot;
        }
        SDL_UnlockSpinlock(&tls_lock);
    }
    if (!tls_errbuf) {
        return SDL_GetStaticErrBuf(void);
    }

    SDL_MemoryBarrierAcquire(void);
    errbuf = (SDL_error *)SDL_GetTLS(tls_errbuf);
    if (errbuf == ALLOCATION_IN_PROGRESS) {
        return SDL_GetStaticErrBuf(void);
    }
    if (!errbuf) {
        /* Get the original memory functions for this allocation because the lifetime
         * of the error buffer may span calls to SDL_SetMemoryFunctions(void) by the app
         */
        SDL_realloc_func realloc_func;
        SDL_free_func free_func;
        SDL_GetOriginalMemoryFunctions(NULL, NULL, &realloc_func, &free_func);

        /* Mark that we're in the middle of allocating our buffer */
        SDL_SetTLS(tls_errbuf, ALLOCATION_IN_PROGRESS, NULL);
        errbuf = (SDL_error *)realloc_func(NULL, sizeof(*errbuf));
        if (!errbuf) {
            SDL_SetTLS(tls_errbuf, NULL, NULL);
            return SDL_GetStaticErrBuf(void);
        }
        SDL_zerop(errbuf);
        errbuf->realloc_func = realloc_func;
        errbuf->free_func = free_func;
        SDL_SetTLS(tls_errbuf, errbuf, SDL_FreeErrBuf);
    }
    return errbuf;
#endif /* SDL_THREADS_DISABLED */
}

void SDL_RunThread(SDL_Thread *thread)
{
    void *userdata = thread->userdata;
    int(SDLCALL * userfunc)(void *) = thread->userfunc;

    int *statusloc = &thread->status;

    /* Perform any system-dependent setup - this function may not fail */
    SDL_SYS_SetupThread(thread->name);

    /* Get the thread id */
    thread->threadid = SDL_GetCurrentThreadID(void);

    /* Run the function */
    *statusloc = userfunc(userdata);

    /* Clean up thread-local storage */
    SDL_CleanupTLS(void);

    /* Mark us as ready to be joined (or detached) */
    if (!SDL_AtomicCompareAndSwap(&thread->state, SDL_THREAD_STATE_ALIVE, SDL_THREAD_STATE_ZOMBIE)) {
        /* Clean up if something already detached us. */
        if (SDL_AtomicCompareAndSwap(&thread->state, SDL_THREAD_STATE_DETACHED, SDL_THREAD_STATE_CLEANED)) {
            if (thread->name) {
                SDL_free(thread->name);
            }
            SDL_free(thread);
        }
    }
}

#ifdef SDL_CreateThread
#undef SDL_CreateThread
#undef SDL_CreateThreadWithStackSize
#endif
#if SDL_DYNAMIC_API
#define SDL_CreateThread              SDL_CreateThread_REAL
#define SDL_CreateThreadWithStackSize SDL_CreateThreadWithStackSize_REAL
#endif

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
SDL_Thread *SDL_CreateThreadWithStackSize(int(SDLCALL *fn)(void *),
                              const char *name, const size_t stacksize, void *data,
                              pfnSDL_CurrentBeginThread pfnBeginThread,
                              pfnSDL_CurrentEndThread pfnEndThread)
#else
SDL_Thread *SDL_CreateThreadWithStackSize(int(SDLCALL *fn)(void *),
                              const char *name, const size_t stacksize, void *data)
#endif
{
    SDL_Thread *thread;
    int ret;

    /* Allocate memory for the thread info structure */
    thread = (SDL_Thread *)SDL_calloc(1, sizeof(*thread));
    if (!thread) {
        return NULL;
    }
    thread->status = -1;
    SDL_AtomicSet(&thread->state, SDL_THREAD_STATE_ALIVE);

    /* Set up the arguments for the thread */
    if (name) {
        thread->name = SDL_strdup(name);
        if (!thread->name) {
            SDL_free(thread);
            return NULL;
        }
    }

    thread->userfunc = fn;
    thread->userdata = data;
    thread->stacksize = stacksize;

    /* Create the thread and go! */
#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
    ret = SDL_SYS_CreateThread(thread, pfnBeginThread, pfnEndThread);
#else
    ret = SDL_SYS_CreateThread(thread);
#endif
    if (ret < 0) {
        /* Oops, failed.  Gotta free everything */
        SDL_free(thread->name);
        SDL_free(thread);
        thread = NULL;
    }

    /* Everything is running now */
    return thread;
}

#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
DECLSPEC SDL_Thread *SDLCALL SDL_CreateThread(int(SDLCALL *fn)(void *),
                 const char *name, void *data,
                 pfnSDL_CurrentBeginThread pfnBeginThread,
                 pfnSDL_CurrentEndThread pfnEndThread)
#else
DECLSPEC SDL_Thread *SDLCALL SDL_CreateThread(int(SDLCALL *fn)(void *),
                 const char *name, void *data)
#endif
{
#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
    return SDL_CreateThreadWithStackSize(fn, name, 0, data, pfnBeginThread, pfnEndThread);
#else
    return SDL_CreateThreadWithStackSize(fn, name, 0, data);
#endif
}

SDL_Thread *SDL_CreateThreadInternal(int(SDLCALL *fn)(void *), const char *name,
                         const size_t stacksize, void *data)
{
#ifdef SDL_PASSED_BEGINTHREAD_ENDTHREAD
    return SDL_CreateThreadWithStackSize(fn, name, stacksize, data, NULL, NULL);
#else
    return SDL_CreateThreadWithStackSize(fn, name, stacksize, data);
#endif
}

SDL_ThreadID SDL_GetThreadID(SDL_Thread *thread)
{
    SDL_ThreadID id;

    if (thread) {
        id = thread->threadid;
    } else {
        id = SDL_GetCurrentThreadID(void);
    }
    return id;
}

const char *SDL_GetThreadName(SDL_Thread *thread)
{
    if (thread) {
        return thread->name;
    } else {
        return NULL;
    }
}

int SDL_SetThreadPriority(SDL_ThreadPriority priority)
{
    return SDL_SYS_SetThreadPriority(priority);
}

void SDL_WaitThread(SDL_Thread *thread, int *status)
{
    if (thread) {
        SDL_SYS_WaitThread(thread);
        if (status) {
            *status = thread->status;
        }
        if (thread->name) {
            SDL_free(thread->name);
        }
        SDL_free(thread);
    }
}

void SDL_DetachThread(SDL_Thread *thread)
{
    if (!thread) {
        return;
    }

    /* Grab dibs if the state is alive+joinable. */
    if (SDL_AtomicCompareAndSwap(&thread->state, SDL_THREAD_STATE_ALIVE, SDL_THREAD_STATE_DETACHED)) {
        SDL_SYS_DetachThread(thread);
    } else {
        /* all other states are pretty final, see where we landed. */
        const int thread_state = SDL_AtomicGet(&thread->state);
        if ((thread_state == SDL_THREAD_STATE_DETACHED) || (thread_state == SDL_THREAD_STATE_CLEANED)) {
            return; /* already detached (you shouldn't call this twice!) */
        } else if (thread_state == SDL_THREAD_STATE_ZOMBIE) {
            SDL_WaitThread(thread, NULL); /* already done, clean it up. */
        } else {
            SDL_assert(0 && "Unexpected thread state");
        }
    }
}

int SDL_WaitSemaphore(SDL_Semaphore *sem)
{
    return SDL_WaitSemaphoreTimeoutNS(sem, -1);
}

int SDL_TryWaitSemaphore(SDL_Semaphore *sem)
{
    return SDL_WaitSemaphoreTimeoutNS(sem, 0);
}

int SDL_WaitSemaphoreTimeout(SDL_Semaphore *sem, Sint32 timeoutMS)
{
    Sint64 timeoutNS;

    if (timeoutMS >= 0) {
        timeoutNS = SDL_MS_TO_NS(timeoutMS);
    } else {
        timeoutNS = -1;
    }
    return SDL_WaitSemaphoreTimeoutNS(sem, timeoutNS);
}

int SDL_WaitCondition(SDL_Condition *cond, SDL_Mutex *mutex)
{
    return SDL_WaitConditionTimeoutNS(cond, mutex, -1);
}

int SDL_WaitConditionTimeout(SDL_Condition *cond, SDL_Mutex *mutex, Sint32 timeoutMS)
{
    Sint64 timeoutNS;

    if (timeoutMS >= 0) {
        timeoutNS = SDL_MS_TO_NS(timeoutMS);
    } else {
        timeoutNS = -1;
    }
    return SDL_WaitConditionTimeoutNS(cond, mutex, timeoutNS);
}
