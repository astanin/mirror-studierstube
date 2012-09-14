<?xml version="1.0" encoding="UTF-8"?>
<!--
OpenTracker Configuration to dot style sheet
Copyright (c) 2006,
Institute for Computer Graphics and Vision
Graz University of Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

Neither the name of the Graz University of Technology nor the names of
its contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:output method="text" encoding="ASCII"/>
	<!-- The basic part of a dot file -->
	<xsl:template match="OpenTracker">
digraph "<xsl:value-of select=".//NetworkSinkConfig/@name"/>" 
{ 
 	orientation = "landscape";
<xsl:apply-templates/>
	{ rank = same; <xsl:for-each select="*[position()>1]">
			<xsl:number count="*" format="a" level="any"/>;</xsl:for-each> }
} 
</xsl:template>
	<!-- template removing configuration part -->
	<xsl:template match="configuration" priority="2"/>
	<!-- template counts elements to provide unique node names and passes the current node name as parent to its children 
     after the node definition it calls the templates with mode node for any special definition -->
	<xsl:template match="*">
		<xsl:param name="parent"/>
		<xsl:number count="*" format="a" level="any"/>
		<xsl:apply-templates select="." mode="node"/>; 		
	<xsl:if test="not(name(..) = 'OpenTracker')">
			<xsl:number count="*" format="a" level="any"/> -> <xsl:copy-of select="$parent"/>; 
	</xsl:if>
		<!-- if the current element is a Ref element, add also an edge from the referenced element to the current -->
		<xsl:if test="name(.) = 'Ref'">
			<xsl:for-each select="//*[@DEF=current()/@USE]">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:for-each> -> <xsl:number count="*" format="a" level="any"/>;
	</xsl:if>
		<xsl:apply-templates select="." mode="navigate"/>
	</xsl:template>
	<!-- templates for customization of nodes, these have the mode node -->
	<!-- source nodes are round and may have some information -->
	<xsl:template match="ARToolKitSource" name="ARToolKitSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@tag-file"/>"]</xsl:template>
	<xsl:template match="NetworkSource" name="NetworkSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="ConsoleSource" name="ConsoleSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="InterSenseSource" name="InterSenseSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@id"/>"]</xsl:template>
	<xsl:template match="FastTrakSource" name="FastTrakSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="FileSource" name="FileSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@file"/> - <xsl:value-of select="@station"/>"]</xsl:template>
	<xsl:template match="FOBSource" name="FOBSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="StbKeyboardSource" name="StbKeyboardSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="ARTDataTrackerSource" name="ARTDataTrackerSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="TestSource" name="TestSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="WacomGraphireSource" name="WacomGraphireSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="CyberMouseSource" name="CyberMouseSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="ParButtonSource" name="ParButtonSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="JoystickSource" name="JoystickSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="StbMouseSource" name="StbMouseSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
   <xsl:template match="GPSSource" name="GPSSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
   <xsl:template match="GPSDirectionSource" name="GPSDirectionSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="ActiveGate" name="ActiveGate" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@group"/>"]</xsl:template>	
	<!-- sink nodes are round and may have some information -->
	<xsl:template match="ConsoleSink" name="ConsoleSink" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@comment"/>"]</xsl:template>
	<xsl:template match="NetworkSink" name="NetworkSink" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@name"/> - <xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="StbSink" name="StbSink" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@station"/>"]</xsl:template>
	<xsl:template match="FileSink" name="FileSink" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@file"/> - <xsl:value-of select="@station"/>"]</xsl:template>
	<xsl:template match="TCPSink" name="TCPSink" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<!-- other special node types -->
	<xsl:template match="EventDynamicTransform|QueueDynamicTransform|TimeDynamicTransform" mode="node">[shape=record,label="{{&lt;data&gt; Data | &lt;base&gt; Base } | &lt;bottom&gt; DynamicTransformation}"]</xsl:template>
	<xsl:template match="Ref" name="Ref" mode="node"> [ label="<xsl:value-of select="name(.)"/>" , shape=box , height=0.3 , width=.45 ]</xsl:template>
	<xsl:template match="Selection" name="Selection" mode="node">[shape=record,label="{{&lt;select&gt; Select | &lt;default&gt; Default } |&lt;bottom&gt; Selection}"]</xsl:template>
	<xsl:template match="ButtonOp" name="ButtonOp" mode="node">[shape=record,label="{{&lt;arg1&gt; Arg1 | &lt;arg2&gt; Arg2 }|&lt;bottom&gt; ButtonOp}"]</xsl:template>
     <xsl:template match="GroupGate" name="GroupGate" mode="node">[shape=record,label="{{&lt;default&gt; Default | &lt;override&gt; Override } | &lt;bottom&gt; GroupGate - <xsl:value-of select="@group"/>}"]</xsl:template>
	<!-- merge sub node templates for defining node ports -->
	<xsl:template match="Merge" name="Merge" mode="node">[shape=record, label="{{<xsl:apply-templates mode="merge"/>} | &lt;bottom&gt; <xsl:value-of select="name(.)"/>} "]</xsl:template>
	<xsl:template match="MergeDefault" mode="merge">&lt;default&gt; Default <xsl:if test="not(position()=last())">|</xsl:if>
	</xsl:template>
	<xsl:template match="MergePosition" mode="merge">&lt;position&gt; Position <xsl:if test="not(position()=last())">|</xsl:if>
	</xsl:template>
	<xsl:template match="MergeOrientation" mode="merge">&lt;orientation&gt; Orientation <xsl:if test="not(position()=last())">|</xsl:if>
	</xsl:template>
	<xsl:template match="MergeButton" mode="merge">&lt;button&gt; Button <xsl:if test="not(position()=last())">|</xsl:if>
	</xsl:template>
	<xsl:template match="MergeConfidence" mode="merge">&lt;confidence&gt; Confidence <xsl:if test="not(position()=last())">|</xsl:if>
	</xsl:template>
	<!-- default node style is a box and the name -->
	<xsl:template match="*" mode="node"> [ label="<xsl:value-of select="name(.)"/>", shape=box ]</xsl:template>
	<!-- templates for going down the children tree, this is needed to use dot node ports, these have mode navigate -->	
	<!-- ButtonOp children tree templates -->	
       <xsl:template match="ButtonOp" mode="navigate">
		<xsl:apply-templates mode="dynamic-navigate-buttonop">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Arg1" mode="dynamic-navigate-buttonop">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:arg1</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Arg2" mode="dynamic-navigate-buttonop">
		<xsl:param name="parent"/>
		<xsl:apply-templates select=".">
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:arg2</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<!-- Merge children tree templates -->
	<xsl:template match="Merge" mode="navigate">
		<xsl:apply-templates mode="merge-navigate">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeDefault" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:default</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergePosition" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:position</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeOrientation" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:orientation</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeButton" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:button</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeConfidence" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:confidence</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<!-- DynamicTransformation navigation behaviour -->
	<xsl:template match="EventDynamicTransform|QueueDynamicTransform|TimeDynamicTransform" mode="navigate">
		<xsl:apply-templates mode="dynamic-navigate">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="TransformBase" mode="dynamic-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:base</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="*" mode="dynamic-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates select=".">
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:data</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<!-- Selection navigation behaviour -->
	<xsl:template match="Selection" mode="navigate">
		<xsl:apply-templates mode="dynamic-navigate-selection">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Select" mode="dynamic-navigate-selection">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:select</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="*" mode="dynamic-navigate-selection">
		<xsl:param name="parent"/>
		<xsl:apply-templates select=".">
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:default</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<!-- GroupGate navigation behaviour -->
	<xsl:template match="GroupGate" mode="navigate">
		<xsl:apply-templates mode="dynamic-navigate-groupgate">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="Override" mode="dynamic-navigate-groupgate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:override</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="*" mode="dynamic-navigate-groupgate">
		<xsl:param name="parent"/>
		<xsl:apply-templates select=".">
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:default</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<!-- default navigate behaviour -->
	<xsl:template match="*" mode="navigate">
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
</xsl:stylesheet>
