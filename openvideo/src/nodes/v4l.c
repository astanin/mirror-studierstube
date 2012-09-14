//---------------------------------------------------------------------------------
// v4l.c - V4L library implementation
//
// Author:  Ben Bridgwater
// History:
// 03/07/99 Initial version
// 03/14/99 Added V4LBuildRGB8Palette().
// 03/18/99 Added complete set of V4LRGBMasks.
// 03/18/99 Added RGB to/from YCrCb conversions.
// 03/20/99 Changed V4lMSet/GetFormat to implement capture size restrictions.
// 09/10/00 Fixed V4LYCbCrToRGB(), V4LRGBToYCbCr().
// 09/16/00 Moved RGB<->YCbCr conversion to rgb2yuv module.
// 09/16/00 Moved V4LBuildRGB8Palette to hi240 module.
// 02/03/01 Fixed missing "case" keywords in V4LMSetFormat switch.
//---------------------------------------------------------------------------------

#include <stdio.h> // debug

#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <openvideo/nodes/v4l.h>

//---------------------------------------------------------------------------------

static V4LFormat _v4lFormat; // Set by V4LMSetFormat, used by V4LMCapture

//---------------------------------------------------------------------------------

const V4LRGBMasks v4lRGB15Masks = {    0x7c00,     0x03e0,     0x001f};
const V4LRGBMasks v4lRGB16Masks = {    0xf800,     0x07e0,     0x001f};
const V4LRGBMasks v4lRGB24Masks = {  0xff0000,   0x00ff00,   0x0000ff}; // B, G, R
const V4LRGBMasks v4lRGB32Masks = {0x00ff0000, 0x0000ff00, 0x000000ff}; // B, G, R, -

//---------------------------------------------------------------------------------

V4LMaskName V4LVideoTypeMaskList[] = {
  {VID_TYPE_CAPTURE,    "VID_TYPE_CAPTURE"},
  {VID_TYPE_TUNER,      "VID_TYPE_TUNER"},
  {VID_TYPE_TELETEXT,   "VID_TYPE_TELETEXT"},
  {VID_TYPE_OVERLAY,    "VID_TYPE_OVERLAY"},
  {VID_TYPE_CHROMAKEY,  "VID_TYPE_CHROMAKEY"},
  {VID_TYPE_CLIPPING,   "VID_TYPE_CLIPPING"},
  {VID_TYPE_FRAMERAM,   "VID_TYPE_FRAMERAM"},
  {VID_TYPE_SCALES,     "VID_TYPE_SCALES"},
  {VID_TYPE_MONOCHROME, "VID_TYPE_MONOCHROME"},
  {VID_TYPE_SUBCAPTURE, "VID_TYPE_SUBCAPTURE"},
  {0}
};

V4LMaskName V4LTunerCapabilityMaskList[] = {
  {VIDEO_TUNER_PAL,     "VIDEO_TUNER_PAL"},
  {VIDEO_TUNER_NTSC,    "VIDEO_TUNER_NTSC"},
  {VIDEO_TUNER_SECAM,   "VIDEO_TUNER_SECAM"},
  {VIDEO_TUNER_LOW,     "VIDEO_TUNER_LOW"},
  {VIDEO_TUNER_NORM,    "VIDEO_TUNER_NORM"},
  {0}
};

//---------------------------------------------------------------------------------

char *V4LChannelTypeName(int channelType)
{
  switch (channelType) {
  case VIDEO_TYPE_TV:     return ("TV");
  case VIDEO_TYPE_CAMERA: return ("Camera");
  default:                return ("?");
  } // end switch

} // end V4LChannelTypeName

//---------------------------------------------------------------------------------

char *V4LTunerModeName(int tunerMode)
{
  switch (tunerMode) {
  case VIDEO_MODE_PAL:   return ("PAL");
  case VIDEO_MODE_NTSC:  return ("NTSC");
  case VIDEO_MODE_SECAM: return ("SECAM");
  case VIDEO_MODE_AUTO:  return ("AUTO");
  default:               return ("?");
  } // end switch

} // end V4LTunerModeName

//---------------------------------------------------------------------------------

char *V4LFormatName(int format)
{
  switch (format) {
  case VIDEO_PALETTE_GREY:    return ("VIDEO_PALETTE_GREY");
  case VIDEO_PALETTE_HI240:   return ("VIDEO_PALETTE_HI240");
  case VIDEO_PALETTE_RGB565:  return ("VIDEO_PALETTE_RGB565");
  case VIDEO_PALETTE_RGB24:   return ("VIDEO_PALETTE_RGB24");
  case VIDEO_PALETTE_RGB32:   return ("VIDEO_PALETTE_RGB32");
  case VIDEO_PALETTE_RGB555:  return ("VIDEO_PALETTE_RGB555");
  case VIDEO_PALETTE_YUV422:  return ("VIDEO_PALETTE_YUV422");
  case VIDEO_PALETTE_YUYV:    return ("VIDEO_PALETTE_YUYV");
  case VIDEO_PALETTE_UYVY:    return ("VIDEO_PALETTE_UYVY");
  case VIDEO_PALETTE_YUV420:  return ("VIDEO_PALETTE_YUV420");
  case VIDEO_PALETTE_YUV411:  return ("VIDEO_PALETTE_YUV411");
  case VIDEO_PALETTE_RAW:     return ("VIDEO_PALETTE_RAW");
  case VIDEO_PALETTE_YUV422P: return ("VIDEO_PALETTE_YUV422P");
  case VIDEO_PALETTE_YUV411P: return ("VIDEO_PALETTE_YUV411P");
  case VIDEO_PALETTE_YUV420P: return ("VIDEO_PALETTE_YUV420P");
  case VIDEO_PALETTE_YUV410P: return ("VIDEO_PALETTE_YUV410P");
  default:                    return ("?");
  } // end switch

} // end V4LFormatName

//---------------------------------------------------------------------------------

int V4LGetCaps(int fd, V4LCaps *v4lCaps)
{
  if (ioctl(fd, VIDIOCGCAP, v4lCaps) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LGetCaps

//---------------------------------------------------------------------------------

int V4LSetSource(int fd, int channel, int norm)
{
  V4LChannel v4lchannel;

  v4lchannel.channel = channel;
  v4lchannel.norm = norm; // VIDEO_MODE_NTSC, etc

  if (ioctl(fd, VIDIOCSCHAN, &v4lchannel) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LSetSource

//---------------------------------------------------------------------------------

int V4LGetSource(int fd, int source, V4LChannel *v4lchannel)
{
  v4lchannel->channel = source;

  if (ioctl(fd, VIDIOCGCHAN, v4lchannel) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LGetSource

//---------------------------------------------------------------------------------

int _V4LSetPicture(int fd, V4LPicture *v4lPicture)
{
  // Warning: Changes overlay format if bpp matches framebuffer depth bpp

  if (ioctl(fd, VIDIOCSPICT, v4lPicture) < 0) {
    return (errno);
  } // end if

  return (0);

} // end _V4LSetPicture

//---------------------------------------------------------------------------------

int _V4LGetPicture(int fd, V4LPicture *v4lPicture)
{
  if (ioctl(fd, VIDIOCGPICT, v4lPicture) < 0) {
    return (errno);
  } // end if

  return (0);

} // end _V4LGetPicture

//---------------------------------------------------------------------------------

int V4LSetBrightness(int fd, int brightness)
{
  V4LPicture v4lPicture;

  if (_V4LGetPicture(fd, &v4lPicture) < 0) {
    return (errno);
  } // end if

  v4lPicture.brightness = brightness;

  return (_V4LSetPicture(fd, &v4lPicture));

} // end V4LSetBrightness

//---------------------------------------------------------------------------------

int V4LSetContrast(int fd, int contrast)
{
  V4LPicture v4lPicture;

  if (_V4LGetPicture(fd, &v4lPicture) < 0) {
    return (errno);
  } // end if

  v4lPicture.contrast = contrast;

  return (_V4LSetPicture(fd, &v4lPicture));

} // end V4LSetContrast

//---------------------------------------------------------------------------------

int V4LSetSaturation(int fd, int saturation)
{
  V4LPicture v4lPicture;

  if (_V4LGetPicture(fd, &v4lPicture) < 0) {
    return (errno);
  } // end if

  v4lPicture.colour = saturation;

  return (_V4LSetPicture(fd, &v4lPicture));

} // end V4LSetSaturation

//---------------------------------------------------------------------------------

int V4LSetHue(int fd, int hue)
{
  V4LPicture v4lPicture;

  if (_V4LGetPicture(fd, &v4lPicture) < 0) {
    return (errno);
  } // end if

  v4lPicture.hue = hue;

  return (_V4LSetPicture(fd, &v4lPicture));

} // end V4LSetHue

//---------------------------------------------------------------------------------

int V4LOSetWindow(int fd, V4LWindow *v4lWindow)
{
  if (ioctl(fd, VIDIOCSWIN, v4lWindow) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LOSetWindow

//---------------------------------------------------------------------------------

int V4LOGetWindow(int fd, V4LWindow *v4lWindow)
{
  if (ioctl(fd, VIDIOCGWIN, v4lWindow) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LOGetWindow

//---------------------------------------------------------------------------------

int V4LOSetFramebuffer(int fd, V4LFramebuffer *v4lFramebuffer)
{
  if (ioctl(fd, VIDIOCSFBUF, v4lFramebuffer) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LOSetFramebuffer

//---------------------------------------------------------------------------------

int V4LOGetFramebuffer(int fd, V4LFramebuffer *v4lFramebuffer)
{
  if (ioctl(fd, VIDIOCGFBUF, v4lFramebuffer) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LOGetFramebuffer

//---------------------------------------------------------------------------------

int V4LOCapture(int fd, int on)
{
  int captureOn  = 1;
  int captureOff = 0;

  if (ioctl(fd, VIDIOCCAPTURE, (int *) (on ? &captureOn : &captureOff)) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LOCapture

//---------------------------------------------------------------------------------

int V4LMGetMMInfo(int fd, V4LMMInfo *v4lMMInfo)
{
  if (ioctl(fd, VIDIOCGMBUF, v4lMMInfo) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LMGetMMInfo

//---------------------------------------------------------------------------------

void V4LMSetFormat(V4LFormat *v4lFormat)
{
  //#if (0)
  printf("V4LMSetFormat(%s, %i, %i)\n", V4LFormatName(v4lFormat->format), v4lFormat->width, v4lFormat->height);
  //#endif
  _v4lFormat = *v4lFormat;

  switch (_v4lFormat.format) {
  case V4L_FORMAT_YUV422P:
  case V4L_FORMAT_YUV411P:
  case V4L_FORMAT_YUV420P:
  case V4L_FORMAT_YUV410P:
    _v4lFormat.width = (_v4lFormat.width / 16) * 16;
    break;
  case V4L_FORMAT_YUY2:
    _v4lFormat.width = (_v4lFormat.width / 2) * 2;
    break;
  case V4L_FORMAT_Y41P:
    _v4lFormat.width = (_v4lFormat.width / 8) * 8;
    break;
  } // end switch

  _v4lFormat.width  = (_v4lFormat.width  / 4) * 4;
  _v4lFormat.height = (_v4lFormat.height / 4) * 4;

} // end V4LMSetFormat

//---------------------------------------------------------------------------------

void V4LMGetFormat(V4LFormat *v4lFormat)
{
  *v4lFormat = _v4lFormat;

} // end V4LMGetFormat

//---------------------------------------------------------------------------------

int V4LMCapture(int fd, int frame)
{
  struct video_mmap videoMMap;

  videoMMap.width  = _v4lFormat.width;
  videoMMap.height = _v4lFormat.height;
  videoMMap.format = _v4lFormat.format;
  videoMMap.frame  = frame;

  if (ioctl(fd, VIDIOCMCAPTURE, &videoMMap) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LMCapture

//---------------------------------------------------------------------------------

int V4LMSync(int fd, int frame)
{
  if (ioctl(fd, VIDIOCSYNC, &frame) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LMSync

//---------------------------------------------------------------------------------

int V4LSetTuner(int fd, V4LTuner *v4lTuner)
{
  v4lTuner->tuner = 0; // Assume there's only one!

  if (ioctl(fd, VIDIOCSTUNER, v4lTuner) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LSetTuner

//---------------------------------------------------------------------------------

int V4LGetTuner(int fd, V4LTuner *v4lTuner)
{
  v4lTuner->tuner = 0; // Assume there's only one!

  if (ioctl(fd, VIDIOCGTUNER, v4lTuner) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LGetTuner

//---------------------------------------------------------------------------------

int V4LSetTunerFreq(int fd, ulong tunerFreq)
{
  if (ioctl(fd, VIDIOCSFREQ, &tunerFreq) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LSetTunerFreq

//---------------------------------------------------------------------------------

int V4LGetTunerFreq(int fd, ulong *tunerFreq)
{
  if (ioctl(fd, VIDIOCGFREQ, tunerFreq) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LGetTunerFreq

//---------------------------------------------------------------------------------

int V4LSetAudio(int fd, V4LAudio *v4lAudio)
{
  if (ioctl(fd, VIDIOCSAUDIO, v4lAudio) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LSetAudio

//---------------------------------------------------------------------------------

int V4LGetAudio(int fd, V4LAudio *v4lAudio)
{
  if (ioctl(fd, VIDIOCGAUDIO, v4lAudio) < 0) {
    return (errno);
  } // end if

  return (0);

} // end V4LGetAudio

//---------------------------------------------------------------------------------
