/************************************************************
note
	Macのカレンダー.appを使って定期的にアプリケーションを実行してみよう
		http://gijutsubu.backstage.jp/?eid=87
		
	カレンダーの通知ウィンドウを表示させない.
		http://mirunews.jp/3582
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "ofxNetwork.h"

#include "sj_common.h"
#include "sj_Osc.h"
#include "UseFftResult.h"
#include "StageManager.h"

#include "test_char.h"
#include "test_Stage.h"

/************************************************************
************************************************************/
#define EXE__DEMO			0
#define EXE__DEBUG_CHAR		1
#define EXE__DEBUG_STAGE	2
	#define EXE_MODE EXE__DEMO

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	/********************
	********************/
	OSC_TARGET Osc_MusicPlayer;
	
	ofTrueTypeFont font;
	int MusicTime_ms;
	bool b_DispTime;
	
	bool b_DispGui[NUM_GUIS];
	
	bool b_fullScreen;
	bool b_Cursor;
	
	/********************
	********************/
	ofFbo fbo;
	
	/********************
	********************/
	USE_FFT_RESULT UseFftResult;
	
	TEST_CHARACTER test_Character;
	TEST_STAGE test_Stage;
	STAGE_MANAGER StageManager;
	
	/****************************************
	****************************************/
	void update_OscFrom_MusicPlayer();
	void draw_time();
	void drawGuis();
	void setup_Gui();
	void Guis_LoadSetting();
	bool checkif_FileExist(const char* FileName);
	void remove_xml();
	
public:
	/****************************************
	****************************************/
	ofApp(int _BootMode);
	~ofApp();
	
	void exit();
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
