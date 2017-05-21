/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
printf("%f\n", FLT_MAX);
> 340282346638528859811704183484516925440.000000
で1ヶ月の展示には十分.
********************/
float ElapsedTime;

BOOT_MODE BootMode;

int GUI_FFT::id = 0;
GUI_FFT *Gui_FFT[2];
GUI_CHARACTER *Gui_Character[3];
GUI_STAGE* Gui_Stage;
GUI_CAM* Gui_Cam;

/************************************************************
************************************************************/
static FILE* fp_Log[NUM_THREAD_TIMETABLE];

/************************************************************
************************************************************/

/******************************
******************************/
void GUI_FFT::setup(string GuiName, string FileName, float x, float y)
{
	if(!b_IsSet){
		/********************
		********************/
		gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
		
		/********************
		********************/
		GuiGroup_Graph_Disp.setup("GraphDisp");
		
		GuiGroup_Graph_Disp.add(GraphRadius.setup("Radius", 131, 50, 500));
		GuiGroup_Graph_Disp.add(BarHeight.setup("BarHeight", 90, 50, 300));
		GuiGroup_Graph_Disp.add(BarWidth_Bottom.setup("BarWidth Btm", 15, 1, 40));
		GuiGroup_Graph_Disp.add(BarWidth_Top.setup("BarWidth Top", 1.5, 0, 40));
		GuiGroup_Graph_Disp.add(b_GainAdjust.setup("b_GainAdjust", false));
		GuiGroup_Graph_Disp.add(AudioSample_Amp.setup("AudioAmp", 0.06, 0, 0.2));
		GuiGroup_Graph_Disp.add(Rotaion_deg.setup("Rotaion_deg", 0, 0, 360));
		
		{
			const float ofs = 20;
			ofVec2f initPos = ofVec2f(ofs * id, 0.0); // 2つのFFT circleが重なっていることを命じ的に示すため、Guiの初期値をずらす.
			ofVec2f minPos = ofVec2f(-WIDTH, -HEIGHT);
			ofVec2f maxPos = ofVec2f(WIDTH, HEIGHT);
			
			GuiGroup_Graph_Disp.add(pos.setup("pos", initPos, minPos, maxPos));
		}
		
		gui.add(&GuiGroup_Graph_Disp);
		
		/********************
		********************/
		{
			ofColor initColor = ofColor(255, 255, 255, 200);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			gui.add(BarColor.setup("BarColor", initColor, minColor, maxColor));
		}
		
		/********************
		********************/
		id++;
		b_IsSet = true;
	}
}

/******************************
******************************/
void GUI_CHARACTER::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	gui.add(vert_NoiseAmp.setup("vert_NoiseAmp", 100, 0, 400));
	gui.add(vert_NoiseFreq.setup("vert_NoiseFreq", 1, 0.5, 10));
	gui.add(b_vertNoise_fftSync.setup("vert fft sync", true));
	
	gui.add(center_NoiseAmp.setup("center_NoiseAmp", 100, 0, 400));
	gui.add(center_NoiseFreq.setup("center_NoiseFreq", 1, 0.5, 10));
	
	gui.add(BaseScale.setup("BaseScale", 1, 0.1, 2));
	gui.add(fftGain_Range.setup("fftGain_Range", 0.06, 0, 0.2));
	
	gui.add(Alpha.setup("Alpha", 0.98, 0, 1.0));
}

/******************************
******************************/
void GUI_STAGE::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	{
		ofColor initColor = ofColor(255, 255, 255, 255);
		ofColor minColor = ofColor(0, 0, 0, 0);
		ofColor maxColor = ofColor(255, 255, 255, 255);
		gui.add(BackImageColor.setup("BackImageColor", initColor, minColor, maxColor));
	}
	gui.add(fftGain_Range.setup("fftGain_Range", 0.06, 0, 0.2));
}

/******************************
******************************/
void GUI_CAM::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	{
		float _pos_max = 3000;
		
		ofVec3f PosMax = ofVec3f(_pos_max, _pos_max, _pos_max);
		ofVec3f PosMin = ofVec3f(-_pos_max, -_pos_max, -_pos_max);
		ofVec3f PosInit = ofVec3f(0, 0, 623);

		gui.add(CamPosition.setup("pos", PosInit, PosMin, PosMax));
	}
	{
		float _pos_max = 3000;
		
		ofVec3f PosMax = ofVec3f(_pos_max, _pos_max, _pos_max);
		ofVec3f PosMin = ofVec3f(-_pos_max, -_pos_max, -_pos_max);
		ofVec3f PosInit = ofVec3f(0, 0, 0);

		gui.add(CamLookAt.setup("LookAt", PosInit, PosMin, PosMax));
	}
	{
		ofVec3f DirMax = ofVec3f(1, 1, 1);
		ofVec3f DirMin = ofVec3f(-1, -1, -1);
		ofVec3f DirInit = ofVec3f(0, 1, 0);

		gui.add(CamUpper.setup("Upper", DirInit, DirMin, DirMax));
	}
	{
		gui.add(CamAngle.setup("angle", 60, 10, 120));
	}
}

/******************************
******************************/
void fopen_LogFile()
{
	for(int i = 0; i < NUM_THREAD_TIMETABLE; i++){
		char FileName[BUF_SIZE];
		sprintf(FileName, "../../../data/Log_%d.csv", i);
		
		fp_Log[i] = fopen(FileName, "w");
		if(fp_Log[i] == NULL)	{ ERROR_MSG(); std::exit(1); }
	}
}

/******************************
******************************/
void fclose_LogFile()
{
	for(int i = 0; i < NUM_THREAD_TIMETABLE; i++){
		if(fp_Log[i]) fclose(fp_Log[i]);
	}
}

/******************************
******************************/
void fprint_debug_Log(char* message, int FileId)
{
/*
	if(NUM_THREAD_TIMETABLE <= FileId)	{ ERROR_MSG(); std::exit(1); }
	
	
	if(FileId == -1){
		for(int i = 0; i < NUM_THREAD_TIMETABLE; i++){
			fprintf(fp_Log[i], "%s", message);
		}
	}else{
		fprintf(fp_Log[FileId], "%s", message);
	}
*/
}


