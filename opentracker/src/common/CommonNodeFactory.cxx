/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for CommonNodeFactory class.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: CommonNodeFactory.cxx 2103 2007-12-19 16:39:50Z bornik $
 * @file                                                                   */
/* ======================================================================= */

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>

#include <cstdlib>
#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/common/MergeNode.h>
#ifndef OT_NO_MERGENODE_SUPPORT
#  include <OpenTracker/common/TriangulateOrientationNode.h>
#endif
#include <OpenTracker/common/VirtualTransformation.h>
#include <OpenTracker/common/DynamicTransformation.h>
#include <OpenTracker/common/DynamicVirtualTransformation.h>
#include <OpenTracker/common/InvertTransformation.h>
#include <OpenTracker/common/MatrixTransformation.h>
#include <OpenTracker/common/SelectionNode.h>
#include <OpenTracker/common/ConfidenceFilterNode.h>
#include <OpenTracker/common/ConfidenceSelectNode.h>
#include <OpenTracker/common/FilterNode.h>
#include <OpenTracker/common/ThresholdFilterNode.h>
#include <OpenTracker/common/ButtonFilterNode.h>
#include <OpenTracker/common/ButtonOpNode.h>
#include <OpenTracker/common/TimeGateNode.h>
#include <OpenTracker/common/EllipsoidTransformNode.h>
#include <OpenTracker/common/GKTransformNode.h>
#include <OpenTracker/common/RangeFilterNode.h>
#include <OpenTracker/common/PositionFilterNode.h>
#include <OpenTracker/common/EventUtilityNode.h>
#include <OpenTracker/common/DESPFilterNode.h>
#include <OpenTracker/common/EEGFilterNode.h>
#include <OpenTracker/common/HeartrateFilterNode.h>
#include <OpenTracker/common/DifferenceNode.h>

#include <cmath>
#include <cfloat>
#include <cstdio>
#include <iostream>

#include <algorithm>

//using namespace std;

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

// Constructor

namespace ot {

	//initialize static member
	std::vector<std::string> CommonNodeFactory::knownNodes;

    CommonNodeFactory::CommonNodeFactory()
    {
        nodePorts.push_back("MergeAttribute");
        nodePorts.push_back("MergeTime");
        nodePorts.push_back("MergeTrigger");
        nodePorts.push_back("MergeDefault");
        nodePorts.push_back("TransformBase");
        nodePorts.push_back("Select");
        nodePorts.push_back("Arg1");
        nodePorts.push_back("Arg2");
        nodePorts.push_back("Gate");

        knownNodes.push_back("MergeNode");
        knownNodes.push_back("TriangulateOrientationNode");
        knownNodes.push_back("VirtualTransformation");
        knownNodes.push_back("DynamicTransformation");
		knownNodes.push_back("DynamicVirtualTransformation");
        knownNodes.push_back("InvertTransformation");
        knownNodes.push_back("MatrixTransformation");
        knownNodes.push_back("SelectionNode");
        knownNodes.push_back("ConfidenceSelectNode");
        knownNodes.push_back("FilterNode");
        knownNodes.push_back("ThresholdFilterNode");
        knownNodes.push_back("ButtonFilterNode");
        knownNodes.push_back("ButtonOpNode");
        knownNodes.push_back("TimeGateNode");
        knownNodes.push_back("EllipsoidTransformNode");
        knownNodes.push_back("GKTransformNode");
        knownNodes.push_back("RangeFilterNode");
        knownNodes.push_back("PositionFilterNode");
        knownNodes.push_back("PositionFilter");
        knownNodes.push_back("EventUtilityNode");
        knownNodes.push_back("EventUtility");
        knownNodes.push_back("ConfidenceFilterNode");
        knownNodes.push_back("Difference");		
        knownNodes.push_back("DESPFilter");
        knownNodes.push_back("EEGFilter");
        knownNodes.push_back("HeartrateFilter");
    }

    CommonNodeFactory::~CommonNodeFactory()
    {
        nodePorts.clear();
    }

    int CommonNodeFactory::parseVector(const std::string & line, float * val )
    {
        float help[3];
        int num;
        num = sscanf( line.c_str()," %f %f %f", &help[0], &help[1], &help[2] );
        if( num != 3 )
        {
            return 1;
        }
        val[0] = help[0];
        val[1] = help[1];
        val[2] = help[2];
        return 0;
    }

    int CommonNodeFactory::parseRotation(const std::string & line,const std::string & type, float * val )
    {
        int num;
        float matrix[3][3];

        if( type.compare("quaternion") == 0 )
        {
            float help[4];
            num = sscanf(line.c_str()," %f %f %f %f",&help[0], &help[1], &help[2], &help[3]);
            if( num != 4 )
            {
                return 1;
            }
            val[0] = help[0];
            val[1] = help[1];
            val[2] = help[2];
            val[3] = help[3];
            MathUtils::normalizeQuaternion( val );
        }
        else if( type.compare("matrix") == 0 )
        {
            num = sscanf(line.c_str()," %f %f %f %f %f %f %f %f %f",
                         &matrix[0][0], &matrix[0][1], &matrix[0][2],
                         &matrix[1][0], &matrix[1][1], &matrix[1][2],
                         &matrix[2][0], &matrix[2][1], &matrix[2][2] );
            if( num != 9 )
            {
                return 1;
            }
            else
            {
                float det = MathUtils::determinant( matrix );
                if( det < 0.9 || det > 1.1 )
                {
                    printf("Matrix is not a pure rotation matrix !\n");
                    return 1;
                }
                MathUtils::matrixToQuaternion( matrix, val );
            }
        }
        else if( type.compare("euler") == 0 )
        {
            float roll, pitch, yaw;
            num = sscanf(line.c_str()," %f %f %f", &roll, &pitch, &yaw );
            if( num != 3 )
            {
                return 1;
            }
            else
            {
                MathUtils::eulerToQuaternion( roll, pitch, yaw, val );
            }
        }
        else if( type.compare("axisangle") == 0 )
        {
            float axisa[4];
            num = sscanf(line.c_str()," %f %f %f %f", &axisa[0], &axisa[1], &axisa[2], &axisa[3] );
            if( num != 4 )
            {
                return 1;
            }
            else
            {
                MathUtils::axisAngleToQuaternion( axisa, val );
            }
        }
        else
        {
            return 1;
        }
        return 0;
    }

    // constructs a new Node.

    Node * CommonNodeFactory::createNode( const std::string& name, const StringTable& attributes)
    {
        float translation[3] = {0,0,0}, scale[3] = {1,1,1}, rot[4]={0,0,0,1};
        Node * result = NULL;
        if( name.compare("EventPositionTransform") == 0 ||
            name.compare("QueuePositionTransform") == 0 ||
            name.compare("TimePositionTransform") == 0 )
        {
            if( parseVector(attributes.get("translation"), translation ) != 0 )
            {
                logPrintE("Error parsing translation !\n");
            }
            if( parseVector(attributes.get("scale"), scale ) != 0 )
            {
                logPrintE("Error parsing scale !\n");
            }
            if( parseRotation(attributes.get("rotation"),
                              attributes.get("rotationtype"), rot ) != 0 )
            {
                logPrintE("Error parsing rotation !\n");
            }
            result =  new StaticTransformation( translation, scale, rot, true, false );
        }
        else if( name.compare("EventOrientationTransform") == 0 ||
                 name.compare("QueueOrientationTransform") == 0 ||
                 name.compare("TimeOrientationTransform") == 0 )
        {
            if( parseRotation(attributes.get("rotation"),
                              attributes.get("rotationtype"), rot ) != 0 )
            {
                logPrintE("Error parsing rotation !\n");
            }
            result = new StaticTransformation( translation, scale, rot, false, true );
        }
        else if( name.compare("EventTransform") == 0 ||
                 name.compare("QueueTransform") == 0 ||
                 name.compare("TimeTransform") == 0 )
        {
            if( parseVector(attributes.get("translation"), translation ) != 0 )
            {
                logPrintE("Error parsing translation !\n");
            }
            if( parseVector(attributes.get("scale"), scale ) != 0 )
            {
                logPrintE("Error parsing scale !\n");
            }
            if( parseRotation(attributes.get("rotation"),
                              attributes.get("rotationtype"), rot ) != 0 )
            {
                logPrintE("Error parsing rotation !\n");
            }
            result = new StaticTransformation( translation, scale, rot, true, true );
        }
        else if( name.compare("EventVirtualTransform") == 0 ||
                 name.compare("QueueVirtualTransform") == 0 ||
                 name.compare("TimeVirtualTransform") == 0 )
        {
            if( parseVector(attributes.get("translation"), translation ) != 0 )
            {
                logPrintE("Error parsing translation !\n");
            }
            if( parseRotation(attributes.get("rotation"),
                              attributes.get("rotationtype"), rot ) != 0 )
            {
                logPrintE("Error parsing rotation !\n");
            }
            result = new VirtualTransformation( translation, scale, rot, 1, 1 );
        }
        else if( name.compare("EventVirtualPositionTransform") == 0 ||
                 name.compare("QueueVirtualPositionTransform") == 0 ||
                 name.compare("TimeVirtualPositionTransform") == 0 )
        {
            if( parseVector(attributes.get("translation"), translation ) != 0 )
            {
                logPrintE("Error parsing translation !\n");
            }
            result = new VirtualTransformation( translation, scale, rot, 1, 0 );
        }
        else if( name.compare("EventVirtualOrientationTransform") == 0 ||
                 name.compare("QueueVirtualOrientationTransform") == 0 ||
                 name.compare("TimeVirtualOrientationTransform") == 0 )
        {
            if( parseRotation(attributes.get("rotation"),
                              attributes.get("rotationtype"), rot ) != 0 )
            {
                logPrintE("Error parsing rotation !\n");
            }
            result = new VirtualTransformation( translation, scale, rot, 0, 1 );
        }
#ifndef OT_NO_EVENTUTITLY_SUPPORT
        else if( name.compare("EventQueue") == 0 )
        {
            int length;
            if( attributes.get("length", &length) != 1 )
            {
                length = 1;
            }
            result = new EventQueueNode( length );
        }
#endif
#ifndef OT_NO_MERGENODE_SUPPORT
        else if( name.compare("Merge") == 0 )
        {
            float agingFactor;
            MergeNode::CONF_CALCULATION confCalculation;
            attributes.get("agingFactor", &agingFactor);
            if (attributes.get("confidenceCalculation").compare("max") == 0)
                confCalculation = MergeNode::MAX;
            else if (attributes.get("confidenceCalculation").compare("multiply") == 0)
                confCalculation = MergeNode::MULTIPLY;
            else
                confCalculation = MergeNode::MIN;
            result = new MergeNode(agingFactor, confCalculation);
        }
      	else if( name.compare("TriangulateOrientation") == 0)
	{
		result = new TriangulateOrientationNode();
	}

#endif
#ifndef OT_NO_SELECTION_SUPPORT
        else if( name.compare("Selection") == 0 )
        {
            double timeOut = 100;
            attributes.get("timeout", &timeOut, 1 );        
            result = new SelectionNode(timeOut);
        }
#endif
        else if( name.compare("EventDynamicTransform") == 0 ||
                 name.compare("QueueDynamicTransform") == 0 ||
                 name.compare("TimeDynamicTransform") == 0 )
        {
            int baseEvent;
            if( attributes.get("baseevent").compare("true") == 0 )
                baseEvent = 1;
            else
                baseEvent = 0;
            result = new DynamicTransformation( baseEvent, true, true );
        }
		else if( name.compare("EventDynamicVirtualTransform") == 0 )
		{
			int baseEvent;
			if( attributes.get("baseevent").compare("true") == 0 )
				baseEvent = 1;
			else
				baseEvent = 0;
			result = new DynamicVirtualTransformation( baseEvent, true, true );
		}
        else if( name.compare("EventDynamicPositionTransform") == 0 ||
                 name.compare("QueueDynamicPositionTransform") == 0 ||
                 name.compare("TimeDynamicPositionTransform") == 0 )
        {
            int baseEvent;
            if( attributes.get("baseevent").compare("true") == 0 )
                baseEvent = 1;
            else
                baseEvent = 0;
            result = new DynamicTransformation( baseEvent, true, false );
        }
        else if( name.compare("EventDynamicOrientationTransform") == 0 ||
                 name.compare("QueueDynamicOrientationTransform") == 0 ||
                 name.compare("TimeDynamicOrientationTransform") == 0 )
        {
            int baseEvent;
            if( attributes.get("baseevent").compare("true") == 0 )
                baseEvent = 1;
            else
                baseEvent = 0;
            result = new DynamicTransformation( baseEvent, false, true );
        }
        else if( name.compare("EventInvertTransform") == 0 ||
                 name.compare("QueueInvertTransform") == 0 ||
                 name.compare("TimeInvertTransform") == 0 )
        {
            result = new InvertTransformation();
        }
        else if( name.compare("EventMatrixTransform") == 0 ||
                 name.compare("QueueMatrixTransform") == 0 ||
                 name.compare("TimeMatrixTransform") == 0 )
        {
            float data[12];
            attributes.get( "matrix", data, 12 );
            result = new MatrixTransformation( data );
        }
#ifndef OT_NO_CONFIDENCE_SUPPORT
        else if( name.compare("ConfidenceFilter") == 0 )
        {       
            float treshold= 0.5;
            ConfidenceFilterNode::types type = ConfidenceFilterNode::HIGH;
            attributes.get("treshhold", &treshold, 1 );
            if( attributes.get("type").compare("high") == 0 )
                type = ConfidenceFilterNode::HIGH;
            else if( attributes.get("type").compare("low") == 0 )
                type = ConfidenceFilterNode::LOW;
            result = new ConfidenceFilterNode( treshold, type );
        }
#endif
        else if( name.compare("Filter") == 0 )
        {
            std::vector<float> weights;
            const char * data = attributes.get("weight").c_str();
            char * end = (char *) data;
            weights.push_back((float) strtod( data, &end ));
            while( end != data ){        
                data = end;
                weights.push_back((float) strtod( data, &end ));
            }
            weights.pop_back();
            FilterNode::Type type = FilterNode::ALL;
            if( attributes.get("type").compare("position") == 0)
                type = FilterNode::POSITION;
            else if( attributes.get("type").compare("orientation") == 0 )
                type = FilterNode::ORIENTATION;
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:FilterNode with %d weights\n"), weights.size()));
            result = new FilterNode( weights, type );
        }
#ifndef OT_NO_CONFIDENCE_SUPPORT
        else if( name.compare("ConfidenceSelect") == 0 )
        {
            double timeout = 100;
            ConfidenceSelectNode::types type = ConfidenceSelectNode::HIGH;
            attributes.get("timeout", &timeout, 1 );        
            if( attributes.get("type").compare("high") == 0 )
                type = ConfidenceSelectNode::HIGH;
            else if( attributes.get("type").compare("low") == 0 )
                type = ConfidenceSelectNode::LOW;
            result = new ConfidenceSelectNode( timeout, type );
        }
#endif
#ifndef OT_NO_THRESHOLDFILTER_SUPPORT
        else if( name.compare("ThresholdFilter") == 0 )
        {
            float posmin = 0, posmax = FLT_MAX, rotmin = 0, rotmax = 3.141592654f;
            if( attributes.get("positionmax").compare("inf") == 0 )
                posmax = FLT_MAX;
            else
                attributes.get("positionmax", &posmax, 1 );
            attributes.get("positionmin", &posmin, 1 );
            attributes.get("rotationmin", &rotmin, 1 );
            attributes.get("rotationmax", &rotmax, 1 );
            if( posmin < 0 )
                posmin = 0;
            if( posmax < posmin )
                posmax = posmin;
            if( rotmin < 0 )
                rotmin = 0;
            if( rotmax < rotmin )
                rotmax = rotmin;
            if( rotmax > 3.141592654f )
                rotmax = 3.141592654f;
            result = new ThresholdFilterNode( posmin, posmax, rotmin, rotmax );
        }
#endif
#ifndef OT_NO_BUTTON_SUPPORT
        else if( name.compare("ButtonFilter") == 0 )
        {
            result = new ButtonFilterNode( attributes.get("buttonmask").data(), attributes.get("buttonmap").data(), attributes.get("invert").data(), 
                                           attributes.get("validtrans").data() , attributes.get("radiobuttons").data(), attributes.get("setbuttononvalidtrans").data());
        }
#endif
#ifndef OT_NO_CONFIDENCE_SUPPORT
        else if( name.compare("ButtonOp") == 0 )
        {
            result = new ButtonOpNode( (attributes.get("op").compare("OR") == 0)?(ButtonOpNode::OR):(ButtonOpNode::AND));
        }
#endif
#ifndef OT_NO_TIMEGATE_SUPPORT
        else if( name.compare("TimeGate") == 0 )
        {
            double timeframe;
            attributes.get("timeframe", &timeframe );
            TimeGateNode::Mode mode = (attributes.get("mode").compare("pass") == 0)?
                (TimeGateNode::PASS):(TimeGateNode::BLOCK);
            result = new TimeGateNode( timeframe, mode );
        }
#endif
#ifndef OT_NO_ELLIPSOIDTRANSFORM_SUPPORT
        else if( name.compare("EventEllipsoidTransform") == 0 || 
                 name.compare("QueueEllipsoidTransform") == 0 ||
                 name.compare("TimeEllipsoidTransform") == 0 )
        {
            double a;
            double b;
            EllipsoidTransformNode::Mode mode;
            attributes.get("a", &a );
            if( attributes.containsKey("b"))
            {
                attributes.get("b", &b );
            }
            else
                b = a;
            if( attributes.get("mode").compare("toEllipsoid") == 0 )
                mode = EllipsoidTransformNode::toEllipsoid;
            else
                mode = EllipsoidTransformNode::toCartesian;
            result = new EllipsoidTransformNode( a, b, mode );
        }
#endif
#ifndef OT_NO_CONFIDENCE_SUPPORT
        else if( name.compare("EventGKTransform") == 0 || 
                 name.compare("QueueGKTransform") == 0 ||
                 name.compare("TimeGKTransform") == 0 )
        {
            double a = 0, b = 0, meridian = 0, alpha = 0, beta = 0, gamma = 0, delta = 0;
            GKTransformNode::Mode mode;
            attributes.get("a", &a );
            if( attributes.containsKey("b"))
            {
                attributes.get("b", &b );
            }
            else
                b = a;
            attributes.get("meridian", &meridian );
            attributes.get("alpha", &alpha );
            attributes.get("beta", &beta );
            attributes.get("gamma", &gamma );
            attributes.get("delta", &delta );		
            if( attributes.get("mode").compare("to") == 0 )
                mode = GKTransformNode::to;
            else
                mode = GKTransformNode::from;
            result = new GKTransformNode( a, b, meridian, alpha, beta, gamma, delta, mode );
        }
#endif
#ifndef OT_NO_RANGEFILTER_SUPPORT
        else if( name.compare("RangeFilter") == 0 )
        {
            float min, max;
            if( attributes.get("min", &min) != 1 )
                min = 0;
            if( attributes.get("max", &max ) != 1 )
                max = (float)sqrt(FLT_MAX)-0.1f;
            result = new RangeFilterNode( min, max );
        }
#endif
#ifndef OT_NO_POSITIONFILTER_SUPPORT
        else if( name.compare("PositionFilter") == 0 || name.compare("PositionFilterNode") == 0 )
        {
            float min[3], max[3];
            if( attributes.get("min", min, 3) != 3)
            { min[0] = -1; min[1] = -1; min[2] = -1;}
            if( attributes.get("max", max, 3) != 3 )
            { max[0] = 1; max[1] = 1; max[2] = 1;}
            result = new PositionFilterNode( min, max );
        }
#endif
#ifndef OT_NO_DESPFILTER_SUPPORT
        else if( name.compare("DESPFilter") == 0 )
        {
            float filterPos, filterOri;
            if( attributes.get("filter-position", &filterPos) != 1 )
                filterPos = 0.0;
            if( attributes.get("filter-orientation", &filterOri ) != 1 )
                filterOri = 0.0;
            result = new DESPFilterNode( filterPos, filterOri );
        }
#endif
        else if( name.compare("EEGFilter") == 0 )
        {
            std::vector<double> freqs;
            std::string inattrname;
            std::string outattrname;
            std::string triggerattrname;
            int codelength;
            int harmonics;
            bool consume;
            double samplerate;
            double prepause, postpause, interpause;
            double codeduration;
            double threshold;
            double hitpercentage;
            std::string logfile;

            inattrname = attributes.get("inattrname");
            if (inattrname == "" )
                inattrname = "eeg";
            outattrname = attributes.get("outattrname");
            if (outattrname == "" )
                outattrname = "eegout";
            triggerattrname = attributes.get("triggerattrname");
            if (triggerattrname == "" )
                triggerattrname = "eegtrigger";
            if( attributes.get("samplerate", &samplerate) != 1 )
                samplerate = 256.0;
            if( attributes.get("codelength", &codelength) != 1 )
                codelength = 2;
            if( attributes.get("harmonics", &harmonics) != 1 )
                harmonics = 3;
            if( attributes.get("prepause", &prepause) != 1 )
                prepause = 4000.0;
            if( attributes.get("postpause", &postpause) != 1 )
                postpause = 2000.0;
            if( attributes.get("interpause", &interpause) != 1 )
                interpause = 1000.0;
            if( attributes.get("codeduration", &codeduration) != 1 )
                codeduration = 4000.0;
            if( attributes.get("threshold", &threshold) != 1 )
                threshold = 1.0;
            if( attributes.get("hitpercentage", &hitpercentage) != 1 )
                hitpercentage = 80.0;
            if( attributes.get(std::string("frequencies"), freqs, 100) <2)
            {
                freqs.clear();
                freqs.push_back(6.25);
                freqs.push_back(8.00);
            }
            if( attributes.get("consume") == "" || 
                attributes.get("consume") == "false"  )
            {
                consume = false;
            }
            else
            {             
                consume = true;
            }
            logfile = attributes.get("logfile"); 
            result = new EEGFilterNode(freqs, codelength, samplerate, harmonics,
                                       prepause, postpause, interpause,
                                       codeduration,
                                       threshold, hitpercentage,
                                       inattrname, outattrname,
                                       triggerattrname,
                                       consume,
                                       logfile);

        }
        else if( name.compare("HeartrateFilter") == 0 )
        {
            std::string attrname;
            double threshold, toomuch;
            int onDuration, offDuration;
            int samplefactor;
            bool consume;

            if( attributes.containsKey("attrname") )
                attrname = attributes.get("attrname");
            else
                attrname = "bcidata";
            if( attributes.get("threshold", &threshold) != 1 )
                threshold = 0.03;
            if( attributes.get("toomuch", &toomuch) != 1 )
                toomuch = 0.2;
            if( attributes.get("onDuration", &onDuration) != 1 )
                onDuration = 4;
            if( attributes.get("offDuration", &offDuration) != 1 )
                offDuration = 100;
            if( attributes.get("samplefactor", &samplefactor) != 1 )
                samplefactor = 1;
            if( attributes.get("consume") == "" || 
                attributes.get("consume") == "false"  )
            {
                consume = false;
            }
            else
            {             
                consume = true;
            }
                        
            result = new HeartrateFilterNode(attrname, threshold, toomuch, onDuration, offDuration, samplefactor, consume);
        }
#ifndef OT_NO_EVENTUTITLY_SUPPORT
        else if ( name.compare("EventUtility") == 0 || name.compare("EventUtilityNode") == 0 )
        {
            std::string rename = attributes.get("rename");
            std::string create = attributes.get("create");
            std::string discard = attributes.get("discard");

            EventUtilityNode *resultNode = new EventUtilityNode();

            if (rename.length() > 0)
            {
                std::string::size_type firstBlank = rename.find(' ');
                if (firstBlank == std::string::npos)
                    logPrintE("Error parsing names for renaming in EventUtilityNode!\n");
                else
                {
                    std::string oldName = rename.substr(0, firstBlank);
                    std::string newName = rename.substr(firstBlank + 1, std::string::npos);
                    resultNode->setRename(oldName, newName);
                }
            }
            if (create.length() > 0)
            {
                std::string::size_type firstBlank = create.find(' ');
                std::string::size_type secondBlank = create.find(' ', firstBlank + 1);
                if (firstBlank == std::string::npos ||
                    secondBlank == std::string::npos)
                {
                    logPrintE("Error parsing serialized event for creation in EventUtilityNode!\n");
                }
                else
                {
                    std::string type = create.substr(0, firstBlank);
                    std::string name = create.substr(firstBlank + 1, secondBlank - (firstBlank + 1));
                    std::string value = create.substr(secondBlank + 1, std::string::npos);
                    resultNode->setCreate(type, name, value);
                }
            }
            if (discard.length() > 0)
                resultNode->setDiscard(discard);

            result = resultNode;
        }
#endif //OT_NO_EVENTUTITLY_SUPPORT
#ifndef OT_NO_DIFFERENCE_SUPPORT
        else if( name.compare("Difference") == 0 )
        {
            double timeFrame = 1000;
            attributes.get("timeframe", &timeFrame);
            result = new DifferenceNode( attributes.get("attributeName"), timeFrame );
        }
#endif //OT_NO_DIFFERENCE_SUPPORT

        // the node ports are just looked up in a simple list
        else if( std::find( nodePorts.begin(), nodePorts.end(), name ) != nodePorts.end())
        {
            logPrintI("Built NodePort %s.\n", name.c_str());
            return new NodePort();
        }
        if( result != NULL )
        {
            logPrintI("Built %s node.\n", name.c_str());
        }
        return result;
    }


	bool CommonNodeFactory::isKnownNode(std::string nodeName){
		bool result = false;
		std::vector<std::string>::iterator i = find(knownNodes.begin(), knownNodes.end(), nodeName);
		if (i != knownNodes.end())
			result= true;
		return result;
	}

    
} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of CommonNodeFactory.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
