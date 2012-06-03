/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

// (This file gets included by juce_win32_NativeCode.cpp, rather than being
// compiled on its own).
#if JUCE_INCLUDED_FILE && JUCE_DIRECTSOUND

//==============================================================================
END_JUCE_NAMESPACE

extern "C"
{

// Declare just the minimum number of interfaces for the DSound objects that we need..
typedef struct typeDSBUFFERDESC
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwBufferBytes;
    DWORD dwReserved;
    LPWAVEFORMATEX lpwfxFormat;
    GUID guid3DAlgorithm;
} DSBUFFERDESC;

struct IDirectSoundBuffer;

#undef INTERFACE
#define INTERFACE IDirectSound
DECLARE_INTERFACE_(IDirectSound, IUnknown)
{
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;
    STDMETHOD(CreateSoundBuffer)    (THIS_ DSBUFFERDESC*, IDirectSoundBuffer**, LPUNKNOWN) PURE;
    STDMETHOD(GetCaps)              (THIS_ void*) PURE;
    STDMETHOD(DuplicateSoundBuffer) (THIS_ IDirectSoundBuffer*, IDirectSoundBuffer**) PURE;
    STDMETHOD(SetCooperativeLevel)  (THIS_ HWND, DWORD) PURE;
    STDMETHOD(Compact)              (THIS) PURE;
    STDMETHOD(GetSpeakerConfig)     (THIS_ LPDWORD) PURE;
    STDMETHOD(SetSpeakerConfig)     (THIS_ DWORD) PURE;
    STDMETHOD(Initialize)           (THIS_ const GUID*) PURE;
};

#undef INTERFACE
#define INTERFACE IDirectSoundBuffer
DECLARE_INTERFACE_(IDirectSoundBuffer, IUnknown)
{
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;
    STDMETHOD(GetCaps)              (THIS_ void*) PURE;
    STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD, LPDWORD) PURE;
    STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX, DWORD, LPDWORD) PURE;
    STDMETHOD(GetVolume)            (THIS_ LPLONG) PURE;
    STDMETHOD(GetPan)               (THIS_ LPLONG) PURE;
    STDMETHOD(GetFrequency)         (THIS_ LPDWORD) PURE;
    STDMETHOD(GetStatus)            (THIS_ LPDWORD) PURE;
    STDMETHOD(Initialize)           (THIS_ IDirectSound*, DSBUFFERDESC*) PURE;
    STDMETHOD(Lock)                 (THIS_ DWORD, DWORD, LPVOID*, LPDWORD, LPVOID*, LPDWORD, DWORD) PURE;
    STDMETHOD(Play)                 (THIS_ DWORD, DWORD, DWORD) PURE;
    STDMETHOD(SetCurrentPosition)   (THIS_ DWORD) PURE;
    STDMETHOD(SetFormat)            (THIS_ const WAVEFORMATEX*) PURE;
    STDMETHOD(SetVolume)            (THIS_ LONG) PURE;
    STDMETHOD(SetPan)               (THIS_ LONG) PURE;
    STDMETHOD(SetFrequency)         (THIS_ DWORD) PURE;
    STDMETHOD(Stop)                 (THIS) PURE;
    STDMETHOD(Unlock)               (THIS_ LPVOID, DWORD, LPVOID, DWORD) PURE;
    STDMETHOD(Restore)              (THIS) PURE;
};

//==============================================================================
typedef struct typeDSCBUFFERDESC
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwBufferBytes;
    DWORD dwReserved;
    LPWAVEFORMATEX lpwfxFormat;
} DSCBUFFERDESC;

struct IDirectSoundCaptureBuffer;

#undef INTERFACE
#define INTERFACE IDirectSoundCapture
DECLARE_INTERFACE_(IDirectSoundCapture, IUnknown)
{
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;
    STDMETHOD(CreateCaptureBuffer)  (THIS_ DSCBUFFERDESC*, IDirectSoundCaptureBuffer**, LPUNKNOWN) PURE;
    STDMETHOD(GetCaps)              (THIS_ void*) PURE;
    STDMETHOD(Initialize)           (THIS_ const GUID*) PURE;
};

#undef INTERFACE
#define INTERFACE IDirectSoundCaptureBuffer
DECLARE_INTERFACE_(IDirectSoundCaptureBuffer, IUnknown)
{
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID*) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;
    STDMETHOD(GetCaps)              (THIS_ void*) PURE;
    STDMETHOD(GetCurrentPosition)   (THIS_ LPDWORD, LPDWORD) PURE;
    STDMETHOD(GetFormat)            (THIS_ LPWAVEFORMATEX, DWORD, LPDWORD) PURE;
    STDMETHOD(GetStatus)            (THIS_ LPDWORD) PURE;
    STDMETHOD(Initialize)           (THIS_ IDirectSoundCapture*, DSCBUFFERDESC*) PURE;
    STDMETHOD(Lock)                 (THIS_ DWORD, DWORD, LPVOID*, LPDWORD, LPVOID*, LPDWORD, DWORD) PURE;
    STDMETHOD(Start)                (THIS_ DWORD) PURE;
    STDMETHOD(Stop)                 (THIS) PURE;
    STDMETHOD(Unlock)               (THIS_ LPVOID, DWORD, LPVOID, DWORD) PURE;
};

};

//==============================================================================
BEGIN_JUCE_NAMESPACE

namespace
{
    String getDSErrorMessage (HRESULT hr)
    {
        const char* result = nullptr;

        switch (hr)
        {
            case MAKE_HRESULT(1, 0x878, 10):    result = "Device already allocated"; break;
            case MAKE_HRESULT(1, 0x878, 30):    result = "Control unavailable"; break;
            case E_INVALIDARG:                  result = "Invalid parameter"; break;
            case MAKE_HRESULT(1, 0x878, 50):    result = "Invalid call"; break;
            case E_FAIL:                        result = "Generic error"; break;
            case MAKE_HRESULT(1, 0x878, 70):    result = "Priority level error"; break;
            case E_OUTOFMEMORY:                 result = "Out of memory"; break;
            case MAKE_HRESULT(1, 0x878, 100):   result = "Bad format"; break;
            case E_NOTIMPL:                     result = "Unsupported function"; break;
            case MAKE_HRESULT(1, 0x878, 120):   result = "No driver"; break;
            case MAKE_HRESULT(1, 0x878, 130):   result = "Already initialised"; break;
            case CLASS_E_NOAGGREGATION:         result = "No aggregation"; break;
            case MAKE_HRESULT(1, 0x878, 150):   result = "Buffer lost"; break;
            case MAKE_HRESULT(1, 0x878, 160):   result = "Another app has priority"; break;
            case MAKE_HRESULT(1, 0x878, 170):   result = "Uninitialised"; break;
            case E_NOINTERFACE:                 result = "No interface"; break;
            case S_OK:                          result = "No error"; break;
            default:                            return "Unknown error: " + String ((int) hr);
        }

        return result;
    }

    //==============================================================================
    #define DS_DEBUGGING 1

    #ifdef DS_DEBUGGING
        #define CATCH JUCE_CATCH_EXCEPTION
        #undef log
        #define log(a) Logger::writeToLog(a);
        #undef logError
        #define logError(a) logDSError(a, __LINE__);

        static void logDSError (HRESULT hr, int lineNum)
        {
            if (hr != S_OK)
            {
                String error ("DS error at line ");
                error << lineNum << " - " << getDSErrorMessage (hr);
                log (error);
            }
        }
    #else
        #define CATCH JUCE_CATCH_ALL
        #define log(a)
        #define logError(a)
    #endif

    //==============================================================================
    #define DSOUND_FUNCTION(functionName, params) \
        typedef HRESULT (WINAPI *type##functionName) params; \
        static type##functionName ds##functionName = 0;

    #define DSOUND_FUNCTION_LOAD(functionName) \
        ds##functionName = (type##functionName) GetProcAddress (h, #functionName);  \
        jassert (ds##functionName != 0);

    typedef BOOL (CALLBACK *LPDSENUMCALLBACKW) (LPGUID, LPCWSTR, LPCWSTR, LPVOID);
    typedef BOOL (CALLBACK *LPDSENUMCALLBACKA) (LPGUID, LPCSTR, LPCSTR, LPVOID);

    DSOUND_FUNCTION (DirectSoundCreate, (const GUID*, IDirectSound**, LPUNKNOWN))
    DSOUND_FUNCTION (DirectSoundCaptureCreate, (const GUID*, IDirectSoundCapture**, LPUNKNOWN))
    DSOUND_FUNCTION (DirectSoundEnumerateW, (LPDSENUMCALLBACKW, LPVOID))
    DSOUND_FUNCTION (DirectSoundCaptureEnumerateW, (LPDSENUMCALLBACKW, LPVOID))

    void initialiseDSoundFunctions()
    {
        if (dsDirectSoundCreate == 0)
        {
            HMODULE h = LoadLibraryA ("dsound.dll");

            DSOUND_FUNCTION_LOAD (DirectSoundCreate)
            DSOUND_FUNCTION_LOAD (DirectSoundCaptureCreate)
            DSOUND_FUNCTION_LOAD (DirectSoundEnumerateW)
            DSOUND_FUNCTION_LOAD (DirectSoundCaptureEnumerateW)
        }
    }
}

//==============================================================================
class DSoundInternalOutChannel
{
public:
    DSoundInternalOutChannel (const String& name_, const GUID& guid_, int rate,
                              int bufferSize, float* left, float* right)
        : bitDepth (16), name (name_), guid (guid_), sampleRate (rate),
          bufferSizeSamples (bufferSize), leftBuffer (left), rightBuffer (right),
          pDirectSound (nullptr), pOutputBuffer (nullptr)
    {
    }

    ~DSoundInternalOutChannel()
    {
        close();
    }

    void close()
    {
        HRESULT hr;

        if (pOutputBuffer != nullptr)
        {
            log ("closing dsound out: " + name);
            hr = pOutputBuffer->Stop();
            logError (hr);

            hr = pOutputBuffer->Release();
            pOutputBuffer = nullptr;
            logError (hr);
        }

        if (pDirectSound != nullptr)
        {
            hr = pDirectSound->Release();
            pDirectSound = nullptr;
            logError (hr);
        }
    }

    String open()
    {
        log ("opening dsound out device: " + name + "  rate=" + String (sampleRate)
              + " bits=" + String (bitDepth) + " buf=" + String (bufferSizeSamples));

        pDirectSound = nullptr;
        pOutputBuffer = nullptr;
        writeOffset = 0;

        String error;
        HRESULT hr = E_NOINTERFACE;

        if (dsDirectSoundCreate != 0)
            hr = dsDirectSoundCreate (&guid, &pDirectSound, 0);

        if (hr == S_OK)
        {
            bytesPerBuffer = (bufferSizeSamples * (bitDepth >> 2)) & ~15;
            totalBytesPerBuffer = (3 * bytesPerBuffer) & ~15;
            const int numChannels = 2;

            hr = pDirectSound->SetCooperativeLevel (GetDesktopWindow(), 2 /* DSSCL_PRIORITY  */);
            logError (hr);

            if (hr == S_OK)
            {
                IDirectSoundBuffer* pPrimaryBuffer;

                DSBUFFERDESC primaryDesc = { 0 };
                primaryDesc.dwSize = sizeof (DSBUFFERDESC);
                primaryDesc.dwFlags = 1 /* DSBCAPS_PRIMARYBUFFER */;
                primaryDesc.dwBufferBytes = 0;
                primaryDesc.lpwfxFormat = 0;

                log ("opening dsound out step 2");
                hr = pDirectSound->CreateSoundBuffer (&primaryDesc, &pPrimaryBuffer, 0);
                logError (hr);

                if (hr == S_OK)
                {
                    WAVEFORMATEX wfFormat;
                    wfFormat.wFormatTag = WAVE_FORMAT_PCM;
                    wfFormat.nChannels = (unsigned short) numChannels;
                    wfFormat.nSamplesPerSec = sampleRate;
                    wfFormat.wBitsPerSample = (unsigned short) bitDepth;
                    wfFormat.nBlockAlign = (unsigned short) (wfFormat.nChannels * wfFormat.wBitsPerSample / 8);
                    wfFormat.nAvgBytesPerSec = wfFormat.nSamplesPerSec * wfFormat.nBlockAlign;
                    wfFormat.cbSize = 0;

                    hr = pPrimaryBuffer->SetFormat (&wfFormat);
                    logError (hr);

                    if (hr == S_OK)
                    {
                        DSBUFFERDESC secondaryDesc = { 0 };
                        secondaryDesc.dwSize = sizeof (DSBUFFERDESC);
                        secondaryDesc.dwFlags =  0x8000 /* DSBCAPS_GLOBALFOCUS */
                                                  | 0x10000 /* DSBCAPS_GETCURRENTPOSITION2 */;
                        secondaryDesc.dwBufferBytes = totalBytesPerBuffer;
                        secondaryDesc.lpwfxFormat = &wfFormat;

                        hr = pDirectSound->CreateSoundBuffer (&secondaryDesc, &pOutputBuffer, 0);
                        logError (hr);

                        if (hr == S_OK)
                        {
                            log ("opening dsound out step 3");

                            DWORD dwDataLen;
                            unsigned char* pDSBuffData;

                            hr = pOutputBuffer->Lock (0, totalBytesPerBuffer,
                                                      (LPVOID*) &pDSBuffData, &dwDataLen, 0, 0, 0);
                            logError (hr);

                            if (hr == S_OK)
                            {
                                zeromem (pDSBuffData, dwDataLen);

                                hr = pOutputBuffer->Unlock (pDSBuffData, dwDataLen, 0, 0);

                                if (hr == S_OK)
                                {
                                    hr = pOutputBuffer->SetCurrentPosition (0);

                                    if (hr == S_OK)
                                    {
                                        hr = pOutputBuffer->Play (0, 0, 1 /* DSBPLAY_LOOPING */);

                                        if (hr == S_OK)
                                            return String::empty;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        error = getDSErrorMessage (hr);
        close();
        return error;
    }

    void synchronisePosition()
    {
        if (pOutputBuffer != nullptr)
        {
            DWORD playCursor;
            pOutputBuffer->GetCurrentPosition (&playCursor, &writeOffset);
        }
    }

    bool service()
    {
        if (pOutputBuffer == 0)
            return true;

        DWORD playCursor, writeCursor;

        for (;;)
        {
            HRESULT hr = pOutputBuffer->GetCurrentPosition (&playCursor, &writeCursor);

            if (hr == MAKE_HRESULT (1, 0x878, 150)) // DSERR_BUFFERLOST
            {
                pOutputBuffer->Restore();
                continue;
            }

            if (hr == S_OK)
                break;

            logError (hr);
            jassertfalse;
            return true;
        }

        int playWriteGap = writeCursor - playCursor;
        if (playWriteGap < 0)
            playWriteGap += totalBytesPerBuffer;

        int bytesEmpty = playCursor - writeOffset;

        if (bytesEmpty < 0)
            bytesEmpty += totalBytesPerBuffer;

        if (bytesEmpty > (totalBytesPerBuffer - playWriteGap))
        {
            writeOffset = writeCursor;
            bytesEmpty = totalBytesPerBuffer - playWriteGap;
        }

        if (bytesEmpty >= bytesPerBuffer)
        {
            void* lpbuf1 = nullptr;
            void* lpbuf2 = nullptr;
            DWORD dwSize1 = 0;
            DWORD dwSize2 = 0;

            HRESULT hr = pOutputBuffer->Lock (writeOffset, bytesPerBuffer,
                                              &lpbuf1, &dwSize1,
                                              &lpbuf2, &dwSize2, 0);

            if (hr == MAKE_HRESULT (1, 0x878, 150)) // DSERR_BUFFERLOST
            {
                pOutputBuffer->Restore();

                hr = pOutputBuffer->Lock (writeOffset, bytesPerBuffer,
                                          &lpbuf1, &dwSize1,
                                          &lpbuf2, &dwSize2, 0);
            }

            if (hr == S_OK)
            {
                if (bitDepth == 16)
                {
                    int* dest = static_cast<int*> (lpbuf1);
                    const float* left = leftBuffer;
                    const float* right = rightBuffer;
                    int samples1 = dwSize1 >> 2;
                    int samples2 = dwSize2 >> 2;

                    if (left == 0)
                    {
                        while (--samples1 >= 0)
                            *dest++ = (convertInputValue (*right++) << 16);

                        dest = static_cast<int*> (lpbuf2);

                        while (--samples2 >= 0)
                            *dest++ = (convertInputValue (*right++) << 16);
                    }
                    else if (right == 0)
                    {
                        while (--samples1 >= 0)
                            *dest++ = (0xffff & convertInputValue (*left++));

                        dest = static_cast<int*> (lpbuf2);

                        while (--samples2 >= 0)
                            *dest++ = (0xffff & convertInputValue (*left++));
                    }
                    else
                    {
                        while (--samples1 >= 0)
                        {
                            const int l = convertInputValue (*left++);
                            const int r = convertInputValue (*right++);
                            *dest++ = (r << 16) | (0xffff & l);
                        }

                        dest = static_cast<int*> (lpbuf2);

                        while (--samples2 >= 0)
                        {
                            const int l = convertInputValue (*left++);
                            const int r = convertInputValue (*right++);
                            *dest++ = (r << 16) | (0xffff & l);
                        }
                    }
                }
                else
                {
                    jassertfalse;
                }

                writeOffset = (writeOffset + dwSize1 + dwSize2) % totalBytesPerBuffer;

                pOutputBuffer->Unlock (lpbuf1, dwSize1, lpbuf2, dwSize2);
            }
            else
            {
                jassertfalse;
                logError (hr);
            }

            bytesEmpty -= bytesPerBuffer;
            return true;
        }
        else
        {
            return false;
        }
    }

    int bitDepth;
    bool doneFlag;

private:
    String name;
    GUID guid;
    int sampleRate, bufferSizeSamples;
    float* leftBuffer;
    float* rightBuffer;

    IDirectSound* pDirectSound;
    IDirectSoundBuffer* pOutputBuffer;
    DWORD writeOffset;
    int totalBytesPerBuffer, bytesPerBuffer;
    unsigned int lastPlayCursor;

    static inline int convertInputValue (const float v) noexcept
    {
        return jlimit (-32768, 32767, roundToInt (32767.0f * v));
    }

    JUCE_DECLARE_NON_COPYABLE (DSoundInternalOutChannel);
};

//==============================================================================
struct DSoundInternalInChannel
{
public:
    DSoundInternalInChannel (const String& name_, const GUID& guid_, int rate,
                             int bufferSize, float* left, float* right)
        : bitDepth (16), name (name_), guid (guid_), sampleRate (rate),
          bufferSizeSamples (bufferSize), leftBuffer (left), rightBuffer (right),
          pDirectSound (nullptr), pDirectSoundCapture (nullptr), pInputBuffer (nullptr)
    {
    }

    ~DSoundInternalInChannel()
    {
        close();
    }

    void close()
    {
        HRESULT hr;

        if (pInputBuffer != nullptr)
        {
            log ("closing dsound in: " + name);
            hr = pInputBuffer->Stop();
            logError (hr);

            hr = pInputBuffer->Release();
            pInputBuffer = nullptr;
            logError (hr);
        }

        if (pDirectSoundCapture != nullptr)
        {
            hr = pDirectSoundCapture->Release();
            pDirectSoundCapture = nullptr;
            logError (hr);
        }

        if (pDirectSound != nullptr)
        {
            hr = pDirectSound->Release();
            pDirectSound = nullptr;
            logError (hr);
        }
    }

    String open()
    {
        log ("opening dsound in device: " + name
              + "  rate=" + String (sampleRate) + " bits=" + String (bitDepth) + " buf=" + String (bufferSizeSamples));

        pDirectSound = nullptr;
        pDirectSoundCapture = nullptr;
        pInputBuffer = nullptr;
        readOffset = 0;
        totalBytesPerBuffer = 0;

        String error;
        HRESULT hr = E_NOINTERFACE;

        if (dsDirectSoundCaptureCreate != 0)
            hr = dsDirectSoundCaptureCreate (&guid, &pDirectSoundCapture, 0);

        logError (hr);

        if (hr == S_OK)
        {
            const int numChannels = 2;
            bytesPerBuffer = (bufferSizeSamples * (bitDepth >> 2)) & ~15;
            totalBytesPerBuffer = (3 * bytesPerBuffer) & ~15;

            WAVEFORMATEX wfFormat;
            wfFormat.wFormatTag = WAVE_FORMAT_PCM;
            wfFormat.nChannels = (unsigned short)numChannels;
            wfFormat.nSamplesPerSec = sampleRate;
            wfFormat.wBitsPerSample = (unsigned short)bitDepth;
            wfFormat.nBlockAlign = (unsigned short)(wfFormat.nChannels * (wfFormat.wBitsPerSample / 8));
            wfFormat.nAvgBytesPerSec = wfFormat.nSamplesPerSec * wfFormat.nBlockAlign;
            wfFormat.cbSize = 0;

            DSCBUFFERDESC captureDesc = { 0 };
            captureDesc.dwSize = sizeof (DSCBUFFERDESC);
            captureDesc.dwFlags = 0;
            captureDesc.dwBufferBytes = totalBytesPerBuffer;
            captureDesc.lpwfxFormat = &wfFormat;

            log ("opening dsound in step 2");
            hr = pDirectSoundCapture->CreateCaptureBuffer (&captureDesc, &pInputBuffer, 0);

            logError (hr);

            if (hr == S_OK)
            {
                hr = pInputBuffer->Start (1 /* DSCBSTART_LOOPING */);
                logError (hr);

                if (hr == S_OK)
                    return String::empty;
            }
        }

        error = getDSErrorMessage (hr);
        close();

        return error;
    }

    void synchronisePosition()
    {
        if (pInputBuffer != nullptr)
        {
            DWORD capturePos;
            pInputBuffer->GetCurrentPosition (&capturePos, (DWORD*)&readOffset);
        }
    }

    bool service()
    {
        if (pInputBuffer == 0)
            return true;

        DWORD capturePos, readPos;
        HRESULT hr = pInputBuffer->GetCurrentPosition (&capturePos, &readPos);
        logError (hr);

        if (hr != S_OK)
            return true;

        int bytesFilled = readPos - readOffset;
        if (bytesFilled < 0)
            bytesFilled += totalBytesPerBuffer;

        if (bytesFilled >= bytesPerBuffer)
        {
            LPBYTE lpbuf1 = nullptr;
            LPBYTE lpbuf2 = nullptr;
            DWORD dwsize1 = 0;
            DWORD dwsize2 = 0;

            HRESULT hr = pInputBuffer->Lock (readOffset, bytesPerBuffer,
                                             (void**) &lpbuf1, &dwsize1,
                                             (void**) &lpbuf2, &dwsize2, 0);

            if (hr == S_OK)
            {
                if (bitDepth == 16)
                {
                    const float g = 1.0f / 32768.0f;

                    float* destL = leftBuffer;
                    float* destR = rightBuffer;
                    int samples1 = dwsize1 >> 2;
                    int samples2 = dwsize2 >> 2;

                    const short* src = (const short*)lpbuf1;

                    if (destL == 0)
                    {
                        while (--samples1 >= 0)
                        {
                            ++src;
                            *destR++ = *src++ * g;
                        }

                        src = (const short*)lpbuf2;

                        while (--samples2 >= 0)
                        {
                            ++src;
                            *destR++ = *src++ * g;
                        }
                    }
                    else if (destR == 0)
                    {
                        while (--samples1 >= 0)
                        {
                            *destL++ = *src++ * g;
                            ++src;
                        }

                        src = (const short*)lpbuf2;

                        while (--samples2 >= 0)
                        {
                            *destL++ = *src++ * g;
                            ++src;
                        }
                    }
                    else
                    {
                        while (--samples1 >= 0)
                        {
                            *destL++ = *src++ * g;
                            *destR++ = *src++ * g;
                        }

                        src = (const short*)lpbuf2;

                        while (--samples2 >= 0)
                        {
                            *destL++ = *src++ * g;
                            *destR++ = *src++ * g;
                        }
                    }
                }
                else
                {
                    jassertfalse;
                }

                readOffset = (readOffset + dwsize1 + dwsize2) % totalBytesPerBuffer;

                pInputBuffer->Unlock (lpbuf1, dwsize1, lpbuf2, dwsize2);
            }
            else
            {
                logError (hr);
                jassertfalse;
            }

            bytesFilled -= bytesPerBuffer;

            return true;
        }
        else
        {
            return false;
        }
    }

    unsigned int readOffset;
    int bytesPerBuffer, totalBytesPerBuffer;
    int bitDepth;
    bool doneFlag;

private:
    String name;
    GUID guid;
    int sampleRate, bufferSizeSamples;
    float* leftBuffer;
    float* rightBuffer;

    IDirectSound* pDirectSound;
    IDirectSoundCapture* pDirectSoundCapture;
    IDirectSoundCaptureBuffer* pInputBuffer;

    JUCE_DECLARE_NON_COPYABLE (DSoundInternalInChannel);
};

//==============================================================================
class DSoundAudioIODevice  : public AudioIODevice,
                             public Thread
{
public:
    DSoundAudioIODevice (const String& deviceName,
                         const int outputDeviceIndex_,
                         const int inputDeviceIndex_)
        : AudioIODevice (deviceName, "DirectSound"),
          Thread ("Juce DSound"),
          outputDeviceIndex (outputDeviceIndex_),
          inputDeviceIndex (inputDeviceIndex_),
          isOpen_ (false),
          isStarted (false),
          bufferSizeSamples (0),
          totalSamplesOut (0),
          sampleRate (0.0),
          inputBuffers (1, 1),
          outputBuffers (1, 1),
          callback (nullptr)
    {
        if (outputDeviceIndex_ >= 0)
        {
            outChannels.add (TRANS("Left"));
            outChannels.add (TRANS("Right"));
        }

        if (inputDeviceIndex_ >= 0)
        {
            inChannels.add (TRANS("Left"));
            inChannels.add (TRANS("Right"));
        }
    }

    ~DSoundAudioIODevice()
    {
        close();
    }

    const String open (const BigInteger& inputChannels,
                       const BigInteger& outputChannels,
                       double sampleRate, int bufferSizeSamples)
    {
        lastError = openDevice (inputChannels, outputChannels, sampleRate, bufferSizeSamples);
        isOpen_ = lastError.isEmpty();

        return lastError;
    }

    void close()
    {
        stop();

        if (isOpen_)
        {
            closeDevice();
            isOpen_ = false;
        }
    }

    bool isOpen()                                       { return isOpen_ && isThreadRunning(); }
    int getCurrentBufferSizeSamples()                   { return bufferSizeSamples; }
    double getCurrentSampleRate()                       { return sampleRate; }
    const BigInteger getActiveOutputChannels() const    { return enabledOutputs; }
    const BigInteger getActiveInputChannels() const     { return enabledInputs; }
    int getOutputLatencyInSamples()                     { return (int) (getCurrentBufferSizeSamples() * 1.5); }
    int getInputLatencyInSamples()                      { return getOutputLatencyInSamples(); }
    StringArray getOutputChannelNames()                 { return outChannels; }
    StringArray getInputChannelNames()                  { return inChannels; }

    int getNumSampleRates()                             { return 4; }
    int getDefaultBufferSize()                          { return 2560; }
    int getNumBufferSizesAvailable()                    { return 50; }

    double getSampleRate (int index)
    {
        const double samps[] = { 44100.0, 48000.0, 88200.0, 96000.0 };
        return samps [jlimit (0, 3, index)];
    }

    int getBufferSizeSamples (int index)
    {
        int n = 64;
        for (int i = 0; i < index; ++i)
            n += (n < 512) ? 32
                           : ((n < 1024) ? 64
                                         : ((n < 2048) ? 128 : 256));

        return n;
    }

    int getCurrentBitDepth()
    {
        int i, bits = 256;

        for (i = inChans.size(); --i >= 0;)
            bits = jmin (bits, inChans[i]->bitDepth);

        for (i = outChans.size(); --i >= 0;)
            bits = jmin (bits, outChans[i]->bitDepth);

        if (bits > 32)
            bits = 16;

        return bits;
    }

    void start (AudioIODeviceCallback* call)
    {
        if (isOpen_ && call != nullptr && ! isStarted)
        {
            if (! isThreadRunning())
            {
                // something gone wrong and the thread's stopped..
                isOpen_ = false;
                return;
            }

            call->audioDeviceAboutToStart (this);

            const ScopedLock sl (startStopLock);
            callback = call;
            isStarted = true;
        }
    }

    void stop()
    {
        if (isStarted)
        {
            AudioIODeviceCallback* const callbackLocal = callback;

            {
                const ScopedLock sl (startStopLock);
                isStarted = false;
            }

            if (callbackLocal != nullptr)
                callbackLocal->audioDeviceStopped();
        }
    }

    bool isPlaying()                { return isStarted && isOpen_ && isThreadRunning(); }
    const String getLastError()     { return lastError; }

    //==============================================================================
    StringArray inChannels, outChannels;
    int outputDeviceIndex, inputDeviceIndex;

private:
    bool isOpen_;
    bool isStarted;
    String lastError;

    OwnedArray <DSoundInternalInChannel> inChans;
    OwnedArray <DSoundInternalOutChannel> outChans;
    WaitableEvent startEvent;

    int bufferSizeSamples;
    int volatile totalSamplesOut;
    int64 volatile lastBlockTime;
    double sampleRate;
    BigInteger enabledInputs, enabledOutputs;
    AudioSampleBuffer inputBuffers, outputBuffers;

    AudioIODeviceCallback* callback;
    CriticalSection startStopLock;

    String openDevice (const BigInteger& inputChannels,
                       const BigInteger& outputChannels,
                       double sampleRate_, int bufferSizeSamples_);

    void closeDevice()
    {
        isStarted = false;
        stopThread (5000);

        inChans.clear();
        outChans.clear();
        inputBuffers.setSize (1, 1);
        outputBuffers.setSize (1, 1);
    }

    void resync()
    {
        if (! threadShouldExit())
        {
            sleep (5);

            int i;
            for (i = 0; i < outChans.size(); ++i)
                outChans.getUnchecked(i)->synchronisePosition();

            for (i = 0; i < inChans.size(); ++i)
                inChans.getUnchecked(i)->synchronisePosition();
        }
    }

public:
    void run()
    {
        while (! threadShouldExit())
        {
            if (wait (100))
                break;
        }

        const int latencyMs = (int) (bufferSizeSamples * 1000.0 / sampleRate);
        const int maxTimeMS = jmax (5, 3 * latencyMs);

        while (! threadShouldExit())
        {
            int numToDo = 0;
            uint32 startTime = Time::getMillisecondCounter();

            int i;
            for (i = inChans.size(); --i >= 0;)
            {
                inChans.getUnchecked(i)->doneFlag = false;
                ++numToDo;
            }

            for (i = outChans.size(); --i >= 0;)
            {
                outChans.getUnchecked(i)->doneFlag = false;
                ++numToDo;
            }

            if (numToDo > 0)
            {
                const int maxCount = 3;
                int count = maxCount;

                for (;;)
                {
                    for (i = inChans.size(); --i >= 0;)
                    {
                        DSoundInternalInChannel* const in = inChans.getUnchecked(i);

                        if ((! in->doneFlag) && in->service())
                        {
                            in->doneFlag = true;
                            --numToDo;
                        }
                    }

                    for (i = outChans.size(); --i >= 0;)
                    {
                        DSoundInternalOutChannel* const out = outChans.getUnchecked(i);

                        if ((! out->doneFlag) && out->service())
                        {
                            out->doneFlag = true;
                            --numToDo;
                        }
                    }

                    if (numToDo <= 0)
                        break;

                    if (Time::getMillisecondCounter() > startTime + maxTimeMS)
                    {
                        resync();
                        break;
                    }

                    if (--count <= 0)
                    {
                        Sleep (1);
                        count = maxCount;
                    }

                    if (threadShouldExit())
                        return;
                }
            }
            else
            {
                sleep (1);
            }

            const ScopedLock sl (startStopLock);

            if (isStarted)
            {
                JUCE_TRY
                {
                    callback->audioDeviceIOCallback (const_cast <const float**> (inputBuffers.getArrayOfChannels()),
                                                     inputBuffers.getNumChannels(),
                                                     outputBuffers.getArrayOfChannels(),
                                                     outputBuffers.getNumChannels(),
                                                     bufferSizeSamples);
                }
                JUCE_CATCH_EXCEPTION

                totalSamplesOut += bufferSizeSamples;
            }
            else
            {
                outputBuffers.clear();
                totalSamplesOut = 0;
                sleep (1);
            }
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSoundAudioIODevice);
};

//==============================================================================
struct DSoundDeviceList
{
    StringArray outputDeviceNames, inputDeviceNames;
    Array<GUID> outputGuids, inputGuids;

    void scan()
    {
        outputDeviceNames.clear();
        inputDeviceNames.clear();
        outputGuids.clear();
        inputGuids.clear();

        if (dsDirectSoundEnumerateW != 0)
        {
            dsDirectSoundEnumerateW (outputEnumProcW, this);
            dsDirectSoundCaptureEnumerateW (inputEnumProcW, this);
        }
    }

    bool operator!= (const DSoundDeviceList& other) const noexcept
    {
        return outputDeviceNames != other.outputDeviceNames
            || inputDeviceNames != other.inputDeviceNames
            || outputGuids != other.outputGuids
            || inputGuids != other.inputGuids;
    }

private:
    static BOOL enumProc (LPGUID lpGUID, String desc, StringArray& names, Array<GUID>& guids)
    {
        desc = desc.trim();

        if (desc.isNotEmpty())
        {
            const String origDesc (desc);

            int n = 2;
            while (names.contains (desc))
                desc = origDesc + " (" + String (n++) + ")";

            names.add (desc);
            guids.add (lpGUID != nullptr ? *lpGUID : GUID());
        }

        return TRUE;
    }

    BOOL outputEnumProc (LPGUID guid, LPCWSTR desc)  { return enumProc (guid, desc, outputDeviceNames, outputGuids); }
    BOOL inputEnumProc (LPGUID guid, LPCWSTR desc)   { return enumProc (guid, desc, inputDeviceNames, inputGuids); }

    static BOOL CALLBACK outputEnumProcW (LPGUID lpGUID, LPCWSTR description, LPCWSTR, LPVOID object)
    {
        return static_cast<DSoundDeviceList*> (object)->outputEnumProc (lpGUID, description);
    }

    static BOOL CALLBACK inputEnumProcW (LPGUID lpGUID, LPCWSTR description, LPCWSTR, LPVOID object)
    {
        return static_cast<DSoundDeviceList*> (object)->inputEnumProc (lpGUID, description);
    }
};

//==============================================================================
String DSoundAudioIODevice::openDevice (const BigInteger& inputChannels,
                                        const BigInteger& outputChannels,
                                        double sampleRate_, int bufferSizeSamples_)
{
    closeDevice();
    totalSamplesOut = 0;

    sampleRate = sampleRate_;

    if (bufferSizeSamples_ <= 0)
        bufferSizeSamples_ = 960; // use as a default size if none is set.

    bufferSizeSamples = bufferSizeSamples_ & ~7;

    DSoundDeviceList dlh;
    dlh.scan();

    enabledInputs = inputChannels;
    enabledInputs.setRange (inChannels.size(),
                            enabledInputs.getHighestBit() + 1 - inChannels.size(),
                            false);

    inputBuffers.setSize (jmax (1, enabledInputs.countNumberOfSetBits()), bufferSizeSamples);
    inputBuffers.clear();
    int i, numIns = 0;

    for (i = 0; i <= enabledInputs.getHighestBit(); i += 2)
    {
        float* left = nullptr;
        if (enabledInputs[i])
            left = inputBuffers.getSampleData (numIns++);

        float* right = nullptr;
        if (enabledInputs[i + 1])
            right = inputBuffers.getSampleData (numIns++);

        if (left != nullptr || right != nullptr)
            inChans.add (new DSoundInternalInChannel (dlh.inputDeviceNames [inputDeviceIndex],
                                                      dlh.inputGuids [inputDeviceIndex],
                                                      (int) sampleRate, bufferSizeSamples,
                                                      left, right));
    }

    enabledOutputs = outputChannels;
    enabledOutputs.setRange (outChannels.size(),
                             enabledOutputs.getHighestBit() + 1 - outChannels.size(),
                             false);

    outputBuffers.setSize (jmax (1, enabledOutputs.countNumberOfSetBits()), bufferSizeSamples);
    outputBuffers.clear();
    int numOuts = 0;

    for (i = 0; i <= enabledOutputs.getHighestBit(); i += 2)
    {
        float* left = nullptr;
        if (enabledOutputs[i])
            left = outputBuffers.getSampleData (numOuts++);

        float* right = nullptr;
        if (enabledOutputs[i + 1])
            right = outputBuffers.getSampleData (numOuts++);

        if (left != nullptr || right != nullptr)
            outChans.add (new DSoundInternalOutChannel (dlh.outputDeviceNames[outputDeviceIndex],
                                                        dlh.outputGuids [outputDeviceIndex],
                                                        (int) sampleRate, bufferSizeSamples,
                                                        left, right));
    }

    String error;

    // boost our priority while opening the devices to try to get better sync between them
    const int oldThreadPri = GetThreadPriority (GetCurrentThread());
    const int oldProcPri = GetPriorityClass (GetCurrentProcess());
    SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    SetPriorityClass (GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

    for (i = 0; i < outChans.size(); ++i)
    {
        error = outChans[i]->open();

        if (error.isNotEmpty())
        {
            error = "Error opening " + dlh.outputDeviceNames[i] + ": \"" + error + "\"";
            break;
        }
    }

    if (error.isEmpty())
    {
        for (i = 0; i < inChans.size(); ++i)
        {
            error = inChans[i]->open();

            if (error.isNotEmpty())
            {
                error = "Error opening " + dlh.inputDeviceNames[i] + ": \"" + error + "\"";
                break;
            }
        }
    }

    if (error.isEmpty())
    {
        totalSamplesOut = 0;

        for (i = 0; i < outChans.size(); ++i)
            outChans.getUnchecked(i)->synchronisePosition();

        for (i = 0; i < inChans.size(); ++i)
            inChans.getUnchecked(i)->synchronisePosition();

        startThread (9);
        sleep (10);

        notify();
    }
    else
    {
        log (error);
    }

    SetThreadPriority (GetCurrentThread(), oldThreadPri);
    SetPriorityClass (GetCurrentProcess(), oldProcPri);

    return error;
}

//==============================================================================
class DSoundAudioIODeviceType  : public AudioIODeviceType,
                                 private DeviceChangeDetector
{
public:
    DSoundAudioIODeviceType()
        : AudioIODeviceType ("DirectSound"),
          DeviceChangeDetector (L"DirectSound"),
          hasScanned (false)
    {
        initialiseDSoundFunctions();
    }

    //==============================================================================
    void scanForDevices()
    {
        hasScanned = true;
        deviceList.scan();
    }

    StringArray getDeviceNames (bool wantInputNames) const
    {
        jassert (hasScanned); // need to call scanForDevices() before doing this

        return wantInputNames ? deviceList.inputDeviceNames
                              : deviceList.outputDeviceNames;
    }

    int getDefaultDeviceIndex (bool /*forInput*/) const
    {
        jassert (hasScanned); // need to call scanForDevices() before doing this
        return 0;
    }

    int getIndexOfDevice (AudioIODevice* device, bool asInput) const
    {
        jassert (hasScanned); // need to call scanForDevices() before doing this

        DSoundAudioIODevice* const d = dynamic_cast <DSoundAudioIODevice*> (device);
        if (d == 0)
            return -1;

        return asInput ? d->inputDeviceIndex
                       : d->outputDeviceIndex;
    }

    bool hasSeparateInputsAndOutputs() const    { return true; }

    AudioIODevice* createDevice (const String& outputDeviceName,
                                 const String& inputDeviceName)
    {
        jassert (hasScanned); // need to call scanForDevices() before doing this

        const int outputIndex = deviceList.outputDeviceNames.indexOf (outputDeviceName);
        const int inputIndex = deviceList.inputDeviceNames.indexOf (inputDeviceName);

        if (outputIndex >= 0 || inputIndex >= 0)
            return new DSoundAudioIODevice (outputDeviceName.isNotEmpty() ? outputDeviceName
                                                                          : inputDeviceName,
                                            outputIndex, inputIndex);

        return nullptr;
    }

private:
    //==============================================================================
    DSoundDeviceList deviceList;
    bool hasScanned;

    void systemDeviceChanged()
    {
        DSoundDeviceList newList;
        newList.scan();

        if (newList != deviceList)
        {
            deviceList = newList;
            callDeviceChangeListeners();
        }
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSoundAudioIODeviceType);
};

//==============================================================================
AudioIODeviceType* AudioIODeviceType::createAudioIODeviceType_DirectSound()
{
    return new DSoundAudioIODeviceType();
}

#undef log

#endif
