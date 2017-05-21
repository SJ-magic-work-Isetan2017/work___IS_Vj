#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( int argc, char** argv  ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	if(argc < 2){
		ofRunApp(new ofApp(0)); // BOOT_MODE__AUTOPLAY
		
	}else{
		ofRunApp(new ofApp(1)); // BOOT_MODE__REC
	}

}
