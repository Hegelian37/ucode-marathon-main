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

#ifdef SDL_AUDIO_DRIVER_OPENSLES

// For more discussion of low latency audio on Android, see this:
//   https://googlesamples.github.io/android-audio-high-performance/guides/opensl_es.html

#include "../SDL_sysaudio.h"
#include "SDL_openslES.h"

#include "../../core/android/SDL_android.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <android/log.h>


#define NUM_BUFFERS 2 // -- Don't lower this!

struct SDL_PrivateAudioData
{
    Uint8 *mixbuff;
    int next_buffer;
    Uint8 *pmixbuff[NUM_BUFFERS];
    SDL_Semaphore *playsem;
};

#if 0
#define LOG_TAG   "SDL_openslES"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//#define LOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define LOGV(...)
#else
#define LOGE(...)
#define LOGI(...)
#define LOGV(...)
#endif

/*
#define SL_SPEAKER_FRONT_LEFT            ((SLuint32) 0x00000001)
#define SL_SPEAKER_FRONT_RIGHT           ((SLuint32) 0x00000002)
#define SL_SPEAKER_FRONT_CENTER          ((SLuint32) 0x00000004)
#define SL_SPEAKER_LOW_FREQUENCY         ((SLuint32) 0x00000008)
#define SL_SPEAKER_BACK_LEFT             ((SLuint32) 0x00000010)
#define SL_SPEAKER_BACK_RIGHT            ((SLuint32) 0x00000020)
#define SL_SPEAKER_FRONT_LEFT_OF_CENTER  ((SLuint32) 0x00000040)
#define SL_SPEAKER_FRONT_RIGHT_OF_CENTER ((SLuint32) 0x00000080)
#define SL_SPEAKER_BACK_CENTER           ((SLuint32) 0x00000100)
#define SL_SPEAKER_SIDE_LEFT             ((SLuint32) 0x00000200)
#define SL_SPEAKER_SIDE_RIGHT            ((SLuint32) 0x00000400)
#define SL_SPEAKER_TOP_CENTER            ((SLuint32) 0x00000800)
#define SL_SPEAKER_TOP_FRONT_LEFT        ((SLuint32) 0x00001000)
#define SL_SPEAKER_TOP_FRONT_CENTER      ((SLuint32) 0x00002000)
#define SL_SPEAKER_TOP_FRONT_RIGHT       ((SLuint32) 0x00004000)
#define SL_SPEAKER_TOP_BACK_LEFT         ((SLuint32) 0x00008000)
#define SL_SPEAKER_TOP_BACK_CENTER       ((SLuint32) 0x00010000)
#define SL_SPEAKER_TOP_BACK_RIGHT        ((SLuint32) 0x00020000)
*/
#define SL_ANDROID_SPEAKER_STEREO (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT)
#define SL_ANDROID_SPEAKER_QUAD   (SL_ANDROID_SPEAKER_STEREO | SL_SPEAKER_BACK_LEFT | SL_SPEAKER_BACK_RIGHT)
#define SL_ANDROID_SPEAKER_5DOT1  (SL_ANDROID_SPEAKER_QUAD | SL_SPEAKER_FRONT_CENTER | SL_SPEAKER_LOW_FREQUENCY)
#define SL_ANDROID_SPEAKER_7DOT1  (SL_ANDROID_SPEAKER_5DOT1 | SL_SPEAKER_SIDE_LEFT | SL_SPEAKER_SIDE_RIGHT)

// engine interfaces
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine = NULL;

// output mix interfaces
static SLObjectItf outputMixObject = NULL;

// buffer queue player interfaces
static SLObjectItf bqPlayerObject = NULL;
static SLPlayItf bqPlayerPlay = NULL;
static SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue = NULL;
#if 0
static SLVolumeItf bqPlayerVolume;
#endif

// recorder interfaces
static SLObjectItf recorderObject = NULL;
static SLRecordItf recorderRecord = NULL;
static SLAndroidSimpleBufferQueueItf recorderBufferQueue = NULL;

#if 0
static const char *sldevaudiorecorderstr = "SLES Audio Recorder";
static const char *sldevaudioplayerstr   = "SLES Audio Player";

#define SLES_DEV_AUDIO_RECORDER sldevaudiorecorderstr
#define SLES_DEV_AUDIO_PLAYER   sldevaudioplayerstr
static void OPENSLES_DetectDevices( int iscapture )
{
    LOGI( "openSLES_DetectDevices(void)" );
    if ( iscapture )
            addfn( SLES_DEV_AUDIO_RECORDER );
    else
            addfn( SLES_DEV_AUDIO_PLAYER );
}
#endif

static void OPENSLES_DestroyEngine(void)
{
    LOGI("OPENSLES_DestroyEngine(void)");

    // destroy output mix object, and invalidate all associated interfaces
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}

static int OPENSLES_CreateEngine(void)
{
    const SLInterfaceID ids[1] = { SL_IID_VOLUME };
    const SLboolean req[1] = { SL_BOOLEAN_FALSE };
    SLresult result;

    LOGI("openSLES_CreateEngine(void)");

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("slCreateEngine failed: %d", result);
        goto error;
    }
    LOGI("slCreateEngine OK");

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("RealizeEngine failed: %d", result);
        goto error;
    }
    LOGI("RealizeEngine OK");

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("EngineGetInterface failed: %d", result);
        goto error;
    }
    LOGI("EngineGetInterface OK");

    // create output mix
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("CreateOutputMix failed: %d", result);
        goto error;
    }
    LOGI("CreateOutputMix OK");

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("RealizeOutputMix failed: %d", result);
        goto error;
    }
    return 1;

error:
    OPENSLES_DestroyEngine(void);
    return 0;
}

// this callback handler is called every time a buffer finishes recording
static void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    struct SDL_PrivateAudioData *audiodata = (struct SDL_PrivateAudioData *)context;

    LOGV("SLES: Recording Callback");
    SDL_PostSemaphore(audiodata->playsem);
}

static void OPENSLES_DestroyPCMRecorder(SDL_AudioDevice *device)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;
    SLresult result;

    // stop recording
    if (recorderRecord != NULL) {
        result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_STOPPED);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("SetRecordState stopped: %d", result);
        }
    }

    // destroy audio recorder object, and invalidate all associated interfaces
    if (recorderObject != NULL) {
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = NULL;
        recorderRecord = NULL;
        recorderBufferQueue = NULL;
    }

    if (audiodata->playsem) {
        SDL_DestroySemaphore(audiodata->playsem);
        audiodata->playsem = NULL;
    }

    if (audiodata->mixbuff) {
        SDL_free(audiodata->mixbuff);
    }
}

// !!! FIXME: make this non-blocking!
static void SDLCALL AndroidRequestPermissionBlockingCallback(void *userdata, const char *permission, SDL_bool granted)
{
    SDL_AtomicSet((SDL_AtomicInt *) userdata, granted ? 1 : -1);
}

static int OPENSLES_CreatePCMRecorder(SDL_AudioDevice *device)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;
    SLDataFormat_PCM format_pcm;
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq;
    SLDataSink audioSnk;
    SLDataLocator_IODevice loc_dev;
    SLDataSource audioSrc;
    const SLInterfaceID ids[1] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
    const SLboolean req[1] = { SL_BOOLEAN_TRUE };
    SLresult result;
    int i;

    // !!! FIXME: make this non-blocking!
    {
        SDL_AtomicInt permission_response;
        SDL_AtomicSet(&permission_response, 0);
        if (SDL_AndroidRequestPermission("android.permission.RECORD_AUDIO", AndroidRequestPermissionBlockingCallback, &permission_response) == -1) {
            return -1;
        }

        while (SDL_AtomicGet(&permission_response) == 0) {
            SDL_Delay(10);
        }

        if (SDL_AtomicGet(&permission_response) < 0) {
            LOGE("This app doesn't have RECORD_AUDIO permission");
            return SDL_SetError("This app doesn't have RECORD_AUDIO permission");
        }
    }

    // Just go with signed 16-bit audio as it's the most compatible
    device->spec.format = SDL_AUDIO_S16;
    device->spec.channels = 1;
    //device->spec.freq = SL_SAMPLINGRATE_16 / 1000;*/

    // Update the fragment size as size in bytes
    SDL_UpdatedAudioDeviceFormat(device);

    LOGI("Try to open %u hz %u bit chan %u %s samples %u",
         device->spec.freq, SDL_AUDIO_BITSIZE(device->spec.format),
         device->spec.channels, (device->spec.format & 0x1000) ? "BE" : "LE", device->sample_frames);

    // configure audio source
    loc_dev.locatorType = SL_DATALOCATOR_IODEVICE;
    loc_dev.deviceType = SL_IODEVICE_AUDIOINPUT;
    loc_dev.deviceID = SL_DEFAULTDEVICEID_AUDIOINPUT;
    loc_dev.device = NULL;
    audioSrc.pLocator = &loc_dev;
    audioSrc.pFormat = NULL;

    // configure audio sink
    loc_bufq.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    loc_bufq.numBuffers = NUM_BUFFERS;

    format_pcm.formatType = SL_DATAFORMAT_PCM;
    format_pcm.numChannels = device->spec.channels;
    format_pcm.samplesPerSec = device->spec.freq * 1000; // / kilo Hz to milli Hz
    format_pcm.bitsPerSample = SDL_AUDIO_BITSIZE(device->spec.format);
    format_pcm.containerSize = SDL_AUDIO_BITSIZE(device->spec.format);
    format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
    format_pcm.channelMask = SL_SPEAKER_FRONT_CENTER;

    audioSnk.pLocator = &loc_bufq;
    audioSnk.pFormat = &format_pcm;

    // create audio recorder
    // (requires the RECORD_AUDIO permission)
    result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject, &audioSrc, &audioSnk, 1, ids, req);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("CreateAudioRecorder failed: %d", result);
        goto failed;
    }

    // realize the recorder
    result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("RealizeAudioPlayer failed: %d", result);
        goto failed;
    }

    // get the record interface
    result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recorderRecord);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("SL_IID_RECORD interface get failed: %d", result);
        goto failed;
    }

    // get the buffer queue interface
    result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &recorderBufferQueue);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("SL_IID_BUFFERQUEUE interface get failed: %d", result);
        goto failed;
    }

    // register callback on the buffer queue
    // context is '(SDL_PrivateAudioData *)device->hidden'
    result = (*recorderBufferQueue)->RegisterCallback(recorderBufferQueue, bqRecorderCallback, device->hidden);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("RegisterCallback failed: %d", result);
        goto failed;
    }

    // Create the audio buffer semaphore
    audiodata->playsem = SDL_CreateSemaphore(0);
    if (!audiodata->playsem) {
        LOGE("cannot create Semaphore!");
        goto failed;
    }

    // Create the sound buffers
    audiodata->mixbuff = (Uint8 *)SDL_malloc(NUM_BUFFERS * device->buffer_size);
    if (!audiodata->mixbuff) {
        LOGE("mixbuffer allocate - out of memory");
        goto failed;
    }

    for (i = 0; i < NUM_BUFFERS; i++) {
        audiodata->pmixbuff[i] = audiodata->mixbuff + i * device->buffer_size;
    }

    // in case already recording, stop recording and clear buffer queue
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_STOPPED);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Record set state failed: %d", result);
        goto failed;
    }

    // enqueue empty buffers to be filled by the recorder
    for (i = 0; i < NUM_BUFFERS; i++) {
        result = (*recorderBufferQueue)->Enqueue(recorderBufferQueue, audiodata->pmixbuff[i], device->buffer_size);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("Record enqueue buffers failed: %d", result);
            goto failed;
        }
    }

    // start recording
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_RECORDING);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Record set state failed: %d", result);
        goto failed;
    }

    return 0;

failed:
    return SDL_SetError("Open device failed!");
}

// this callback handler is called every time a buffer finishes playing
static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    struct SDL_PrivateAudioData *audiodata = (struct SDL_PrivateAudioData *)context;

    LOGV("SLES: Playback Callback");
    SDL_PostSemaphore(audiodata->playsem);
}

static void OPENSLES_DestroyPCMPlayer(SDL_AudioDevice *device)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;

    // set the player's state to 'stopped'
    if (bqPlayerPlay != NULL) {
        const SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_STOPPED);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("SetPlayState stopped failed: %d", result);
        }
    }

    // destroy buffer queue audio player object, and invalidate all associated interfaces
    if (bqPlayerObject != NULL) {
        (*bqPlayerObject)->Destroy(bqPlayerObject);

        bqPlayerObject = NULL;
        bqPlayerPlay = NULL;
        bqPlayerBufferQueue = NULL;
    }

    if (audiodata->playsem) {
        SDL_DestroySemaphore(audiodata->playsem);
        audiodata->playsem = NULL;
    }

    if (audiodata->mixbuff) {
        SDL_free(audiodata->mixbuff);
    }
}

static int OPENSLES_CreatePCMPlayer(SDL_AudioDevice *device)
{
    /* If we want to add floating point audio support (requires API level 21)
       it can be done as described here:
        https://developer.android.com/ndk/guides/audio/opensl/android-extensions.html#floating-point
    */
    if (SDL_GetAndroidSDKVersion(void) >= 21) {
        const SDL_AudioFormat *closefmts = SDL_ClosestAudioFormats(device->spec.format);
        SDL_AudioFormat test_format;
        while ((test_format = *(closefmts++)) != 0) {
            if (SDL_AUDIO_ISSIGNED(test_format)) {
                break;
            }
        }

        if (!test_format) {
            // Didn't find a compatible format :
            LOGI("No compatible audio format, using signed 16-bit audio");
            test_format = SDL_AUDIO_S16;
        }
        device->spec.format = test_format;
    } else {
        // Just go with signed 16-bit audio as it's the most compatible
        device->spec.format = SDL_AUDIO_S16;
    }

    // Update the fragment size as size in bytes
    SDL_UpdatedAudioDeviceFormat(device);

    LOGI("Try to open %u hz %s %u bit chan %u %s samples %u",
         device->spec.freq, SDL_AUDIO_ISFLOAT(device->spec.format) ? "float" : "pcm", SDL_AUDIO_BITSIZE(device->spec.format),
         device->spec.channels, (device->spec.format & 0x1000) ? "BE" : "LE", device->sample_frames);

    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq;
    loc_bufq.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    loc_bufq.numBuffers = NUM_BUFFERS;

    SLDataFormat_PCM format_pcm;
    format_pcm.formatType = SL_DATAFORMAT_PCM;
    format_pcm.numChannels = device->spec.channels;
    format_pcm.samplesPerSec = device->spec.freq * 1000; // / kilo Hz to milli Hz
    format_pcm.bitsPerSample = SDL_AUDIO_BITSIZE(device->spec.format);
    format_pcm.containerSize = SDL_AUDIO_BITSIZE(device->spec.format);

    if (SDL_AUDIO_ISBIGENDIAN(device->spec.format)) {
        format_pcm.endianness = SL_BYTEORDER_BIGENDIAN;
    } else {
        format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
    }

    switch (device->spec.channels) {
    case 1:
        format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT;
        break;
    case 2:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_STEREO;
        break;
    case 3:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_STEREO | SL_SPEAKER_FRONT_CENTER;
        break;
    case 4:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_QUAD;
        break;
    case 5:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_QUAD | SL_SPEAKER_FRONT_CENTER;
        break;
    case 6:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_5DOT1;
        break;
    case 7:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_5DOT1 | SL_SPEAKER_BACK_CENTER;
        break;
    case 8:
        format_pcm.channelMask = SL_ANDROID_SPEAKER_7DOT1;
        break;
    default:
        // Unknown number of channels, fall back to stereo
        device->spec.channels = 2;
        format_pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        break;
    }

    SLDataSink audioSnk;
    SLDataSource audioSrc;
    audioSrc.pFormat = (void *)&format_pcm;

    SLAndroidDataFormat_PCM_EX format_pcm_ex;
    if (SDL_AUDIO_ISFLOAT(device->spec.format)) {
        // Copy all setup into PCM EX structure
        format_pcm_ex.formatType = SL_ANDROID_DATAFORMAT_PCM_EX;
        format_pcm_ex.endianness = format_pcm.endianness;
        format_pcm_ex.channelMask = format_pcm.channelMask;
        format_pcm_ex.numChannels = format_pcm.numChannels;
        format_pcm_ex.sampleRate = format_pcm.samplesPerSec;
        format_pcm_ex.bitsPerSample = format_pcm.bitsPerSample;
        format_pcm_ex.containerSize = format_pcm.containerSize;
        format_pcm_ex.representation = SL_ANDROID_PCM_REPRESENTATION_FLOAT;
        audioSrc.pFormat = (void *)&format_pcm_ex;
    }

    audioSrc.pLocator = &loc_bufq;

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix;
    loc_outmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    loc_outmix.outputMix = outputMixObject;
    audioSnk.pLocator = &loc_outmix;
    audioSnk.pFormat = NULL;

    // create audio player
    const SLInterfaceID ids[2] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME };
    const SLboolean req[2] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_FALSE };
    SLresult result;
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &bqPlayerObject, &audioSrc, &audioSnk, 2, ids, req);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("CreateAudioPlayer failed: %d", result);
        goto failed;
    }

    // realize the player
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("RealizeAudioPlayer failed: %d", result);
        goto failed;
    }

    // get the play interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("SL_IID_PLAY interface get failed: %d", result);
        goto failed;
    }

    // get the buffer queue interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &bqPlayerBufferQueue);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("SL_IID_BUFFERQUEUE interface get failed: %d", result);
        goto failed;
    }

    // register callback on the buffer queue
    // context is '(SDL_PrivateAudioData *)device->hidden'
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, device->hidden);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("RegisterCallback failed: %d", result);
        goto failed;
    }

#if 0
    // get the volume interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &bqPlayerVolume);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("SL_IID_VOLUME interface get failed: %d", result);
        // goto failed;
    }
#endif

    struct SDL_PrivateAudioData *audiodata = device->hidden;

    // Create the audio buffer semaphore
    audiodata->playsem = SDL_CreateSemaphore(NUM_BUFFERS - 1);
    if (!audiodata->playsem) {
        LOGE("cannot create Semaphore!");
        goto failed;
    }

    // Create the sound buffers
    audiodata->mixbuff = (Uint8 *)SDL_malloc(NUM_BUFFERS * device->buffer_size);
    if (!audiodata->mixbuff) {
        LOGE("mixbuffer allocate - out of memory");
        goto failed;
    }

    for (int i = 0; i < NUM_BUFFERS; i++) {
        audiodata->pmixbuff[i] = audiodata->mixbuff + i * device->buffer_size;
    }

    // set the player's state to playing
    result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Play set state failed: %d", result);
        goto failed;
    }

    return 0;

failed:
    return -1;
}

static int OPENSLES_OpenDevice(SDL_AudioDevice *device)
{
    device->hidden = (struct SDL_PrivateAudioData *)SDL_calloc(1, sizeof(*device->hidden));
    if (!device->hidden) {
        return -1;
    }

    if (device->iscapture) {
        LOGI("OPENSLES_OpenDevice(void) for capture");
        return OPENSLES_CreatePCMRecorder(device);
    } else {
        int ret;
        LOGI("OPENSLES_OpenDevice(void) for playing");
        ret = OPENSLES_CreatePCMPlayer(device);
        if (ret < 0) {
            // Another attempt to open the device with a lower frequency
            if (device->spec.freq > 48000) {
                OPENSLES_DestroyPCMPlayer(device);
                device->spec.freq = 48000;
                ret = OPENSLES_CreatePCMPlayer(device);
            }
        }

        if (ret != 0) {
            return SDL_SetError("Open device failed!");
        }
    }

    return 0;
}

static int OPENSLES_WaitDevice(SDL_AudioDevice *device)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;

    LOGV("OPENSLES_WaitDevice(void)");

    // Wait for an audio chunk to finish
    return SDL_WaitSemaphore(audiodata->playsem);
}

static int OPENSLES_PlayDevice(SDL_AudioDevice *device, const Uint8 *buffer, int buflen)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;

    LOGV("======OPENSLES_PlayDevice(void)======");

    // Queue it up
    const SLresult result = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, buffer, buflen);

    audiodata->next_buffer++;
    if (audiodata->next_buffer >= NUM_BUFFERS) {
        audiodata->next_buffer = 0;
    }

    // If Enqueue fails, callback won't be called.
    // Post the semaphore, not to run out of buffer
    if (SL_RESULT_SUCCESS != result) {
        SDL_PostSemaphore(audiodata->playsem);
    }

    return 0;
}

///           n   playn sem
// getbuf     0   -     1
// fill buff  0   -     1
// play       0 - 0     1
// wait       1   0     0
// getbuf     1   0     0
// fill buff  1   0     0
// play       0   0     0
// wait
//
// okay..

static Uint8 *OPENSLES_GetDeviceBuf(SDL_AudioDevice *device, int *bufsize)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;

    LOGV("OPENSLES_GetDeviceBuf(void)");
    return audiodata->pmixbuff[audiodata->next_buffer];
}

static int OPENSLES_CaptureFromDevice(SDL_AudioDevice *device, void *buffer, int buflen)
{
    struct SDL_PrivateAudioData *audiodata = device->hidden;

    // Copy it to the output buffer
    SDL_assert(buflen == device->buffer_size);
    SDL_memcpy(buffer, audiodata->pmixbuff[audiodata->next_buffer], device->buffer_size);

    // Re-enqueue the buffer
    const SLresult result = (*recorderBufferQueue)->Enqueue(recorderBufferQueue, audiodata->pmixbuff[audiodata->next_buffer], device->buffer_size);
    if (SL_RESULT_SUCCESS != result) {
        LOGE("Record enqueue buffers failed: %d", result);
        return -1;
    }

    audiodata->next_buffer++;
    if (audiodata->next_buffer >= NUM_BUFFERS) {
        audiodata->next_buffer = 0;
    }

    return device->buffer_size;
}

static void OPENSLES_CloseDevice(SDL_AudioDevice *device)
{
    // struct SDL_PrivateAudioData *audiodata = device->hidden;
    if (device->hidden) {
        if (device->iscapture) {
            LOGI("OPENSLES_CloseDevice(void) for capture");
            OPENSLES_DestroyPCMRecorder(device);
        } else {
            LOGI("OPENSLES_CloseDevice(void) for playing");
            OPENSLES_DestroyPCMPlayer(device);
        }

        SDL_free(device->hidden);
        device->hidden = NULL;
    }
}

static SDL_bool OPENSLES_Init(SDL_AudioDriverImpl *impl)
{
    LOGI("OPENSLES_Init(void) called");

    if (!OPENSLES_CreateEngine(void)) {
        return SDL_FALSE;
    }

    LOGI("OPENSLES_Init(void) - set pointers");

    // Set the function pointers
    // impl->DetectDevices = OPENSLES_DetectDevices;
    impl->ThreadInit = Android_AudioThreadInit;
    impl->OpenDevice = OPENSLES_OpenDevice;
    impl->WaitDevice = OPENSLES_WaitDevice;
    impl->PlayDevice = OPENSLES_PlayDevice;
    impl->GetDeviceBuf = OPENSLES_GetDeviceBuf;
    impl->WaitCaptureDevice = OPENSLES_WaitDevice;
    impl->CaptureFromDevice = OPENSLES_CaptureFromDevice;
    impl->CloseDevice = OPENSLES_CloseDevice;
    impl->Deinitialize = OPENSLES_DestroyEngine;

    // and the capabilities
    impl->HasCaptureSupport = SDL_TRUE;
    impl->OnlyHasDefaultOutputDevice = SDL_TRUE;
    impl->OnlyHasDefaultCaptureDevice = SDL_TRUE;

    LOGI("OPENSLES_Init(void) - success");

    // this audio target is available.
    return SDL_TRUE;
}

AudioBootStrap OPENSLES_bootstrap = {
    "openslES", "OpenSL ES audio driver", OPENSLES_Init, SDL_FALSE
};

void OPENSLES_ResumeDevices(void)
{
    if (bqPlayerPlay != NULL) {
        // set the player's state to 'playing'
        SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("OPENSLES_ResumeDevices failed: %d", result);
        }
    }
}

void OPENSLES_PauseDevices(void)
{
    if (bqPlayerPlay != NULL) {
        // set the player's state to 'paused'
        SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PAUSED);
        if (SL_RESULT_SUCCESS != result) {
            LOGE("OPENSLES_PauseDevices failed: %d", result);
        }
    }
}

#endif // SDL_AUDIO_DRIVER_OPENSLES
