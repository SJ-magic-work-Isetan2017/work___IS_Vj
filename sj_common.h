/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "stdio.h"

#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

enum{
	WIDTH = 1280,
	HEIGHT = 720,
};

enum{
	BUF_SIZE = 2000,
};

enum{
	AUDIO_BUF_SIZE = 512,
	
	AUDIO_BUFFERS = 2,
	AUDIO_SAMPLERATE = 44100,
};

enum{
	COMPRESS = 2,
};

enum{
	GUI_ID__FFT,
	GUI_ID__CHARACTER,
	GUI_ID__STAGE,
	
	GUI_ID__CAM,
	GUI_ID__PARTICLE,
	
	NUM_GUIS,
};

enum{
	THREAD_TIMETABLE__EVENT,
	THREAD_TIMETABLE__COLOR,
	
	NUM_THREAD_TIMETABLE,
};

enum BOOT_MODE{
	BOOT_MODE__AUTOPLAY,
	BOOT_MODE__REC,
};

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_FFT{
private:
	static int id;
	bool b_IsSet;
	
public:
	/****************************************
	****************************************/
	GUI_FFT():b_IsSet(false)
	{
	}
	
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	/* */
	ofxGuiGroup GuiGroup_Graph_Disp;
	
	ofxFloatSlider GraphRadius;
	ofxFloatSlider BarHeight;
	ofxFloatSlider BarWidth_Bottom;
	ofxFloatSlider BarWidth_Top;
	
	ofxToggle b_GainAdjust;
	ofxFloatSlider AudioSample_Amp;
	
	ofxFloatSlider Rotaion_deg;
	
	ofxVec2Slider pos;
	
	
	/* */
	ofxColorSlider BarColor;
};

/**************************************************
**************************************************/
class GUI_CHARACTER{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	ofxFloatSlider vert_NoiseAmp;
	ofxFloatSlider vert_NoiseFreq;
	ofxToggle b_vertNoise_fftSync;
	
	ofxFloatSlider center_NoiseAmp;
	ofxFloatSlider center_NoiseFreq;
	
	ofxFloatSlider BaseScale;
	ofxFloatSlider fftGain_Range;
	
	ofxFloatSlider Alpha;
};

/**************************************************
**************************************************/
class GUI_STAGE{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	ofxFloatSlider fftGain_Range;
	ofxColorSlider BackImageColor;
};

/**************************************************
**************************************************/
class GUI_CAM{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	ofxVec3Slider CamPosition;
	ofxVec3Slider CamLookAt;
	ofxVec3Slider CamUpper;
	ofxFloatSlider CamAngle;
};

/**************************************************
**************************************************/
class GUI_PARTICLE{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	ofxFloatSlider ParticleSize;
	ofxFloatSlider Particle_friction_DownPer_sec;
	ofxFloatSlider Particle_forceScale;
	// ofxVec4Slider ParticleColor;
	ofxFloatSlider ParticleSpeedThresh;
	
	ofxVec4Slider ParticleColor;
};

/************************************************************
************************************************************/
extern void fopen_LogFile();
extern void fclose_LogFile();
extern void fprint_debug_Log(char* message, int FileId);

/************************************************************
************************************************************/
extern float ElapsedTime;

extern BOOT_MODE BootMode;

extern GUI_FFT *Gui_FFT[2];
extern GUI_CHARACTER *Gui_Character[3];
extern GUI_STAGE* Gui_Stage;
extern GUI_CAM* Gui_Cam;
extern GUI_PARTICLE* Gui_Particle;

