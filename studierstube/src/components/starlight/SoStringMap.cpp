/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * source file for SoStringMap engine 
 * 
 * @author   Gerhard Reitmayr 
 * 
 * $Id: SoStringMap.cpp 31 2005-04-04 15:46:47Z daniel $
 * @file                                                                   */ 
/* ======================================================================= */ 
 

#include <cassert> 
#include <set> 
#include <algorithm>
#include <iterator>
#include <vector>

#include <stb/components/starlight/SoStringMap.h>


/** 
 * comparison operator for SbString required for set operations on SbString. 
 */ 
bool operator <( const SbString & a , const SbString & b ) 
{ 
    return (strcmp(a.getString(),  b.getString()) < 0); 
} 
 
SO_ENGINE_SOURCE(SoStringMap); 

void SoStringMap::initClass() 
{ 
    SO_ENGINE_INIT_CLASS(SoStringMap, SoEngine, "Engine"); 
} 

SoStringMap::SoStringMap() 
{ 
    SO_ENGINE_CONSTRUCTOR(SoStringMap); 

    SO_ENGINE_DEFINE_ENUM_VALUE( Mode, MAPPING ); 
    SO_ENGINE_DEFINE_ENUM_VALUE( Mode, UNION ); 
    SO_ENGINE_DEFINE_ENUM_VALUE( Mode, INTERSECTION ); 
    SO_ENGINE_DEFINE_ENUM_VALUE( Mode, INDEX );
    SO_ENGINE_SET_SF_ENUM_TYPE( mode, Mode ); 
 
    SO_ENGINE_ADD_INPUT(a, ("")); 
    SO_ENGINE_ADD_INPUT(b, ("")); 
    SO_ENGINE_ADD_INPUT(aIn, ("")); 
    SO_ENGINE_ADD_INPUT(bIn, ("")); 
    SO_ENGINE_ADD_INPUT(mode, (MAPPING)); 
    SO_ENGINE_ADD_INPUT(compact, (TRUE));
    SO_ENGINE_ADD_OUTPUT(aOut, SoMFString); 
    SO_ENGINE_ADD_OUTPUT(bOut, SoMFString); 
} 
 
SoStringMap::~SoStringMap() 
{ 
} 
 
void SoStringMap::evaluate() 
{ 
    SbBool compact = this->compact.getValue();

    switch( mode.getValue() ) 
    { 
    case SoStringMap::MAPPING: 
        { 
            int i; 
            int index; 
            // map aIn to bOut 
            for( i = 0; i < aIn.getNum(); i++) 
            { 
                index = a.find( aIn[i] ); 
                if( index == -1 || index >= b.getNum()) 
                    if(!compact)
                        SO_ENGINE_OUTPUT(bOut, SoMFString, set1Value(i,"")); 
                else 
                    SO_ENGINE_OUTPUT(bOut, SoMFString, set1Value(i,b[index])); 
            } 
            SO_ENGINE_OUTPUT(bOut, SoMFString, setNum(aIn.getNum())); 
            // map bIn to aOut 
            for( i = 0; i < bIn.getNum(); i++) 
            { 
                index = b.find( bIn[i] ); 
                if( index == -1 || index >= a.getNum()) 
                    if(!compact)
                        SO_ENGINE_OUTPUT(aOut, SoMFString, set1Value(i,"")); 
                else 
                    SO_ENGINE_OUTPUT(aOut, SoMFString, set1Value(i,a[index])); 
            } 
            SO_ENGINE_OUTPUT(aOut, SoMFString, setNum(bIn.getNum())); 
        } 
        break; 
    case SoStringMap::UNION : 
        { 
            int i,j; 
            std::set<SbString> output; 
            std::set<SbString>::const_iterator it; 
            // relate aIn to bOut 
            for( i = 0; i < aIn.getNum(); i++ ) 
            { 
                for( j = 0; j < a.getNum(); j++ ) 
                { 
                    if( a[j] == aIn[i] && j < b.getNum()) 
                    { 
                        output.insert( b[j] ); 
                    }                 
                }             
            } 
            for( i = 0, it = output.begin(); it != output.end(); it++, i++) 
            { 
                SO_ENGINE_OUTPUT(bOut, SoMFString, set1Value(i, (*it))); 
            } 
            SO_ENGINE_OUTPUT(bOut, SoMFString, setNum(output.size())); 
            // relate bIn to aOut 
            output.clear(); 
            for( i = 0; i < bIn.getNum(); i++ ) 
            { 
                for( j = 0; j < b.getNum(); j++ ) 
                { 
                    if( b[j] == bIn[i] && j < a.getNum()) 
                    { 
                        output.insert( a[j] ); 
                    }                 
                }             
            } 
            for( i = 0, it = output.begin(); it != output.end(); it++, i++) 
            { 
                SO_ENGINE_OUTPUT(aOut, SoMFString, set1Value(i, (*it))); 
            } 
            SO_ENGINE_OUTPUT(aOut, SoMFString, setNum(output.size())); 
        } 
        break; 
    case SoStringMap::INTERSECTION : 
        { 
            int i,j; 
            std::set<SbString> output, temp, result; 
            std::set<SbString>::const_iterator it;             
            // relate aIn to bOut 
            for( i = 0; i < b.getNum(); i ++) 
                output.insert( b[i] ); 
            for( i = 0; i < aIn.getNum(); i++ ) 
            { 
                for( j = 0; j < a.getNum(); j++ ) 
                { 
                    if( a[j] == aIn[i] && j < b.getNum()) 
                    { 
                        temp.insert( b[j] ); 
                    } 
                } 
                // output = output /\ temp 
                std::set_intersection( output.begin(), output.end(), temp.begin(), temp.end(), std::inserter( result, result.begin())); 
                output.swap(result);
                result.clear();
                temp.clear();
            } 
            SO_ENGINE_OUTPUT(bOut, SoMFString, setNum(output.size())); 
            for( i = 0, it = output.begin(); it != output.end(); it++, i++) 
            { 
                SO_ENGINE_OUTPUT(bOut, SoMFString, set1Value(i, (*it))); 
            } 
            // relate bIn to aOut 
            output.clear(); 
            for( i = 0; i < b.getNum(); i ++) 
                output.insert( b[i] ); 
            for( i = 0; i < bIn.getNum(); i++ ) 
            { 
                for( j = 0; j < b.getNum(); j++ ) 
                { 
                    if( b[j] == bIn[i] && j < a.getNum()) 
                    { 
                        temp.insert( a[j] ); 
                    }                 
                } 
                // output = output /\ temp 
                std::set_intersection( output.begin(), output.end(), temp.begin(), temp.end(), std::inserter( result, result.begin())); 
                output.swap(result);
                result.clear();
                temp.clear();
            } 
            SO_ENGINE_OUTPUT(aOut, SoMFString, setNum(output.size())); 
            for( i = 0, it = output.begin(); it != output.end(); it++, i++) 
            { 
                SO_ENGINE_OUTPUT(aOut, SoMFString, set1Value(i, (*it))); 
            } 
        } 
        break; 
    case INDEX:
        {
            int i;
            std::vector<int32_t> indices;
            for( i = 0; i < aIn.getNum(); i++)
            {
                int index = a.find( aIn[i] );
                if( index != -1 || !compact)
                    indices.push_back( index );
            }
            SO_ENGINE_OUTPUT(bOut, SoMFString, setNum(indices.size()));
            for( i = 0; (unsigned int)i < indices.size(); i++ )
            {
                SO_ENGINE_OUTPUT(bOut, SoMFString, set1Value(i, SbString(indices[i]))); 
            }
        }
        break;
    default: 
        assert(false); 
    } 
} 
