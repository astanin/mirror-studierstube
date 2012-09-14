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
/** The header file for the OpenCVSrc class.
 *
 * @author Jorn Skaarud Karlsen
 *
 * $Id: OpenCVSrc.h 252 2007-06-13 14:34:11Z mendez $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _OPENCVSRC_H
#define _OPENCVSRC_H

// OpenVideo standard include file
#include <openvideo/openVideo.h>

// If OpenCV is available
#ifdef ENABLE_OPENCV

#include <string>
#include <openvideo/Node.h>



// Define private class
class OpenCVSrcP;

// Our classes are in the openvideo namespace
namespace openvideo {
    /**
    * @ingroup nodes
    * This class is a wrapper for USB and Firewire cameras. Each class
    * supports reading input from one camera and this is transferred to
    * the listening nodes. The functionality is taken from OpenCV.
    * 
    * @author Jorn Skaarud Karlsen
    */
  // Define source class
  class OPENVIDEO_API OpenCVSrc : public openvideo::Node 
  {
    /// Typedef inherited for clean access
    typedef Node inherited;
    /// Make sure to be friends with the private class
    friend class OpenCVSrcP;

  public:
    /**
     * Constructor.
     */
    OpenCVSrc();

    /**
     * Destructor
     */
    virtual ~OpenCVSrc();

    /**
     *	clears the image and sets the width and height onto its context.
     */
    virtual void init();

    /**
     * Starts the capturing process
     */
    virtual void start();

    /**
     * Stops the capturing process
     */
    virtual void stop();

    /**
     * Performs post processing
     */
    virtual void postProcess(void);

    /**
     *	Attempts to grab an image from the firewire device
     */
    virtual void process( );
    
    /**
     * Initializes the supported pixel formats
     */
    virtual void initPixelFormats();

    /**
     * Sets a parameter from the XML-file
     */
    virtual bool setParameter(std::string key, std::string value);

  protected:
    /**
     * Private class
     */
    OpenCVSrcP * pimpl;

    int numBuffers;
    unsigned int updateCtr;

  };

} // namespace openvideo {

#endif // ENABLE_OPENCV
#endif // _OPENCVSRC_H
