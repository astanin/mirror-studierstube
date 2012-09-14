package at.ac.tuwien.ims.vrlu;

import at.ac.tuwien.ims.vrlu.*;

public class startInputCatcher {
	public static void main(String[] args){
		ButtonInputCatcher myBIC = new ButtonInputCatcher();
                myBIC.doSetup(args[0],args[1],args[2]);
                myBIC.start();
	}
}