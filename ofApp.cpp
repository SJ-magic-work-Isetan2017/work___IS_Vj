/************************************************************
************************************************************/
#include "ofApp.h"

#include <time.h>


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _BootMode)
: Osc_MusicPlayer("127.0.0.1", 12347, 12350)
, MusicTime_ms(-1)
, b_DispTime(true)
{
	srand((unsigned) time(NULL));
	
	for(int i = 0; i < NUM_GUIS; i++){
		b_DispGui[i] = false;
	}
	
	BootMode = BOOT_MODE(_BootMode);
}

/******************************
******************************/
ofApp::~ofApp()
{
}

/******************************
******************************/
void ofApp::exit()
{
#if(EXE_MODE == EXE__DEMO)
	StageManager.exit();
#endif

	fclose_LogFile();
	printf("\n> Good bye\n");
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	fopen_LogFile();
	
	/********************
	********************/
	font.loadFont("RictyDiminished-Regular.ttf", 15);
	
	/********************
	********************/
	ofSetWindowTitle("VJ");
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
	
	/********************
	********************/
	if(BootMode == BOOT_MODE__AUTOPLAY)	b_fullScreen = true;
	else								b_fullScreen = false;
	ofSetFullscreen(b_fullScreen);
	
	/********************
	********************/
	setup_Gui();
	
	/********************
	********************/
	UseFftResult.setup();
	
	/********************
	********************/
#if(EXE_MODE == EXE__DEBUG_CHAR)
	test_Character.setup();
#elif(EXE_MODE == EXE__DEBUG_STAGE)
	test_Stage.setup();
#elif(EXE_MODE == EXE__DEMO)
	StageManager.setup();
#endif

	/********************
	********************/
	Guis_LoadSetting();
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	for(int i = 0; i < 2; i++){
		Gui_FFT[i] = new GUI_FFT;
	}
	Gui_FFT[0]->setup("FFT 0", "FFT_0.xml", 10, 10);
	Gui_FFT[1]->setup("FFT 1", "FFT_1.xml", 230, 10);
	
	/********************
	********************/
	for(int i = 0; i < 3; i++){
		Gui_Character[i] = new GUI_CHARACTER;
	}
	Gui_Character[0]->setup("Character num 1", "Char_0.xml", 10, 10);
	Gui_Character[1]->setup("Character num 2", "Char_1.xml", 230, 10);
	Gui_Character[2]->setup("Character num 3", "Char_2.xml", 450, 10);
	
	/********************
	********************/
	Gui_Stage = new GUI_STAGE;
	Gui_Stage->setup("Stage", "Stage.xml", 10, 10);
	
	/********************
	********************/
	Gui_Cam = new GUI_CAM;
	Gui_Cam->setup("cam", "cam.xml", 10, 10);
}

/******************************
******************************/
void ofApp::update_OscFrom_MusicPlayer()
{
	while(Osc_MusicPlayer.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc_MusicPlayer.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/Quit"){
			ofExit();
			return;
			// std::exit(1);
			
		}else if(m_receive.getAddress() == "/MusicTime"){
			MusicTime_ms = m_receive.getArgAsInt32(0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	/********************
	********************/
	ElapsedTime = ofGetElapsedTimef();
	
	/********************
	********************/
	update_OscFrom_MusicPlayer();
	
	/********************
	********************/
	UseFftResult.update();

	/********************
	********************/
#if(EXE_MODE == EXE__DEBUG_CHAR)
	test_Character.update(UseFftResult.get_Gain(1));
#elif(EXE_MODE == EXE__DEBUG_STAGE)
	test_Stage.update(UseFftResult.get_Gain(1));
#elif(EXE_MODE == EXE__DEMO)
	StageManager.update(MusicTime_ms, UseFftResult.get_Gain(1));
#endif
}

/******************************
******************************/
void ofApp::draw_time()
{
	/********************
	********************/
	char buf[BUF_SIZE];
	
	if(MusicTime_ms == -1){
		sprintf(buf, "--STOP--");
		
	}else{
		int min	= MusicTime_ms / 1000 / 60;
		int sec	= MusicTime_ms / 1000 - min * 60;
		int ms	= MusicTime_ms % 1000;
		
		sprintf(buf, "%6d:%6d:%6d\n%6.1f", min, sec, ms, ofGetFrameRate());
	}
	
	
	/********************
	********************/
	// ofSetColor(255, 255, 255);
	ofSetColor(180);
	
	font.drawString(buf, 10, 50);
}

//--------------------------------------------------------------
void ofApp::draw(){

	/********************
	********************/
	fbo.begin();
		/********************
		********************/
		ofBackground(0);
		ofSetColor(255);
		
		/********************
		********************/
#if(EXE_MODE == EXE__DEBUG_CHAR)
		test_Character.draw(UseFftResult.get_Gain(1));
#elif(EXE_MODE == EXE__DEBUG_STAGE)
		test_Stage.draw();
#elif(EXE_MODE == EXE__DEMO)
		StageManager.draw();
#endif
		
		/********************
		********************/
		UseFftResult.draw();
		
		/********************
		********************/
		if(b_DispTime) draw_time();
	
	fbo.end();
	
	
	/********************
	********************/
	ofPushStyle();
	ofDisableAlphaBlending();
	
	ofBackground(0);
	ofSetColor(255);
	
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	drawGuis();
	ofPopStyle();
}

/******************************
******************************/
void ofApp::drawGuis()
{
	if(b_DispGui[GUI_ID__FFT]){
		for(int i = 0; i < 2; i++) Gui_FFT[i]->gui.draw();
	}
	
	if(b_DispGui[GUI_ID__CHARACTER]){
		for(int i = 0; i < 3; i++) Gui_Character[i]->gui.draw();
	}
	
	if(b_DispGui[GUI_ID__STAGE]) Gui_Stage->gui.draw();
	
	if(b_DispGui[GUI_ID__CAM]) Gui_Cam->gui.draw();
}

/******************************
******************************/
void ofApp::Guis_LoadSetting()
{
	/********************
	********************/
	printf("\n> Gui Load setting\n");
	
	/********************
	********************/
	string FileName;
	
	/********************
	********************/
	FileName = "FFT_0.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_FFT[0/**/]->gui.loadFromFile(FileName.c_str());
	
	FileName = "FFT_1.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_FFT[1/**/]->gui.loadFromFile(FileName.c_str());
	
	/********************
	********************/
	FileName = "Char_0.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_Character[0/**/]->gui.loadFromFile(FileName.c_str());
	
	FileName = "Char_1.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_Character[1/**/]->gui.loadFromFile(FileName.c_str());
	
	FileName = "Char_2.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_Character[2/**/]->gui.loadFromFile(FileName.c_str());
	
	/********************
	********************/
	FileName = "Stage.xml";
	if(checkif_FileExist(FileName.c_str())) Gui_Stage->gui.loadFromFile(FileName.c_str());
}

/******************************
******************************/
bool ofApp::checkif_FileExist(const char* FileName)
{
	if(ofFile::doesFileExist(FileName)){
		printf("exist:%s\n", FileName);
		return true;
	}else{
		return false;
	}
}

/******************************
******************************/
void ofApp::remove_xml()
{
	ofFile::removeFile("FFT_0.xml");
	ofFile::removeFile("FFT_1.xml");
	
	ofFile::removeFile("Char_0.xml");
	ofFile::removeFile("Char_1.xml");
	ofFile::removeFile("Char_2.xml");
	
	ofFile::removeFile("Stage.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case '0':
		case '1':
		case '2':
		case '3':
		{
			int GuiId = key - '0';
			b_DispGui[GuiId] = !b_DispGui[GuiId];
		}
			break;
			
		case 'a':
			b_fullScreen = !b_fullScreen;
			ofSetFullscreen(b_fullScreen);
			if(!b_fullScreen){
				ofSetWindowShape( WIDTH, HEIGHT );
			}
			break;
				
		case ' ':
			// change stage.
			StageManager.setEvent(MusicTime_ms, STAGE_MANAGER::EVENT__CHANGE_STAGE);
			break;
			
		case 'd':
			// remove_xml();
			break;
			
		case 'f':
			// change structure
			StageManager.setEvent(MusicTime_ms, STAGE_MANAGER::EVENT__CHANGE_STRUCTURE);
			break;
			
		case 'g':
			break;
			
		case 'j':
			// change stage immediately.
			StageManager.setEvent(MusicTime_ms, STAGE_MANAGER::EVENT__CHANGE_STAGE_IMMEDIATE);
			break;
			
		case 'l':
			Guis_LoadSetting();
			break;
			
		case 's':
			ofSaveScreen("image.png");
			printf("image saved\n");
			break;
			
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
