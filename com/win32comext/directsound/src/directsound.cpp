// directsound.cpp :
// $Id$

// directsound wrapper contributed by Lars Immisch <lars@ibp.de>

/***
Note that this source file contains embedded documentation.
This documentation consists of marked up text inside the
C comments, and is prefixed with an '@' symbol.  The source
files are processed by a tool called "autoduck" which
generates Windows .hlp files.
@doc
***/

#include "directsound_pch.h"
#include "stddef.h" // for offsetof
#include "PythonCOMRegister.h" // For simpler registration of IIDs etc.
#include "PyIDirectSound.h"
#include "PyIDirectSoundBuffer.h"
#include "PyIDirectSoundNotify.h"


// @pymethod <o PyIUnknown>|directsound|DirectSoundCreate|Creates and initializes a new object that supports the IDirectSound interface.
static PyObject *directsound_DirectSoundCreate(PyObject *, PyObject *args)
{
	PyObject *ret = NULL;
	PyObject *obGUID = NULL, *obUnk = NULL;
	IUnknown *pUnkIn = NULL;
	GUID guid, *pguid = NULL;
	LPDIRECTSOUND ds;
	HRESULT hr;

	if (!PyArg_ParseTuple(args, "|OO:DirectSoundCreate", 
		&obGUID, // @pyparm <o PyIID>|guid|None|Address of the GUID that identifies the sound device. The value of this parameter must be one of the GUIDs returned by DirectSoundEnumerate, or None for the default device.
		&obUnk))  // @pyparm <o PyIUknown>|unk|None|The IUnknown for COM aggregation.
	{
		return NULL;
	}

	if (obUnk)
	{
		if (!PyCom_InterfaceFromPyInstanceOrObject(obUnk, IID_IUnknown, (void **)&pUnkIn, TRUE))
			goto done;
	}

	if (obGUID && obGUID != Py_None)
	{
		if (!PyWinObject_AsIID(obGUID, &guid))
			goto done;

		pguid = &guid;
	}

	Py_BEGIN_ALLOW_THREADS
	hr = ::DirectSoundCreate(pguid, &ds, pUnkIn);
	Py_END_ALLOW_THREADS
	if (FAILED(hr)) {
		PyCom_BuildPyException(hr);
		goto done;
	}
	ret = new PyIDirectSound(ds);
done:
	if (pUnkIn)
		pUnkIn->Release();

	return ret;
}

BOOL CALLBACK dsEnumCallback(LPGUID guid, LPCSTR desc, LPCSTR module, LPVOID context)
{
	PyObject *list = (PyObject*)context;
	PyObject *item = PyTuple_New(3);
	PyObject *oguid;

	// abort enumeration if we cannot create a tuple
	if (!item)
	{
		return FALSE;
	}

	if (guid)
	{
		oguid = PyWinObject_FromIID(*guid);
	}
	else
	{
		Py_INCREF(Py_None);
		oguid = Py_None;
	}

	if (PyTuple_SetItem(item, 0, oguid))
		return FALSE;

	if (PyTuple_SetItem(item, 1, desc ? PyString_FromString(desc) : PyString_FromString("")))
		return FALSE;

	if (PyTuple_SetItem(item, 2, module ? PyString_FromString(module) : PyString_FromString("")))
		return FALSE;

	if (PyList_Append(list, item))
		return FALSE;

	return TRUE;

}

// @pymethod <o list>|directsound|DirectSoundEnumerate|Enumerates DirectSound drivers installed in the system.
static PyObject *directsound_DirectSoundEnumerate(PyObject *, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ":DirectSoundEnumerate"))
	{
		return NULL;
	}

	PyObject *list = PyList_New(0);	
	if (!list)
	{
		return NULL;
	}

	HRESULT hr;
	Py_BEGIN_ALLOW_THREADS
	hr = ::DirectSoundEnumerate(dsEnumCallback, list);
	Py_END_ALLOW_THREADS

	if (PyErr_Occurred())
	{
		return NULL;
	}

	if (FAILED(hr)) {
		PyCom_BuildPyException(hr);
		return NULL;
	}

	return list;
}

/* List of module functions */
/* @module directsound|A module encapsulating the DirectSound interfaces. See <l DirectSound examples> for a quick overview. */
static struct PyMethodDef directsound_methods[]=
{
    { "DirectSoundCreate",    directsound_DirectSoundCreate, 1 }, // @pymeth DirectSoundCreate|Creates and initializes a new object that supports the IDirectSound interface.
	{ "DirectSoundEnumerate",      directsound_DirectSoundEnumerate, 1 },      // @pymeth DirectSoundEnumerate|The DirectSoundEnumerate function enumerates the DirectSound drivers installed in the system.

//	{ "DirectSoundCaptureCreate",  directsound_DirectSoundCaptureCreate, 1},   // @pymeth DirectSoundCaptureCreate|The DirectSoundCaptureCreate function creates and initializes an object that supports the IDirectSoundCapture interface.
//	{ "DirectSoundCaptureEnumerate",  directsound_DirectSoundCaptureEnumerate, 1},   // @pymeth DirectSoundCaptureEnumerate|The DirectSoundCaptureEnumerate function enumerates the DirectSoundCapture objects installed in the system.
	{"DSCAPS",         PyWinMethod_NewDSCAPS, 1 },      // @pymeth DSCAPS|Creates a new <o PyDSCAPS> object.
	{"DSBCAPS",         PyWinMethod_NewDSBCAPS, 1 },      // @pymeth DSBCAPS|Creates a new <o PyDSBCAPS> object.
	{"DSBUFFERDESC",         PyWinMethod_NewDSBUFFERDESC, 1 },      // @pymeth DSBUFFERDESC|Creates a new <o PyDSBUFFERDESC> object.
	{ NULL, NULL },
};

static int AddConstant(PyObject *dict, const char *key, long value)
{
	PyObject *oval = PyInt_FromLong(value);
	if (!oval)
	{
		return 1;
	}
	int rc = PyDict_SetItemString(dict, (char*)key, oval);
	Py_DECREF(oval);
	return rc;
}

#define ADD_CONSTANT(tok) AddConstant(dict, #tok, tok)

static const PyCom_InterfaceSupportInfo g_interfaceSupportData[] =
{
	PYCOM_INTERFACE_CLIENT_ONLY   (DirectSound),
	PYCOM_INTERFACE_CLIENT_ONLY   (DirectSoundBuffer),
	PYCOM_INTERFACE_CLIENT_ONLY   (DirectSoundNotify),
};

/* Module initialisation */
extern "C" __declspec(dllexport) void initdirectsound()
{
	char *modName = "directsound";
	PyObject *oModule;
	// Create the module and add the functions
	oModule = Py_InitModule(modName, directsound_methods);
	if (!oModule) /* Eeek - some serious error! */
		return;
	PyObject *dict = PyModule_GetDict(oModule);
	if (!dict) return; /* Another serious error!*/

	// Register all of our interfaces, gateways and IIDs.
	PyCom_RegisterExtensionSupport(dict, g_interfaceSupportData, sizeof(g_interfaceSupportData)/sizeof(g_interfaceSupportData[0]));

	// @topic DSCAPS constants|

	// @const directsound|DSCAPS_PRIMARYMONO|The device supports monophonic primary buffers. 
	ADD_CONSTANT(DSCAPS_PRIMARYMONO); 
	// @const directsound|DSCAPS_PRIMARYSTEREO|The device supports stereo primary buffers. 
	ADD_CONSTANT(DSCAPS_PRIMARYSTEREO);
	// @const directsound|DSCAPS_PRIMARY8BIT|The device supports hardware-mixed secondary buffers with 8-bit samples. 
	ADD_CONSTANT(DSCAPS_PRIMARY8BIT);
	// @const directsound|DSCAPS_PRIMARY16BIT|The device supports primary sound buffers with 16-bit samples.
	ADD_CONSTANT(DSCAPS_PRIMARY16BIT);
	// @const directsound|DSCAPS_CONTINUOUSRATE|The device supports all sample rates between the dwMinSecondarySampleRate and dwMaxSecondarySampleRate member values. Typically, this means that the actual output rate will be within +/- 10 hertz (Hz) of the requested frequency. 
	ADD_CONSTANT(DSCAPS_CONTINUOUSRATE);
	// @const directsound|DSCAPS_EMULDRIVER|The device does not have a DirectSound driver installed, so it is being emulated through the waveform-audio functions. Performance degradation should be expected. 
	ADD_CONSTANT(DSCAPS_EMULDRIVER);
	// @const directsound|DSCAPS_CERTIFIED|This driver has been tested and certified by Microsoft. 
	ADD_CONSTANT(DSCAPS_CERTIFIED);
	// @const directsound|DSCAPS_SECONDARYMONO|The device supports hardware-mixed monophonic secondary buffers.
	ADD_CONSTANT(DSCAPS_SECONDARYMONO);
	// @const directsound|DSCAPS_SECONDARYSTEREO|The device supports hardware-mixed stereo secondary buffers. 
	ADD_CONSTANT(DSCAPS_SECONDARYSTEREO);
	// @const directsound|DSCAPS_SECONDARY8BIT|The device supports hardware-mixed secondary buffers with 8-bit samples. 
	ADD_CONSTANT(DSCAPS_SECONDARY8BIT);
	// @const directsound|DSCAPS_SECONDARY16BIT|The device supports hardware-mixed secondary sound buffers with 16-bit samples. 

	ADD_CONSTANT(DSBPLAY_LOOPING); // @const directsound|DSBPLAY_LOOPING|text. 
    ADD_CONSTANT(DSBSTATUS_PLAYING);
	ADD_CONSTANT(DSBSTATUS_BUFFERLOST);
	ADD_CONSTANT(DSBSTATUS_LOOPING);
	ADD_CONSTANT(DSBLOCK_FROMWRITECURSOR);
	ADD_CONSTANT(DSBLOCK_ENTIREBUFFER);
	ADD_CONSTANT(DSSCL_NORMAL);
	ADD_CONSTANT(DSSCL_PRIORITY);
	ADD_CONSTANT(DSSCL_EXCLUSIVE);
	ADD_CONSTANT(DSSCL_WRITEPRIMARY);
	ADD_CONSTANT(DS3DMODE_NORMAL);
	ADD_CONSTANT(DS3DMODE_HEADRELATIVE);
	ADD_CONSTANT(DS3DMODE_DISABLE);

	// @topic DSCAPS constants|

	// @const directsound|DSBCAPS_PRIMARYBUFFER|Indicates that the buffer is a primary sound buffer. If this value is not specified, a secondary sound buffer will be created. 
	ADD_CONSTANT(DSBCAPS_PRIMARYBUFFER);
	// @const directsound|DSBCAPS_STATIC|Indicates that the buffer will be used for static sound data. Typically, these buffers are loaded once and played many times. These buffers are candidates for hardware memory. 
	ADD_CONSTANT(DSBCAPS_STATIC);
	// @const directsound|DSBCAPS_LOCHARDWARE|The buffer is in hardware memory and uses hardware mixing. 
	ADD_CONSTANT(DSBCAPS_LOCHARDWARE);
	// @const directsound|DSBCAPS_LOCSOFTWARE|The buffer is in software memory and uses software mixing. 
	ADD_CONSTANT(DSBCAPS_LOCSOFTWARE);
	// @const directsound|DSBCAPS_CTRL3D|The buffer is either a primary buffer or a secondary buffer that uses 3-D control. To create a primary buffer, the dwFlags member of the DSBUFFERDESC structure should include the DSBCAPS_PRIMARYBUFFER flag. 
	ADD_CONSTANT(DSBCAPS_CTRL3D);
	// @const directsound|DSBCAPS_CTRLFREQUENCY|The buffer must have frequency control capability. 
	ADD_CONSTANT(DSBCAPS_CTRLFREQUENCY);
	// @const directsound|DSBCAPS_CTRLPAN|The buffer must have pan control capability. 
	ADD_CONSTANT(DSBCAPS_CTRLPAN);
	// @const directsound|DSBCAPS_CTRLVOLUME|The buffer must have volume control capability. 
	ADD_CONSTANT(DSBCAPS_CTRLVOLUME);
	// @const directsound|DSBCAPS_CTRLPOSITIONNOTIFY|The buffer must have control position notify capability. 
	ADD_CONSTANT(DSBCAPS_CTRLPOSITIONNOTIFY);
	// @const directsound|DSBCAPS_STICKYFOCUS|Changes the focus behavior of the sound buffer. This flag can be specified in an IDirectSound::CreateSoundBuffer call. With this flag set, an application using DirectSound can continue to play its sticky focus buffers if the user switches to another application not using DirectSound. In this situation, the application's normal buffers are muted, but the sticky focus buffers are still audible. This is useful for nongame applications, such as movie playback (DirectShow™), when the user wants to hear the soundtrack while typing in Microsoft Word or Microsoft® Excel, for example. However, if the user switches to another DirectSound application, all sound buffers, both normal and sticky focus, in the previous application are muted. 
	ADD_CONSTANT(DSBCAPS_STICKYFOCUS);
	// @const directsound|DSBCAPS_GLOBALFOCUS|The buffer is a global sound buffer. With this flag set, an application using DirectSound can continue to play its buffers if the user switches focus to another application, even if the new application uses DirectSound. The one exception is if you switch focus to a DirectSound application that uses the DSSCL_EXCLUSIVE or DSSCL_WRITEPRIMARY flag for its cooperative level. In this case, the global sounds from other applications will not be audible. 
	ADD_CONSTANT(DSBCAPS_GLOBALFOCUS);
	// @const directsound|DSBCAPS_GETCURRENTPOSITION2|Indicates that IDirectSoundBuffer::GetCurrentPosition should use the new behavior of the play cursor. In DirectSound in DirectX 1, the play cursor was significantly ahead of the actual playing sound on emulated sound cards; it was directly behind the write cursor. Now, if the DSBCAPS_GETCURRENTPOSITION2 flag is specified, the application can get a more accurate play position. If this flag is not specified, the old behavior is preserved for compatibility. Note that this flag affects only emulated sound cards; if a DirectSound driver is present, the play cursor is accurate for DirectSound in all versions of DirectX. 
	ADD_CONSTANT(DSBCAPS_GETCURRENTPOSITION2);
	// @const directsound|DSBCAPS_MUTE3DATMAXDISTANCE|The sound is reduced to silence at the maximum distance. The buffer will stop playing when the maximum distance is exceeded, so that processor time is not wasted. 
	ADD_CONSTANT(DSBCAPS_MUTE3DATMAXDISTANCE);

	ADD_CONSTANT(DSCBCAPS_WAVEMAPPED);

	// @const directsound|DSSPEAKER_HEADPHONE|The speakers are headphones. 
	ADD_CONSTANT(DSSPEAKER_HEADPHONE);
	// @const directsound|DSSPEAKER_MONO|The speakers are monaural.
	ADD_CONSTANT(DSSPEAKER_MONO);
	// @const directsound|DSSPEAKER_QUAD|The speakers are quadraphonic.
	ADD_CONSTANT(DSSPEAKER_QUAD);
	// @const directsound|DSSPEAKER_STEREO|The speakers are stereo (default value). 
	ADD_CONSTANT(DSSPEAKER_STEREO);
	// @const directsound|DSSPEAKER_SURROUND|The speakers are surround sound. 
	ADD_CONSTANT(DSSPEAKER_SURROUND);
	// @const directsound|DSSPEAKER_GEOMETRY_MIN|The speakers are directed over an arc of 5 degrees. 
	ADD_CONSTANT(DSSPEAKER_GEOMETRY_MIN);
	// @const directsound|DSSPEAKER_GEOMETRY_NARROW|The speakers are directed over an arc of 10 degrees. 
	ADD_CONSTANT(DSSPEAKER_GEOMETRY_NARROW);
	// @const directsound|DSSPEAKER_GEOMETRY_WIDE|The speakers are directed over an arc of 20 degrees. 
	ADD_CONSTANT(DSSPEAKER_GEOMETRY_WIDE);
	// @const directsound|DSSPEAKER_GEOMETRY_MAX|The speakers are directed over an arc of 180 degrees. 
	ADD_CONSTANT(DSSPEAKER_GEOMETRY_MAX);
	// real macros - todo if can be bothered
	// ADD_CONSTANT(DSSPEAKER_COMBINED);
	// ADD_CONSTANT(DSSPEAKER_CONFIG);
	// ADD_CONSTANT(DSSPEAKER_GEOMETRY);
	ADD_CONSTANT(DSBFREQUENCY_MIN);
	ADD_CONSTANT(DSBFREQUENCY_MAX);
	ADD_CONSTANT(DSBFREQUENCY_ORIGINAL);
	ADD_CONSTANT(DSBPAN_LEFT);
	ADD_CONSTANT(DSBPAN_CENTER);
	ADD_CONSTANT(DSBPAN_RIGHT);
	ADD_CONSTANT(DSBVOLUME_MIN);
	ADD_CONSTANT(DSBVOLUME_MAX);
	ADD_CONSTANT(DSBSIZE_MIN);
	ADD_CONSTANT(DSBSIZE_MAX);
	ADD_CONSTANT(DSCCAPS_EMULDRIVER);
	ADD_CONSTANT(DSCBLOCK_ENTIREBUFFER);
	ADD_CONSTANT(DSCBSTATUS_CAPTURING);
	ADD_CONSTANT(DSCBSTATUS_LOOPING);
	ADD_CONSTANT(DSCBSTART_LOOPING);
	ADD_CONSTANT(DSBPN_OFFSETSTOP);

	PyDict_SetItemString(dict, "DSCAPSType", (PyObject *)&PyDSCAPSType);
	PyDict_SetItemString(dict, "DSBCAPSType", (PyObject *)&PyDSBCAPSType);
	PyDict_SetItemString(dict, "DSBUFFERDESCType", (PyObject *)&PyDSBUFFERDESCType);
}

/* @topic DirectSound examples|

@ex Our raison d'etre - playing sounds:|

WAV_HEADER_SIZE = struct.calcsize('<4sl4s4slhhllhh4sl')

def wav_header_unpack(data):
	'''Unpack a wav header and stuff it into a WAVEFORMATEX structure'''
    (riff, riffsize, wave, fmt, fmtsize, format, nchannels, samplespersecond, 
     datarate, blockalign, bitspersample, data, datalength) \
     = struct.unpack('<4sl4s4slhhllhh4sl', data)

    if riff != 'RIFF' or fmtsize != 16 or fmt != 'fmt ' or data != 'data':
        raise ValueError, 'illegal wav header'

    wfx = pywintypes.WAVEFORMATEX()
    wfx.wFormatTag = format
    wfx.nChannels = nchannels
    wfx.nSamplesPerSec = samplespersecond
    wfx.nAvgBytesPerSec = datarate
    wfx.nBlockAlign = blockalign
    wfx.wBitsPerSample = bitspersample

    return wfx, datalength

# Play a wav file and wait until it's finished

fname=os.path.join(os.path.dirname(__file__), "01-Intro.wav")
f = open(fname, 'rb')

# Read and unpack the wav header
hdr = f.read(WAV_HEADER_SIZE)
wfx, size = wav_header_unpack(hdr)

d = ds.DirectSoundCreate(None, None)
d.SetCooperativeLevel(None, ds.DSSCL_PRIORITY)

sdesc = ds.DSBUFFERDESC()
sdesc.dwFlags = ds.DSBCAPS_STICKYFOCUS | ds.DSBCAPS_CTRLPOSITIONNOTIFY
sdesc.dwBufferBytes = size
sdesc.lpwfxFormat = wfx

buffer = d.CreateSoundBuffer(sdesc, None)

event = win32event.CreateEvent(None, 0, 0, None)
notify = buffer.QueryInterface(ds.IID_IDirectSoundNotify)

notify.SetNotificationPositions((ds.DSBPN_OFFSETSTOP, event))

buffer.Update(0, f.read(size))

buffer.Play(0)

win32event.WaitForSingleObject(event, -1)
*/
