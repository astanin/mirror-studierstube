/* ==========================================
 * JGraphT : a free Java graph-theory library
 * ==========================================
 *
 * Project Info:  http://jgrapht.sourceforge.net/
 * Project Lead:  Barak Naveh (http://sourceforge.net/users/barak_naveh)
 *
 * (C) Copyright 2003-2004, by Barak Naveh and Contributors.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 */

package org.studierstube.opentracker.swing;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.geom.Rectangle2D;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.util.Hashtable;

import javax.swing.JApplet;
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPanel;

import org._3pq.jgrapht.Edge;
import org._3pq.jgrapht.ext.JGraphModelAdapter;

import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.GraphConstants;
import org.studierstube.opentracker.OpenTrackerListenableDirectedMultigraph;
import org.studierstube.opentracker.OpenTrackerNodeType;
import org.studierstube.opentracker.event.OT_GraphModelListener;
import org.studierstube.opentracker.node.AbstractOpenTrackerNode;
import org.studierstube.opentracker.node.OT_ConsoleNode;
import org.studierstube.opentracker.node.OT_EventTransformNode;
import org.studierstube.opentracker.node.OT_NetworkSinkNode;
import org.studierstube.opentracker.node.OT_MergeNode;

/**
 * JGraphOpenTrackerEditor.
 *
 * @author Michael Kalkusch
 *
 */
public class JGraphOpenTrackerEditor extends JApplet {
	
    private static final long      serialVersionUID = 3256444702936019250L;
    private static final Color     DEFAULT_BG_COLOR = Color.decode( "#FAFBFF" );
    private static final Dimension DEFAULT_SIZE     = new Dimension( 530, 320 );

    protected JGraphOpenTrackerEditor applet;
    
    private JFrame jf_frame;
    
    private Container jc_canvas;
    
    private JLabel jl_statusBar;
    
    private JTextField jtf_targetFileName;
    
    private JButton jb_writeXMLButton;
    private JButton jb_writeXMLPickFile;
    private JButton jb_insertNode;
    private JButton jb_insertEdge;
    private JButton jb_toggleNodeType;
    
//    private int iVertexIndex = 1;
    
    private JPanel jp_statusPanel;
    
    private String sTargetFilename = "my_xml_ot_file.xml";
    
    private OpenTrackerListenableDirectedMultigraph graph;
    
    private OT_GraphModelListener graphListener;
    
//    private DefaultGraphSelectionModel graphSelectionModel;
    
    private Hashtable <Integer,AbstractOpenTrackerNode> vecVertex;
    
    private Hashtable <Integer,Edge> vecEdges;
    
    private int iCurrentUniqueNodeId = 0;
    
    private int iCurrentUniqueEdgeId = 1000;
    
    private final int iUniqueEdgeIdInc = 1000;
    
    private final int iUniqueNodeIdInc = 1;
     
    private int iCurrentNodeTypeIndex = 0;
    
    private OpenTrackerNodeType enumCurrentNodeType = 
    	OpenTrackerNodeType.CONSOLE_SINK;
    
    //
    private JGraphModelAdapter m_jgAdapter;

    private OpenTracker_XMLWriter fileWriter;
    
    /**
     * An alternative starting point for this demo, to also allow running this
     * applet as an application.
     *
     * @param args ignored.
     */
    public static void main( String[] args ) {
        JGraphOpenTrackerEditor applet = new JGraphOpenTrackerEditor(  );
                
        applet.initMain();
    }


    public void initMain() {
    	
    	fileWriter = new OpenTracker_XMLWriter();
    	
    	vecVertex = new Hashtable <Integer,AbstractOpenTrackerNode> ();
        vecEdges = new Hashtable <Integer,Edge> ();
        
    	applet = new JGraphOpenTrackerEditor();    	
    	jf_frame = new JFrame();    	
        jl_statusBar = new JLabel("status bar");        
        jtf_targetFileName = new JTextField( sTargetFilename );        
        jb_writeXMLButton = new JButton("write XML");
        jb_insertNode = new JButton("add Node");
        jb_insertEdge = new JButton("add Edge");
        jb_writeXMLPickFile = new JButton("save as..");
        jb_toggleNodeType = new JButton( enumCurrentNodeType.getNodeButtonText() );
        jp_statusPanel = new JPanel();        
        
        jp_statusPanel.setLayout( new FlowLayout() );
        jp_statusPanel.add( jl_statusBar );
        jp_statusPanel.add( jb_insertNode );
        jp_statusPanel.add( jb_toggleNodeType );
        jp_statusPanel.add( jb_insertEdge );
        jp_statusPanel.add( jb_writeXMLButton );
        jp_statusPanel.add( jb_writeXMLPickFile );
        jp_statusPanel.add( jtf_targetFileName );
                
        jtf_targetFileName.setEditable( false );
        jb_writeXMLPickFile.setEnabled( false );
        
    	jc_canvas = jf_frame.getContentPane();
        jc_canvas.setLayout( new BorderLayout() );
        //jc_canvas.add( applet, BorderLayout.CENTER );
        jc_canvas.add( jp_statusPanel, BorderLayout.SOUTH );
        
        init();
        
        jb_writeXMLButton.addActionListener( new ActionListener() {
        		public void actionPerformed(ActionEvent ae) {        			
        			if (( ! fileWriter.isFileNameSet() )||
        					( ! fileWriter.isFileWrittenAtLeastOnce())) {
        				jl_statusBar.setText("select file.." );
        				fileWriter.setFileNameViaGUI( jtf_targetFileName.getText() );
        				
        				jtf_targetFileName.setText( fileWriter.getFileName() );
        				jl_statusBar.setText("save .." );
        				
        				jb_writeXMLPickFile.setEnabled( true );
        			}        			
        			if ( fileWriter.writeXMLFile( graph.listEdgesAndNodes() ) ) {
        				jl_statusBar.setText("save done" );
        			} else {
        				jl_statusBar.setText("error while writing to " + fileWriter.getFileName() );
        			}
        			
        		}
        });
        
        jb_writeXMLPickFile.addActionListener( new ActionListener() {
        		public void actionPerformed(ActionEvent ae) {  
        			jl_statusBar.setText("save as .." ); 
        			
        			if ( fileWriter.setFileNameViaGUI( jtf_targetFileName.getText() ) ) {
	        			jtf_targetFileName.setText( fileWriter.getFileName() );	        			
	    			
	        			if ( fileWriter.writeXMLFile( graph.listEdgesAndNodes() ) ) {
	        				jl_statusBar.setText("save as .. done" );
	        			} else {
	        				jl_statusBar.setText("error while writing to " + fileWriter.getFileName() );
	        			}
        			} else {
        				jl_statusBar.setText( "abort save" );
        			}
        			
        		}
        });
        
        jb_insertNode.addActionListener( new ActionListener() {
    		public void actionPerformed(ActionEvent ae) {
    			
    			createNode(-1,enumCurrentNodeType);
    			
    			jl_statusBar.setText("insert new " + enumCurrentNodeType.getNodeText() );
    		}
        });
        
        jb_insertEdge.addActionListener( new ActionListener() {
    		public void actionPerformed(ActionEvent ae) {
    			
    			createEdgeFromNodeId( 1, 2 );
    			
    			jl_statusBar.setText("insert new edge");
    		}
        });
        
        jb_toggleNodeType.addActionListener( new ActionListener() {
    		public void actionPerformed(ActionEvent ae) {
    			
    			jb_toggleNodeType.setText( toggleCreationNodeType() );
    			jl_statusBar.setText("toggle node: " + enumCurrentNodeType.getNodeText() );    			
    		}
        });
        
        
    	jf_frame.setTitle( "OpenTracker 2.0 - Visual JGraph Editor v0.1" );
        jf_frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
        jf_frame.setPreferredSize( new Dimension(1000, 600) );
        jf_frame.pack(  );
        jf_frame.setVisible( true );
        
       
    }
    
    public AbstractOpenTrackerNode createNode( final int iVertexId, OpenTrackerNodeType nodeType ) {
    	int iNewNodeId;
    	
    	if ( iVertexId < 0 ) {
    		iNewNodeId = currentUniqueNodeId();
    	} else {
    		iNewNodeId = iVertexId;
    	}
    	
    	AbstractOpenTrackerNode newNode = null;
    	
    	switch ( nodeType ) {
    	
    	case CONSOLE_SINK:
    		newNode= new OT_ConsoleNode(graph,iNewNodeId);
    		break;
    		
    	case TRANSFORM_NODE:
    		newNode= new OT_EventTransformNode(graph,iNewNodeId);
    		break;
    		
    	case NETWORK_SINK:
    		newNode= new OT_NetworkSinkNode(graph,iNewNodeId);
    		break;
    		
    	case MERGE_NODE:
    		newNode= new OT_MergeNode(graph,iNewNodeId);
    		break;
    		
    	default:
    		System.err.println("Unsupported node type [" + nodeType.toString() + "]" );
    		return null;
    	    	
    	}
    	
    	vecVertex.put( new Integer(iNewNodeId), newNode );    	    	
    	
    	return newNode;
    }
    
    private int currentUniqueNodeId() {
    	return iCurrentUniqueNodeId += iUniqueNodeIdInc;
    }
    
    private int createUniqueEdgeId() {
    	return iCurrentUniqueEdgeId += iUniqueEdgeIdInc;
    }
    
    public AbstractOpenTrackerNode getNodeById( final int iNodeId ) {
    	return vecVertex.get( new Integer(iNodeId) );
    }
    
    public boolean createEdgeFromNodeId( final int iSourceNodeId, final int iTargetNodeId ) {
    	
    	if ( iTargetNodeId == iSourceNodeId ) {
    		System.err.println("Try to create an edged from one node to itself! Ingore comamnd.");
    	}
    	
    	AbstractOpenTrackerNode source = getNodeById(iSourceNodeId);
    	
    	if ( source == null ) {
    		return false;
    	}
    	AbstractOpenTrackerNode target = getNodeById(iTargetNodeId);
    	
    	if ( target == null ) {
    		return false;
    	}
    	
    	//OT_Edge newEdge = new OT_Edge( source, target, createUniqueEdgeId() );
    	
    	Edge newEdge = graph.addEdge( source, target );
    	
    	//graph.addEdge( newEdge );
    	
    	System.out.println(" Edge: " + newEdge.toString() );
    	
    	vecEdges.put( new Integer( createUniqueEdgeId() ),  newEdge );
    	
    	return true;
    }
    
    public boolean removeEdge( Edge removeEdge ) {
    	
    	if( vecEdges.contains(removeEdge) ) {
    		graph.removeEdge( removeEdge );
    		vecEdges.remove( removeEdge );
    		
    		return true;
    	}
    	
    	return false;
    }
    
    /**
     * {@inheritDoc}
     */
    public void init(  ) {
        // create a JGraphT graph
        graph = new OpenTrackerListenableDirectedMultigraph(  );

        // create a visualization using JGraph, via an adapter
        m_jgAdapter = new JGraphModelAdapter( graph );
        graphListener = new OT_GraphModelListener( graph ); 
        m_jgAdapter.addGraphModelListener( graphListener );
               


        JGraph jgraph = new JGraph( m_jgAdapter );

        adjustDisplaySettings( jgraph );
        jc_canvas.add( jgraph , BorderLayout.CENTER);
        resize( DEFAULT_SIZE );
        
        createNode(-1,OpenTrackerNodeType.CONSOLE_SINK);
        createNode(-1,OpenTrackerNodeType.CONSOLE_SINK);
        createNode(-1,OpenTrackerNodeType.CONSOLE_SINK);
        createNode(-1,OpenTrackerNodeType.TRANSFORM_NODE);
        createNode(-1,OpenTrackerNodeType.NETWORK_SINK);
        createNode(-1,OpenTrackerNodeType.NETWORK_SINK);
        createNode(-1,OpenTrackerNodeType.MERGE_NODE);
        
        
        createEdgeFromNodeId(1,5);
        createEdgeFromNodeId(1,3);
        createEdgeFromNodeId(2,4);
        createEdgeFromNodeId(4,5);
        createEdgeFromNodeId(2,6);
       
        // position vertices nicely within JGraph component        
        positionVertexAt( getNodeById(1), 200, 400 );
        positionVertexAt( getNodeById(2), 600, 400 );
        positionVertexAt( getNodeById(3), 100, 200 );
        positionVertexAt( getNodeById(4), 400, 200 );
        positionVertexAt( getNodeById(5), 250, 50 );
        positionVertexAt( getNodeById(6), 700, 200 );

        // that's all there is to it!...
    }


    private String toggleCreationNodeType() {
    	
    	if ( iCurrentNodeTypeIndex++ >= OpenTrackerNodeType.getMaxiumumIndex() ) {
    		iCurrentNodeTypeIndex = 0;
    	}
    	enumCurrentNodeType = OpenTrackerNodeType.getTypeByIndex(iCurrentNodeTypeIndex);
    	
    	return enumCurrentNodeType.getNodeButtonText();
    }
    
    private void adjustDisplaySettings( JGraph jg ) {
        jg.setPreferredSize( DEFAULT_SIZE );

        Color  c        = DEFAULT_BG_COLOR;
        String colorStr = null;

        try {
            colorStr = getParameter( "bgcolor" );
        }
         catch( Exception e ) {}

        if( colorStr != null ) {
            c = Color.decode( colorStr );
        }

        jg.setBackground( c );
    }


    private void positionVertexAt( Object vertex, int x, int y ) {
        DefaultGraphCell cell   = m_jgAdapter.getVertexCell( vertex );
        AttributeMap     attr   = cell.getAttributes(  );
        Rectangle2D      bounds = GraphConstants.getBounds( attr );

        Rectangle2D      newBounds =
            new Rectangle2D.Double( x, y, bounds.getWidth(  ),
                bounds.getHeight(  ) );

        GraphConstants.setBounds( attr, newBounds );

        AttributeMap cellAttr = new AttributeMap(  );
        cellAttr.put( cell, attr );
        m_jgAdapter.edit( cellAttr, null, null, null );
    }
  
}
