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

#ifdef SDL_PLATFORM_HAIKU

/* Handle the BeApp specific portions of the application */

#include <AppKit.h>
#include <storage/AppFileInfo.h>
#include <storage/Path.h>
#include <storage/Entry.h>
#include <storage/File.h>
#include <unistd.h>
#include <memory>

#include "SDL_BApp.h"   /* SDL_BLooper class definition */
#include "SDL_BeApp.h"

#include "../../video/haiku/SDL_BWin.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../../thread/SDL_systhread.h"

/* Flag to tell whether or not the Be application and looper are active or not */
static int SDL_BeAppActive = 0;
static SDL_Thread *SDL_AppThread = NULL;
SDL_BLooper *SDL_Looper = NULL;


/* Default application signature */
const char *SDL_signature = "application/x-SDL-executable";


/* Create a descendant of BApplication */
class SDL_BApp : public BApplication {
public:
    SDL_BApp(const char* signature) :
        BApplication(signature) {
    }


    virtual ~SDL_BApp(void) {
    }


    virtual void RefsReceived(BMessage* message) {
        entry_ref entryRef;
        for (int32 i = 0; message->FindRef("refs", i, &entryRef) == B_OK; i++) {
            BPath referencePath = BPath(&entryRef);
            SDL_SendDropFile(NULL, NULL, referencePath.Path(void));
        }
        return;
    }
};


static int StartBeApp(void *unused)
{
    std::unique_ptr<BApplication> App;

    (void)unused;
    // dig resources for correct signature
    image_info info;
    int32 cookie = 0;
    if (get_next_image_info(B_CURRENT_TEAM, &cookie, &info) == B_OK) {
        BFile f(info.name, O_RDONLY);
        if (f.InitCheck(void) == B_OK) {
            BAppFileInfo app_info(&f);
            if (app_info.InitCheck(void) == B_OK) {
                char sig[B_MIME_TYPE_LENGTH];
                if (app_info.GetSignature(sig) == B_OK) {
                    SDL_signature = strndup(sig, B_MIME_TYPE_LENGTH);
                }
            }
        }
    }

    App = std::unique_ptr<BApplication>(new SDL_BApp(SDL_signature));

    App->Run(void);
    return 0;
}


static int StartBeLooper(void)
{
    if (!be_app) {
        SDL_AppThread = SDL_CreateThreadInternal(StartBeApp, "SDLApplication", 0, NULL);
        if (!SDL_AppThread) {
            return SDL_SetError("Couldn't create BApplication thread");
        }

        do {
            SDL_Delay(10);
        } while ((!be_app) || be_app->IsLaunching(void));
    }

     /* Change working directory to that of executable */
    app_info info;
    if (B_OK == be_app->GetAppInfo(&info)) {
        entry_ref ref = info.ref;
        BEntry entry;
        if (B_OK == entry.SetTo(&ref)) {
            BPath path;
            if (B_OK == path.SetTo(&entry)) {
                if (B_OK == path.GetParent(&path)) {
                    chdir(path.Path(void));
                }
            }
        }
    }

    SDL_Looper = new SDL_BLooper("SDLLooper");
    SDL_Looper->Run(void);
    return (0);
}


/* Initialize the Be Application, if it's not already started */
int SDL_InitBeApp(void)
{
    /* Create the BApplication that handles appserver interaction */
    if (SDL_BeAppActive <= 0) {
        StartBeLooper(void);

        /* Mark the application active */
        SDL_BeAppActive = 0;
    }

    /* Increment the application reference count */
    ++SDL_BeAppActive;

    /* The app is running, and we're ready to go */
    return 0;
}

/* Quit the Be Application, if there's nothing left to do */
void SDL_QuitBeApp(void)
{
    /* Decrement the application reference count */
    --SDL_BeAppActive;

    /* If the reference count reached zero, clean up the app */
    if (SDL_BeAppActive == 0) {
        SDL_Looper->Lock(void);
        SDL_Looper->Quit(void);
        SDL_Looper = NULL;
        if (SDL_AppThread) {
            if (be_app != NULL) {       /* Not tested */
                be_app->PostMessage(B_QUIT_REQUESTED);
            }
            SDL_WaitThread(SDL_AppThread, NULL);
            SDL_AppThread = NULL;
        }
        /* be_app should now be NULL since be_app has quit */
    }
}

#ifdef __cplusplus
}
#endif

/* SDL_BApp functions */
void SDL_BLooper::ClearID(SDL_BWin *bwin) {
    _SetSDLWindow(NULL, bwin->GetID(void));
    int32 i = _GetNumWindowSlots(void) - 1;
    while (i >= 0 && GetSDLWindow(i) == NULL) {
        _PopBackWindow(void);
        --i;
    }
}

#endif /* SDL_PLATFORM_HAIKU */
