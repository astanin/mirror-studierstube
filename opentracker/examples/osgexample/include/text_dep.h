#ifndef OSGPONG_TEXT_HH_INCLUDED
#define OSGPONG_TEXT_HH_INCLUDED


#define WINVER 0x0400
#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0400
#define _OSG_HAVE_CONFIGURED_H_
#define __INTEL_COMPILER_VERSION 600
#define OSG_BUILD_DLL
#define OSG_WITH_GLUT
#define OSG_WITH_GIF
#define OSG_WITH_TIF
#define OSG_WITH_JPG


// pilfered from testTextTXFParam in the source/text folder of the opensg installation
#include <OpenSG/OSGGLUT.h>
#include <OpenSG/OSGConfig.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGGLUTWindow.h>
#include <OpenSG/OSGSimpleSceneManager.h>
#include <OpenSG/OSGSceneFileHandler.h>

#include <OpenSG/OSGTextTXFFace.h>
#include <OpenSG/OSGTextTXFGlyph.h>
#include <OpenSG/OSGTextFaceFactory.h>
#include <OpenSG/OSGTextLayoutParam.h>
#include <OpenSG/OSGTextLayoutResult.h>

#include <OpenSG/OSGChunkMaterial.h>
#include <OpenSG/OSGTextureChunk.h>
#include <OpenSG/OSGBlendChunk.h>
#include <OpenSG/OSGImage.h>
#include <OpenSG/OSGBaseFunctions.h>

#undef WINVER
#undef _WIN32_WINDOWS
#undef _WIN32_WINNT
#undef _OSG_HAVE_CONFIGURED_H_
#undef __INTEL_COMPILER_VERSION
#undef OSG_BUILD_DLL
#undef OSG_WITH_GLUT
#undef OSG_WITH_GIF
#undef OSG_WITH_TIF
#undef OSG_WITH_JPG


#endif //OSGPONG_TEXT_HH_INCLUDED
