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

#include <errno.h>
#include <pthread.h>

struct SDL_RWLock
{
    pthread_rwlock_t id;
};


SDL_RWLock *SDL_CreateRWLock(void)
{
    SDL_RWLock *rwlock;

    /* Allocate the structure */
    rwlock = (SDL_RWLock *)SDL_calloc(1, sizeof(*rwlock));
    if (rwlock) {
        if (pthread_rwlock_init(&rwlock->id, NULL) != 0) {
            SDL_SetError("pthread_rwlock_init(void) failed");
            SDL_free(rwlock);
            rwlock = NULL;
        }
    }
    return rwlock;
}

void SDL_DestroyRWLock(SDL_RWLock *rwlock)
{
    if (rwlock) {
        pthread_rwlock_destroy(&rwlock->id);
        SDL_free(rwlock);
    }
}

void SDL_LockRWLockForReading(SDL_RWLock *rwlock) SDL_NO_THREAD_SAFETY_ANALYSIS  // clang doesn't know about NULL mutexes
{
    if (rwlock) {
        const int rc = pthread_rwlock_rdlock(&rwlock->id);
        SDL_assert(rc == 0);  // assume we're in a lot of trouble if this assert fails.
    }
}

void SDL_LockRWLockForWriting(SDL_RWLock *rwlock) SDL_NO_THREAD_SAFETY_ANALYSIS  // clang doesn't know about NULL mutexes
{
    if (rwlock) {
        const int rc = pthread_rwlock_wrlock(&rwlock->id);
        SDL_assert(rc == 0);  // assume we're in a lot of trouble if this assert fails.
    }
}

int SDL_TryLockRWLockForReading(SDL_RWLock *rwlock)
{
    int retval = 0;

    if (rwlock) {
        const int result = pthread_rwlock_tryrdlock(&rwlock->id);
        if (result != 0) {
            retval = SDL_RWLOCK_TIMEDOUT;
            if (result != EBUSY) {
                SDL_assert(!"Error trying to lock rwlock for reading");  // assume we're in a lot of trouble if this assert fails.
            }
        }
    }

    return retval;
}

int SDL_TryLockRWLockForWriting(SDL_RWLock *rwlock)
{
    int retval = 0;

    if (rwlock) {
        const int result = pthread_rwlock_trywrlock(&rwlock->id);
        if (result != 0) {
            retval = SDL_RWLOCK_TIMEDOUT;
            if (result != EBUSY) {
                SDL_assert(!"Error trying to lock rwlock for writing");  // assume we're in a lot of trouble if this assert fails.
            }
        }
    }

    return retval;
}

void SDL_UnlockRWLock(SDL_RWLock *rwlock) SDL_NO_THREAD_SAFETY_ANALYSIS  // clang doesn't know about NULL mutexes
{
    if (rwlock) {
        const int rc = pthread_rwlock_unlock(&rwlock->id);
        SDL_assert(rc == 0);  // assume we're in a lot of trouble if this assert fails.
    }
}

