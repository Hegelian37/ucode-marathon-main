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

#ifdef SDL_VIDEO_DRIVER_UIKIT

#include "SDL_uikitvideo.h"
#include "../../events/SDL_clipboardevents_c.h"

#import <UIKit/UIPasteboard.h>

int UIKit_SetClipboardText(SDL_VideoDevice *_this, const char *text)
{
#ifdef SDL_PLATFORM_TVOS
    return SDL_SetError("The clipboard is not available on tvOS");
#else
    @autoreleasepool {
        [UIPasteboard generalPasteboard].string = @(text);
        return 0;
    }
#endif
}

char *UIKit_GetClipboardText(SDL_VideoDevice *_this)
{
#ifdef SDL_PLATFORM_TVOS
    return SDL_strdup(""); // Unsupported.
#else
    @autoreleasepool {
        UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
        NSString *string = pasteboard.string;

        if (string != nil) {
            return SDL_strdup(string.UTF8String);
        } else {
            return SDL_strdup("");
        }
    }
#endif
}

SDL_bool UIKit_HasClipboardText(SDL_VideoDevice *_this)
{
    @autoreleasepool {
#ifndef SDL_PLATFORM_TVOS
        if ([UIPasteboard generalPasteboard].string != nil) {
            return SDL_TRUE;
        }
#endif
        return SDL_FALSE;
    }
}

void UIKit_InitClipboard(SDL_VideoDevice *_this)
{
#ifndef SDL_PLATFORM_TVOS
    @autoreleasepool {
        SDL_UIKitVideoData *data = (__bridge SDL_UIKitVideoData *)_this->driverdata;
        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];

        id observer = [center addObserverForName:UIPasteboardChangedNotification
                                          object:nil
                                           queue:nil
                                      usingBlock:^(NSNotification *note) {
                                        SDL_SendClipboardUpdate(void);
                                      }];

        data.pasteboardObserver = observer;
    }
#endif
}

void UIKit_QuitClipboard(SDL_VideoDevice *_this)
{
    @autoreleasepool {
        SDL_UIKitVideoData *data = (__bridge SDL_UIKitVideoData *)_this->driverdata;

        if (data.pasteboardObserver != nil) {
            [[NSNotificationCenter defaultCenter] removeObserver:data.pasteboardObserver];
        }

        data.pasteboardObserver = nil;
    }
}

#endif /* SDL_VIDEO_DRIVER_UIKIT */
