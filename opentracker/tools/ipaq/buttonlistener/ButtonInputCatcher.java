package at.ac.tuwien.ims.vrlu;

import java.awt.*;
import java.awt.event.*;
import java.awt.peer.*;
import java.net.*;
import java.io.*;
import javax.comm.*;
import java.util.*;

public class ButtonInputCatcher extends Canvas implements KeyListener{

  CommPort myComm = null;
  Socket mySocket = null;

  private static Frame myFrame = new Frame("ButtonInputCatcher");
  private static TextField myTextField = new TextField();
  private OutputStreamWriter myOutStream;

  protected static final int VK_LEFT = 37;
  protected static final int VK_RIGHT = 39;
  protected static final int VK_UP = 38;
  protected static final int VK_DOWN = 40;
  protected static final int VK_ENTER = 10;
  protected static final int VK_C = 67;
  protected static final int VK_Q = 81;
  protected static final int KEY_RELEASED = 0;
  protected static final int KEY_PRESSED = 128;

  protected boolean LeftPressed = false;
  protected boolean RightPressed = false;
  protected boolean UpPressed = false;
  protected boolean DownPressed = false;
  protected boolean EnterPressed = false;

    public void keyPressed(KeyEvent e){
      try{
      switch (e.getKeyCode()){
        case VK_LEFT:
          myTextField.setText("Left");
	    if(!LeftPressed){
	          myOutStream.write(VK_LEFT|KEY_PRESSED);
      	    myOutStream.flush();
	    }
	    LeftPressed = true;
          break;
        case VK_RIGHT:
          myTextField.setText("Right");
	    if(!RightPressed){
            myOutStream.write(VK_RIGHT|KEY_PRESSED);
            myOutStream.flush();
	    }
	    RightPressed = true;
          break;
        case VK_UP:
          myTextField.setText("Up");
	    if(!UpPressed){
            myOutStream.write(VK_UP|KEY_PRESSED);
            myOutStream.flush();
          }
	    UpPressed = true;
          break;
        case VK_DOWN:
          myTextField.setText("Down");
	    if(!DownPressed){
            myOutStream.write(VK_DOWN|KEY_PRESSED);
            myOutStream.flush();
	    }
	    DownPressed = true;
          break;
        case VK_ENTER:
          myTextField.setText("Enter");
	    if(!EnterPressed){
            myOutStream.write(VK_ENTER|KEY_PRESSED);
            myOutStream.flush();
	    }
	    EnterPressed = true;
          break;
        case VK_Q:
          myOutStream.write(VK_Q);
          myOutStream.flush();
          myFrame.dispose();
          myOutStream.close();
	    if(myComm!=null) myComm.close();
	    if(mySocket!=null) mySocket.close();
          System.exit(0);
          break;
        default:
          System.out.print("Unknown key: ");
          System.out.println(e.getKeyCode());
      }
      }
      catch(Exception ex){
        System.out.print("Error sending input message: ");
        System.out.println(ex.getMessage());
        System.exit(1);
      }
    }

    public void keyReleased(KeyEvent e){
      try{
	myTextField.setText("");
      switch (e.getKeyCode()){
        case VK_LEFT:
	    if(LeftPressed){
	          myOutStream.write(VK_LEFT|KEY_RELEASED);
      	    myOutStream.flush();
	    }
	    LeftPressed = false;
          break;
        case VK_RIGHT:
	    if(RightPressed){
            myOutStream.write(VK_RIGHT|KEY_RELEASED);
            myOutStream.flush();
	    }
	    RightPressed = false;
          break;
        case VK_UP:
	    if(UpPressed){
            myOutStream.write(VK_UP|KEY_RELEASED);
            myOutStream.flush();
          }
	    UpPressed = false;
          break;
        case VK_DOWN:
	    if(DownPressed){
            myOutStream.write(VK_DOWN|KEY_RELEASED);
            myOutStream.flush();
	    }
	    DownPressed = false;
          break;
        case VK_ENTER:
	    if(EnterPressed){
            myOutStream.write(VK_ENTER|KEY_RELEASED);
            myOutStream.flush();
	    }
	    EnterPressed = false;
          break;
        default:
          System.out.print("Unknown key: ");
          System.out.println(e.getKeyCode());
      }
      }
      catch(Exception ex){
        System.out.print("Error sending input message: ");
        System.out.println(ex.getMessage());
        System.exit(1);
      }
    }

    public void keyTyped(KeyEvent e){
    }

    public void doSetup(String method, String address, String port){
      myFrame.setBounds(0,0,239,319);
      myFrame.addKeyListener(this);
      myTextField.addKeyListener(this);
      myTextField.setBounds(50,50,30,10);
      myFrame.add(myTextField);
      connect(method,address,port);
    }

    public void connect(String method, String address, String port){
	  Enumeration myPorts = CommPortIdentifier.getPortIdentifiers();
	
	  	try{	
		    if(method.compareTo("COM")==0){
			CommPortIdentifier myPortId = (CommPortIdentifier)(myPorts.nextElement());

			myPortId = CommPortIdentifier.getPortIdentifier(address);
			myComm = myPortId.open("ButtonInputCatcher",2500);
	  		System.out.println("Comm port aquired");
	  		System.out.flush();
  			myOutStream = new OutputStreamWriter(myComm.getOutputStream());
  			System.out.print("Connection established with ");
  			System.out.println(address);
		    }
		    else if(method.compareTo("NET")==0){
			mySocket = new Socket(address,new Integer(port).intValue());
			myOutStream = new OutputStreamWriter(mySocket.getOutputStream());
			System.out.print("Connection established with ");
			System.out.println(address + ":" + port);
		    }
      	}
		catch(NoSuchPortException nsp){
	  		System.out.print("No such port: ");
	  		System.out.println(myComm.getName());
			System.exit(1);
		}
		catch(IOException io){
  	  		System.out.print("Error setting up output stream: ");
	  		System.out.println(io.getMessage());
			System.exit(1);
      	}
      	catch(PortInUseException piu){
	  		System.out.print("Port in use: ");
	  		System.out.println(address);
			System.exit(1);
		}
    }

    public void start(){
      myFrame.show();
    }

}
