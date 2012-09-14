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
/** The source file for the TestSrc class.
  *
  * @author Denis Kalkofen
  * 
  * $Id: TestSrc.cxx 237 2007-05-02 15:45:07Z bornik $
  * @file                                                                   
  * ======================================================================= */


#include <openvideo/nodes/TestSrc.h>
#include <openvideo/openVideo.h>


#ifdef ENABLE_TESTSRC


#include <openvideo/Manager.h>
#include <openvideo/State.h>
#include <iostream>

namespace openvideo {


// Allows TestSrc to set internal data of openvideo::Frame
class TestSrcBuffer : public Buffer
{
friend class TestSrc;
public:
	TestSrcBuffer(unsigned char* pixbuffer, State* state)
	{
		buffer = pixbuffer;
		width = state->width;
		height = state->height;
		format = state->format;
	}

	~TestSrcBuffer()
	{
		delete buffer;
		buffer = NULL;
	}

	//void setState(Frame::STATE newstate)  {  state = newstate;  }
	void incUpdateCounter()  {  updateCtr++;  }
};


// Allows TestSrc to set internal data of openvideo::State
class TestSrcState : public State
{

public:
	~TestSrcState()
	{
	  for(size_t i=0; i<buffers.size(); i++)
	    delete buffers[i];
	  buffers.clear();
	}
	BufferVector& getBuffers()  {  return buffers;  }
	void setCurrentBuffer(Buffer* buf)  {  currentBuffer = buf;  }
};



// constructor
TestSrc::TestSrc()
{
	name = typeName = "TestSrc";
    state=new TestSrcState();
    width=320;
    height=240;
    updateCtr = 1;
}

// destructor
TestSrc::~TestSrc()
{
    delete state;


}

void 
TestSrc::initPixelFormats()
{
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_R8G8B8));
	pixelFormats.push_back(PIXEL_FORMAT(FORMAT_B8G8R8));
}

void 
TestSrc::init()
{
    logPrintS("Building TestSrc\n");
    state->clear();

    state->width=width;
    state->height=height;

	// make a double buffered state
	for(int i=0; i<2; i++)
	{
		unsigned char *pixels = new unsigned char[width*height*3];
		memset(pixels, 255, width*height*3);

		reinterpret_cast<TestSrcState*>(state)->getBuffers().push_back(new TestSrcBuffer(pixels, state));
	}
	
    posX=posY=0;
}


void 
TestSrc::process()
{
  using namespace std;
  //cerr << "errr" << endl;
    TestSrcBuffer* buffer = reinterpret_cast<TestSrcBuffer*>(state->findFreeBuffer());

    if(!buffer)
      {
	logPrintW("DSVLSrc all buffers locked, can not read a new camera image!\n");
	return;
      }

    reinterpret_cast<TestSrcState*>(state)->setCurrentBuffer(buffer);
    updateCtr++;
    

    unsigned char* img = const_cast<unsigned char*>(buffer->getPixels());

    unsigned char R,G,B;
    int dist=10;
int x,y;
for(x=0; x<width; x++)
  for (y = 0; y<height; y++)
    {
       img[(3*width*y)+(3*x)  ]=255;
       img[(3*width*y)+(3*x)+1]=255;
       img[(3*width*y)+(3*x)+2]=255;
    }

    for( y=posY;y<posY+dist;y++)
    {
	    int ix=0;
	    for(int x=posX;x<posX+(dist*3);x++)
	    {
    	    
	        R=255;
	        G=255;
	        B=255;
    	    
	        img[(3*width*y)+(3*x)  ]=R;
	        img[(3*width*y)+(3*x)+1]=G;
	        img[(3*width*y)+(3*x)+2]=B;
	    }
    }
    posX++;;

    if(posX>=width-(dist*3)){
		posX=0;
		posY=posY+dist;
		if(posY>=height-dist)
			posY=0;
	}
   
   
    for(int y=posY;y<posY+dist;y++)
    {
		int ix=0;
		for(int x=posX;x<posX+(dist*3);x++)
		{
			if(ix<=dist){
			R=255;
			G=0;
			B=0;
			}else if(ix>dist && ix<=dist*2){
			R=0;
			G=255;
			B=0;
			}else if(ix>dist*2 && ix<dist*3){
			R=0;
			G=0;
			B=255;
			}else if(ix>=dist*3){
			ix=0;
			}

			img[(3*width*y)+(3*x)  ]=R;
			img[(3*width*y)+(3*x)+1]=G;
			img[(3*width*y)+(3*x)+2]=B;
			ix++;
		}
    }

	buffer->incUpdateCounter();

}


}  // namespace openvideo


#endif //#ifdef ENABLE_TESTSRC
