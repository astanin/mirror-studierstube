//---------------------------------------------------------------------------------
// v4l.h - V4L library definitions
//
// Author:  Ben Bridgwater
// History:
// 03/07/99 Initial version
// 03/14/99 Added V4LBuildRGB8Palette().
// 03/18/99 Added complete set of V4LRGBMasks.
// 03/18/99 Added RGB to/from Y'CrCb conversions.
// 03/20/99 Added remaining planar YUV formats. Fixed FOURCC correspondence.
// 09/10/00 Renamed V4LYCbCrToRGB(), V4LRGBToYCbCr().
// 09/10/00 Fixed Y',Cb,Cr plane order documentaion of planar "YUV" formats.
// 09/16/00 Moved RGB<->YCbCr conversion to rgb2yuv module.
// 09/16/00 Moved V4LBuildRGB8Palette to hi240 module.
//---------------------------------------------------------------------------------
#ifndef _V4L_H
#define _V4L_H
//---------------------------------------------------------------------------------

#include <sys/types.h> /* ulong for videodev.h */
#include <linux/videodev.h>

//---------------------------------------------------------------------------------

// Alternate capture format specifiers ("Y" = Y', "U" = Cb, "V" = Cr). P = Planar.
#ifdef __cplusplus
extern "C" {
#endif

enum {
  V4L_FORMAT_RGB32    = VIDEO_PALETTE_RGB32,   // B G R Alpha
  V4L_FORMAT_RGB24    = VIDEO_PALETTE_RGB24,   // B G R
  V4L_FORMAT_RGB16    = VIDEO_PALETTE_RGB565,  // r5 g6 b5 15-0 (lo byte, hi byte)
  V4L_FORMAT_RGB15    = VIDEO_PALETTE_RGB555,  // r5 g5 b5 14-0 (lo byte, hi byte)

  V4L_FORMAT_HI240    = VIDEO_PALETTE_HI240,   // Hi240 color cube index

  V4L_FORMAT_Y8       = VIDEO_PALETTE_GREY,    // Y'

  V4L_FORMAT_YUV422   = VIDEO_PALETTE_YUV422,  // (YUY2 bttv.c) } Packed YUV format names
  V4L_FORMAT_YUYV     = VIDEO_PALETTE_YUYV,    // (Y41P bttv.c) } are ambigous.
  V4L_FORMAT_UYVY     = VIDEO_PALETTE_UYVY,    // ( n/a bttv.c) }
  V4L_FORMAT_YUV420   = VIDEO_PALETTE_YUV420,  // ( n/a bttv.c) } bttv.c YUY2, Y41P mappings
  V4L_FORMAT_YUV411   = VIDEO_PALETTE_YUV411,  // ( n/a bttv.c) } may be driver specific.

  V4L_FORMAT_YUV422P  = VIDEO_PALETTE_YUV422P, // Y W*H, U W/2*H/1, V W/2*H/1 (16 bpp)
  V4L_FORMAT_YUV411P  = VIDEO_PALETTE_YUV411P, // Y W*H, U W/4*H/1, V W/4*H/1 (12 bpp)
  V4L_FORMAT_YUV420P  = VIDEO_PALETTE_YUV420P, // Y W*H, U W/2*H/2, V W/2*H/2 (12 bpp)
  V4L_FORMAT_YUV410P  = VIDEO_PALETTE_YUV410P, // Y W*H, U W/4*H/4, V W/4*H/4 ( 9 bpp)

  // Aliases

  V4L_FORMAT_YUY2     = VIDEO_PALETTE_YUV422,  // 422 YUYV 16 bpp
  V4L_FORMAT_Y41P     = VIDEO_PALETTE_YUYV,    // 411 UYVY 12 bpp (aka BtYUV)

  V4L_FORMAT_YUV9     = VIDEO_PALETTE_YUV410P, // U, V reversed from AVI YVU9
  V4L_FORMAT_YUV12    = VIDEO_PALETTE_YUV420P  // U, V reversed from AVI YV12
};

//---------------------------------------------------------------------------------

// R/G/B masks

typedef struct {
  u_int32_t redMask;
  u_int32_t greenMask;
  u_int32_t blueMask;
} V4LRGBMasks;

extern  const V4LRGBMasks v4lRGB15Masks;
extern  const V4LRGBMasks v4lRGB16Masks;
extern  const V4LRGBMasks v4lRGB24Masks;
extern  const V4LRGBMasks v4lRGB32Masks;

//---------------------------------------------------------------------------------

// Mask and type to name conversion

typedef struct {
  int  mask; // 0 terminates list
  char *name;
} V4LMaskName;

 extern V4LMaskName V4LVideoTypeMaskList[];        // V4LCaps.type
extern  V4LMaskName V4LTunerCapabilityMaskList[];  // V4LTuner.flags

  char *V4LChannelTypeName(int channelType); // V4LChannel.type
  char *V4LTunerModeName(int tunerMode);     // V4LTuner.mode
  char *V4LFormatName(int format);           // V4LFormat.format, V4LPicture.palette

//---------------------------------------------------------------------------------

typedef struct video_capability  V4LCaps;

typedef struct video_channel     V4LChannel;     // input, NTSC/PAL
typedef struct video_picture     V4LPicture;     //

typedef struct video_buffer      V4LFramebuffer; // depth => format
typedef struct video_window      V4LWindow;      // x, y, width, height, clipping
typedef struct video_clip        V4LClip;        //

typedef struct video_mbuf        V4LMMInfo;

typedef struct video_tuner       V4LTuner;
typedef struct video_audio       V4LAudio;

typedef struct {
  int width;
  int height;
  int format; // V4L_FORMAT_XXX
} V4LFormat;

//---------------------------------------------------------------------------------

  int V4LGetCaps(int fd, V4LCaps *v4lCaps);

  int V4LSetSource(int fd, int channel, int norm);
  int V4LGetSource(int fd, int channel, V4LChannel *v4lChannel);

  int _V4LSetPicture(int fd, V4LPicture *v4lPicture);
  int _V4LGetPicture(int fd, V4LPicture *v4lPicture);

  int V4LSetBrightness(int fd, int brightness); // 0-65535 -> +/-50% luma offset
  int V4LSetContrast(int fd, int contrast);     // 0-65535 -> 0-200% luma gain
  int V4LSetSaturation(int fd, int saturation); // 0-65535 -> 0-200% chroma gain
  int V4LSetHue(int fd, int hue);               // 0-65535 -> +/-90o hue phase

// Overlay
  int V4LOSetFramebuffer(int fd, V4LFramebuffer *v4lFramebuffer); // depth => format
  int V4LOGetFramebuffer(int fd, V4LFramebuffer *v4lFramebuffer);
  int V4LOSetWindow(int fd, V4LWindow *v4lWindow);
  int V4LOGetWindow(int fd, V4LWindow *v4lWindow);
  int V4LOCapture(int fd, int on); // 0=off, 1=on

// Memory
  int V4LMGetMMInfo(int fd, V4LMMInfo *v4lMMInfo);
  void V4LMSetFormat(V4LFormat *v4lFormat);
  void V4LMGetFormat(V4LFormat *v4lFormat);
  int V4LMCapture(int fd, int frame);
  int V4LMSync(int fd, int frame);

// TV
  int V4LSetTuner(int fd, V4LTuner *v4lTuner);
  int V4LGetTuner(int fd, V4LTuner *v4lTuner);
  int V4LSetTunerFreq(int fd, ulong tunerFreq);
  int V4LGetTunerFreq(int fd, ulong *tunerFreq);
  int V4LSetAudio(int fd, V4LAudio *v4lAudio);
  int V4LGetAudio(int fd, V4LAudio *v4lAudio);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------------
