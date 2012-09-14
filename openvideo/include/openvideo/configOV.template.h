/* ========================================================================
 * Copyright (C) 2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Denis Kalkofen under
 * <kalkofen@icg.tu-graz.ac.at> or write to Denis Kalkofen,
 * Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
 * Austria.
 * ========================================================================
 * PROJECT: OpenVideo
 * ======================================================================== */
/** The node configuration file.
  *
  * @author Denis Kalkofen
  *
  * $Id: configOV.template.h 148 2006-05-26 14:53:05Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

//enable/disable nodes


//#define  ENABLE_TESTSRC
#define  ENABLE_VIDEOSINK
//#define  ENABLE_IMAGESRC
#define  ENABLE_OPENCV

//
// if using TinyXML_MOD, uncomment one of the next two
// definitions to choose between DLL and static linking
//
#ifndef TINYXML_MOD_DLL
#  define TINYXML_MOD_DLL
#endif

//#ifndef TINYXML_MOD_STATIC
//#  define TINYXML_MOD_STATIC
//#endif

#if defined(TINYXML_MOD_DLL) && defined(TINYXML_MOD_STATIC)
#  pragma message(">>> ERROR: can not define TINYXML_MOD_DLL and TINYXML_MOD_STATIC at the same time!")
#  pragma error(">>> ERROR: can not define TINYXML_MOD_DLL and TINYXML_MOD_STATIC at the same time!")
#endif
