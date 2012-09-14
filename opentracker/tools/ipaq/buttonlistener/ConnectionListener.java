package at.ac.tuwien.ims.vrlu;

import at.ac.tuwien.ims.vrlu.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.peer.*;
import java.io.*;
import javax.comm.*;
import java.util.*;
import java.lang.*;

public class ConnectionListener {
  	protected static final int VK_LEFT = 37;
	protected static final int VK_RIGHT = 39;
  	protected static final int VK_UP = 38;
  	protected static final int VK_DOWN = 40;
  	protected static final int VK_ENTER = 10;
  	protected static final int VK_C = 67;
  	protected static final int VK_Q = 81;
  	protected static final int KEY_RELEASED = 0;
	protected static final int KEY_PRESSED = 128;

      private boolean pressed = false;
      private int lastKey = 0;
	private static byte myPackage[] = {
		(byte)0xbe, (byte)0xef,		// magic number
		0x02, 0x00,				// rev number
		0x00, 0x0f,				// maxStationNum
		0x00, 0x0c,				// headerLength
		0x00, 0x01,				// number of Stations
		0x00, 0x00,				// comment length
		0x00, 0x00,				// station number
		0x00, 0x01,				// format = quaternion
		0x00, 0x00,				// button events
		0x00, 0x26,				// bytes per station
		0x00, 0x00,				// station name length
		0x00, 0x00, 0x00, 0x00,		// position x				
		0x00, 0x00, 0x00, 0x00,		// position y
		0x00, 0x00, 0x00, 0x00,		// position z
		0x00, 0x00, 0x00, 0x00,		// orientation 1
		0x00, 0x00, 0x00, 0x00,		// orientation 2
		0x00, 0x00, 0x00, 0x00,		// orientation 3
		0x00, 0x00, 0x00, 0x00		// orientation 4
	};
	private static boolean leftDown = false;
	private static boolean rightDown = false;
	private static boolean upDown = false;
	private static boolean downDown = false;
	private static boolean enterDown = false;

      public static void main(String args[]){
        InputStreamReader myInStream = null;
        ServerSocket myServer = null;
        Socket mySocket = null;
	  DatagramSocket mySender = null;
	  CommPortIdentifier myPortId = null;
	  CommPort myComm = null;

        int myChar=0;
	try{
	  String addr = "224.100.200.101";
	  mySender = new DatagramSocket();
	  InetAddress myAddr = InetAddress.getByName(addr);
	  mySender.connect(myAddr,5556);

	  if((new String(args[0])).compareTo("COM")==0){
	    System.out.println("Getting Port List");
	    System.out.flush();
	    Enumeration myPorts = CommPortIdentifier.getPortIdentifiers();
	    System.out.println("Listing ports");
	    System.out.flush();

	    Object myObj = null;
          if(myPorts==null){
            System.out.println("No CommPort Enumeration object!");
            System.out.flush();
          }
          else{
            try{
	      do{
                myObj = myPorts.nextElement();
                if(myObj!=null){
  		  System.out.print("Comm Port found:");
                  System.out.println(((CommPortIdentifier)myObj).getName());
	          System.out.flush();
                }
	      }while(myObj!=null);
            }
            catch(NoSuchElementException nse){
              System.out.print("NoSuchElementException: ");
	      System.out.println(nse.getMessage());
              System.out.flush();
            }
          }
          myPortId = CommPortIdentifier.getPortIdentifier(args[1]);
	    if (myPortId!=null) myComm = myPortId.open("ButtonInputCatcher",2500);
	    if (myComm!=null) myInStream = new InputStreamReader(myComm.getInputStream());
	    System.out.print("Listening for input at ");
	    System.out.println(myPortId.getName());
	  }
	  else if((new String(args[0])).compareTo("NET")==0){
		myServer = new ServerSocket(Integer.parseInt(args[1]));
		mySocket = myServer.accept();
		myInStream = new InputStreamReader(mySocket.getInputStream());
	  	System.out.println("Connection established");
	  }
	}
	catch(NoSuchPortException nsp){
  	  System.out.print("NoSuchPortException: ");
	  System.out.println(nsp.getMessage());
          System.out.flush();
          System.exit(1);
	}
	catch(IOException io){
	  System.out.print("IOException: ");
	  System.out.println(io.getMessage());
          System.out.flush();
          System.exit(1);
	}
      catch(PortInUseException piu){
	  System.out.print("Port in Use: ");
	  System.out.println(piu.getMessage());
          System.out.flush();
          System.exit(1);
	}
	catch(Exception uhe){
	  System.out.print("Error connecting to multicast address: ");
	  System.out.println(uhe.getMessage());
          System.out.flush();
          System.exit(1);
	}
	


        Frame myFrame = new Frame("iPAQ Input");
        TextField myTextField = new TextField();

        try{
          myFrame.setBounds(0,0,239,319);
          myTextField.setBounds(50,50,30,10);
          myFrame.add(myTextField);
          myFrame.show();
        }
	catch(Exception e){
	  System.out.print("Error setting up Window: ");
	  System.out.println(e.getMessage());
          System.exit(1);
	}
        try{
          do{
      	  myChar = myInStream.read();
              switch (myChar){
                case (VK_LEFT|KEY_PRESSED):
                  myTextField.setText("Left");
			leftDown = true;
                  break;
		    case (VK_LEFT|KEY_RELEASED):
                  myTextField.setText("");
			leftDown = false;
                  break;
                case (VK_RIGHT|KEY_PRESSED):
                  myTextField.setText("Right");
			rightDown = true;
                  break;
                case (VK_RIGHT|KEY_RELEASED):
                  myTextField.setText("");
			rightDown = false;
                  break;
                case (VK_UP|KEY_PRESSED):
                  myTextField.setText("Up");
			upDown = true;
                  break;
                case (VK_UP|KEY_RELEASED):
                  myTextField.setText("");
			upDown = false;
                  break;
                case (VK_DOWN|KEY_PRESSED):
                  myTextField.setText("Down");
			downDown = true;
                  break;
                case (VK_DOWN|KEY_RELEASED):
                  myTextField.setText("");
			downDown = false;
                  break;
                case (VK_ENTER|KEY_PRESSED):
                  myTextField.setText("Enter");
			enterDown = true;
                  break;
                case (VK_ENTER|KEY_RELEASED):
                  myTextField.setText("");
			enterDown = false;
                  break;
              }
  		  myPackage[17] = 0;
		  if(upDown) myPackage[17]+=1;
		  if(leftDown) myPackage[17]+=2;
		  if(downDown) myPackage[17]+=4;
		  if(rightDown) myPackage[17]+=8;
		  if(enterDown) myPackage[17]+=16;
		  System.out.print("Sending packet with button events: ");
		  System.out.println(myPackage[17]);
		  mySender.send(new DatagramPacket(myPackage,33));

          }while (myChar!=VK_Q);
        }
        catch(Exception e){
          System.out.print("Error reading from stream: ");
	  System.out.println(e.getMessage());
          System.exit(1);
        }

        try{
	    System.out.println("Connection terminated.");
	    System.out.flush();
          myFrame.dispose();
          myInStream.close();
          if(mySocket!=null) mySocket.close();
          if(myServer!=null) myServer.close();
	    if(myComm!=null) myComm.close();
          System.exit(0);
        }
        catch(Exception e){
          System.out.print("Error cleaning up: ");
	  System.out.println(e.getMessage());
          System.exit(1);
        }
      }
}
