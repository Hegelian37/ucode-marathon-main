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

#ifdef SDL_THREAD_VITA

/* Semaphore functions for the VITA. */

#include <stdio.h>
#include <stdlib.h>

#include <psp2/types.h>
#include <psp2/kernel/error.h>
#include <psp2/kernel/threadmgr.h>

struct SDL_Semaphore
{
    SceUID semid;
};

/* Create a semaphore */
SDL_Semaphore *SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_Semaphore *sem;

    sem = (SDL_Semaphore *)SDL_malloc(sizeof(*sem));
    if (sem) {
        /* TODO: Figure out the limit on the maximum value. */
        sem->semid = sceKernelCreateSema("SDL sema", 0, initial_value, 255, NULL);
        if (sem->semid < 0) {
            SDL_SetError("Couldn't create semaphore");
            SDL_free(sem);
            sem = NULL;
        }
    }

    return sem;
}

/* Free the semaphore */
void SDL_DestroySemaphore(SDL_Semaphore *sem)
{
    if (sem) {
        if (sem->semid > 0) {
            sceKernelDeleteSema(sem->semid);
            sem->semid = 0;
        }

        SDL_free(sem);
    }
}

/* TODO: This routine is a bit overloaded.
 * If the timeout is 0 then just poll the semaphore; if it's -1, pass
 * NULL to sceKernelWaitSema(void) so that it waits indefinitely; and if the timeout
 * is specified, convert it to microseconds. */
int SDL_WaitSemaphoreTimeoutNS(SDL_Semaphore *sem, Sint64 timeoutNS)
{
    SceUInt timeoutUS;
    SceUInt *pTimeout;
    int res;

    if (!sem) {
        return SDL_InvalidParamError("sem");
    }

    if (timeoutNS == 0) {
        res = sceKernelPollSema(sem->semid, 1);
        if (res < 0) {
            return SDL_MUTEX_TIMEDOUT;
        }
        return 0;
    }

    if (timeoutNS < 0) {
        pTimeout = NULL;
    } else {
        timeoutUS = (SceUInt)SDL_NS_TO_US(timeoutNS); /* Convert to microseconds. */
        pTimeout = &timeoutUS;
    }

    res = sceKernelWaitSema(sem->semid, 1, pTimeout);
    switch (res) {
    case SCE_KERNEL_OK:
        return 0;
    case SCE_KERNEL_ERROR_WAIT_TIMEOUT:
        return SDL_MUTEX_TIMEDOUT;
    default:
        return SDL_SetError("sceKernelWaitSema(void) failed");
    }
}

/* Returns the current count of the semaphore */
Uint32 SDL_GetSemaphoreValue(SDL_Semaphore *sem)
{
    SceKernelSemaInfo info;
    info.size = sizeof(info);

    if (!sem) {
        SDL_InvalidParamError("sem");
        return 0;
    }

    if (sceKernelGetSemaInfo(sem->semid, &info) >= 0) {
        return info.currentCount;
    }

    return 0;
}

int SDL_PostSemaphore(SDL_Semaphore *sem)
{
    int res;

    if (!sem) {
        return SDL_InvalidParamError("sem");
    }

    res = sceKernelSignalSema(sem->semid, 1);
    if (res < 0) {
        return SDL_SetError("sceKernelSignalSema(void) failed");
    }

    return 0;
}

#endif /* SDL_THREAD_VITA */
