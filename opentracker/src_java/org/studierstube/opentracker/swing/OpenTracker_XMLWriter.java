/**
 * 
 */
package org.studierstube.opentracker.swing;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

/**
 * @author kalkusch
 *
 */
public class OpenTracker_XMLWriter {

	
	public final class OpenTrackerFileFilterXML extends FileFilter {
		
		public final static String FILEEXTENSION_OPENTRACKER = "xml";
		
		 //Accept all directories and all gif, jpg, tiff, or png files.
	    public boolean accept(File f) {
	        if (f.isDirectory()) {
	            return true;
	        }

	        String extension = getExtension(f);
	        if (extension != null) {
	            if (extension.equalsIgnoreCase(FILEEXTENSION_OPENTRACKER)) {
	                    return true;
	            } else {
	                return false;
	            }
	        }

	        return false;
	    }

	    //The description of this filter
	    public String getDescription() {
	        return "OpenTracker XML (*.xml)";
	    }
	    
	    /**
	     * get extension if is does exist.
	     * 
	     * @param f File
	     * @return extension or null if no extension was available
	     */
	    private String getExtension(File f) {
	        String ext = null;
	        String s = f.getName();
	        int i = s.lastIndexOf('.');

	        if (i > 0 &&  i < s.length() - 1) {
	            ext = s.substring(i+1).toLowerCase();
	        }
	        return ext;
	    }
	}
	
	private File curDir;
	
	private boolean bIsFileDefined = false;
	
	private boolean bIsFileWritten = false;
	
	protected String sFileName = "opentracker_file.xml";
	
	protected JFileChooser chooser;
	
	protected File currentDir_xml = null;
		
	/**
	 * 
	 */
	public OpenTracker_XMLWriter() {
		chooser = new JFileChooser(curDir);
        FileFilter filter = new OpenTrackerFileFilterXML();
        chooser.setFileFilter( filter );
	}
	
	public void setFileName( final String sFileName ) {
		this.sFileName = sFileName;
		
		bIsFileDefined = true;
		bIsFileWritten = false;
	}
	
	public boolean setFileNameViaGUI( final String sPath ) {
		if ( currentDir_xml != null ) {
			chooser.setCurrentDirectory( currentDir_xml );
		}
		
		int res = chooser.showOpenDialog(null);
		
		if (res == JFileChooser.APPROVE_OPTION) {
			currentDir_xml = chooser.getSelectedFile();
	        
			if ( currentDir_xml != null ) {
		        sFileName = currentDir_xml.getAbsolutePath();
		        bIsFileDefined = true;
		        bIsFileWritten = false;
		        return true;
			}
			else {
				bIsFileDefined = false;
			    bIsFileWritten = false;
			    return false;
			}        
		} //end: if (res == JFileChooser.APPROVE_OPTION) {
		
		 return false;
	}
	
	public final String getFileName() {
		return this.sFileName;
	}
	
	public final boolean isFileNameSet() {
		return bIsFileDefined;
	}
	
	public final boolean isFileWrittenAtLeastOnce() {
		return bIsFileWritten;
	}
	
	public boolean writeXMLFile( final String sTextForXMLFile ) {		
		
		BufferedWriter bw = null;
		
		try {
			bw = new BufferedWriter( new FileWriter( this.sFileName ) );	
			
			boolean bStatus = false;			
			try {
				bw.write( sTextForXMLFile );	
				bw.flush();
				
				bStatus = true;
				bIsFileWritten = true;
				
			} catch ( IOException ioe) {
				System.err.println("error while writing  to file [" +sFileName +"]" );
				bIsFileWritten = false;
			} finally {
				bw.close();	
			}
			
			return bStatus;
		} catch ( IOException ioe) {
			System.err.println("can not open file [" +sFileName +"]" );
			bIsFileWritten = false;
		}
		return false;	
	}

}
