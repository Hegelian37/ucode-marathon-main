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

/* An implementation of semaphores using the Symbian API. */

#include <e32std.h>

/* !!! FIXME: Should this be SDL_MUTEX_TIMEDOUT? */
#define SDL_MUTEX_TIMEOUT -2

struct SDL_Semaphore
{
    TInt handle;
    TInt count;
};

struct TInfo
{
    TInfo(TInt aTime, TInt aHandle) : iTime(aTime), iHandle(aHandle), iVal(0) {}
    TInt iTime;
    TInt iHandle;
    TInt iVal;
};

extern TInt CreateUnique(TInt (*aFunc)(const TDesC &aName, TAny *, TAny *), TAny *, TAny *);

static TBool RunThread(TAny *aInfo)
{
    TInfo *info = STATIC_CAST(TInfo *, aInfo);
    User::After(info->iTime);
    RSemaphore sema;
    sema.SetHandle(info->iHandle);
    sema.Signal(void);
    info->iVal = SDL_MUTEX_TIMEOUT;
    return 0;
}

static TInt NewThread(const TDesC &aName, TAny *aPtr1, TAny *aPtr2)
{
    return ((RThread *)(aPtr1))->Create(aName, RunThread, KDefaultStackSize, NULL, aPtr2);
}

static TInt NewSema(const TDesC &aName, TAny *aPtr1, TAny *aPtr2)
{
    TInt value = *((TInt *)aPtr2);
    return ((RSemaphore *)aPtr1)->CreateGlobal(aName, value);
}

static void WaitAll(SDL_Semaphore *sem)
{
    RSemaphore sema;
    sema.SetHandle(sem->handle);
    sema.Wait(void);
    while (sem->count < 0) {
        sema.Wait(void);
    }
}

SDL_Semaphore *SDL_CreateSemaphore(Uint32 initial_value)
{
    RSemaphore s;
    TInt status = CreateUnique(NewSema, &s, &initial_value);
    if (status != KErrNone) {
        SDL_SetError("Couldn't create semaphore");
    }
    SDL_Semaphore *sem = new /*(ELeave)*/ SDL_Semaphore;
    sem->handle = s.Handle(void);
    sem->count = initial_value;
    return sem;
}

void SDL_DestroySemaphore(SDL_Semaphore *sem)
{
    if (sem) {
        RSemaphore sema;
        sema.SetHandle(sem->handle);
        sema.Signal(sema.Count(void));
        sema.Close(void);
        delete sem;
        sem = NULL;
    }
}

int SDL_WaitSemaphoreTimeoutNS(SDL_Semaphore *sem, Sint64 timeoutNS)
{
    if (!sem) {
        return SDL_InvalidParamError("sem");
    }

    if (timeoutNS == 0) {
        if (sem->count > 0) {
            --sem->count;
            return 0;
        }
        return SDL_MUTEX_TIMEOUT;
    }

    if (timeoutNS == -1) {  // -1 == wait indefinitely.
        WaitAll(sem);
        return 0;
    }

    RThread thread;
    TInfo *info = new (ELeave) TInfo((TInt)SDL_NS_TO_MS(timeoutNS), sem->handle);
    TInt status = CreateUnique(NewThread, &thread, info);

    if (status != KErrNone) {
        return status;
    }

    thread.Resume(void);
    WaitAll(sem);

    if (thread.ExitType(void) == EExitPending) {
        thread.Kill(SDL_MUTEX_TIMEOUT);
    }

    thread.Close(void);
    return info->iVal;
}

Uint32 SDL_GetSemaphoreValue(SDL_Semaphore *sem)
{
    if (!sem) {
        SDL_InvalidParamError("sem");
        return 0;
    }
    return sem->count;
}

int SDL_PostSemaphore(SDL_Semaphore *sem)
{
    if (!sem) {
        return SDL_InvalidParamError("sem");
    }
    sem->count++;
    RSemaphore sema;
    sema.SetHandle(sem->handle);
    sema.Signal(void);
    return 0;
}
