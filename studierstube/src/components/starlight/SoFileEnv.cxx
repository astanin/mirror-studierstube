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
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The Source file for the SoFileEnv
*
* @author Erick Mendez
*
* $Id: SoFileEnv.cpp 2006-03-10 mendez $
* @file                                                                   */
/* ======================================================================= */

#include <stb/components/starlight/SoFileEnv.h>

using namespace std;

SO_NODE_SOURCE(SoFileEnv);

SoFileEnv::SoFileEnv()
{
    SO_NODE_CONSTRUCTOR(SoFileEnv);
    SO_NODE_ADD_FIELD(envPath, (""));
    SO_NODE_ADD_FIELD(fileName, (""));
}

SoFileEnv::~SoFileEnv()
{
}

void SoFileEnv::initClass(void)
{
    SO_NODE_INIT_CLASS(SoFileEnv, SoFile, "File");
}

SbBool SoFileEnv::readNamedFile(SoInput * in)
{
    SbString newName=fileName.getValue().getString();
    SbString tmpPath=getenv(envPath.getValue().getString());
    if (tmpPath.getLength()>0)
    {
        tmpPath+="/";
        tmpPath+=newName.getString();
        newName=tmpPath;
    }
    name.setValue(newName);
    return SoFile::readNamedFile(in);
}
