/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "StageStructure.h"
#include "th_Event.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class STAGE_MANAGER{
public:
	/****************************************
	****************************************/
	enum EVENT{
		EVENT__NONE,
		EVENT__CHANGE_STRUCTURE,
		EVENT__CHANGE_STAGE,
		EVENT__CHANGE_STAGE_IMMEDIATE,
		
		NUM_EVENTS,
	};
	
	enum{
		STRUCTURE_LAYER,
		STRUCTURE_SURFACE,
		STRUCTURE_CUBE,
		
		NUM_STRUCTURES,
	};
	
	enum STRUCTURE_SURFACE_TYPE{
		STRUCTURE_SURFACE_STATIC,
		STRUCTURE_SURFACE_TARGET_FAST,
		STRUCTURE_SURFACE_CAMPOS_FAST,
		
		NUM_STRUCTURE_SURFACE_TYPES,
	};
	
private:
	/****************************************
	****************************************/
	struct INTERVAL_TIMER{
		const float t_interval;
		int counter;
		
		INTERVAL_TIMER()
		: t_interval(9000 /* 2.5h */), counter(1)
		{
		}
		
		bool IsBell(float ElapsedTime){
			/********************
			一瞬、クッと止まってしまうのが嫌なのでナシにしよう。
			「毎日違うコンテンツ」でOKとする。
			********************/
			return false;
			
			/*
			bool ret = false;
			
			while(0 < ElapsedTime - t_interval * counter){
				counter++;
				ret = true;
			}
			return ret;
			*/
		}
		
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	STAGE_STRUCTURE::STRUCTURE_TYPE CurrentStructureType;
	STAGE_STRUCTURE StageStructure[NUM_STRUCTURES];
	
	ofEasyCam EasyCam;
	
	ofCamera cam;
	CAM_PARAM CamParam_Current;
	CAM_PARAM CamParam_From;
	CAM_PARAM CamParam_To;
	float t_CamParam_ChangeFrom;
	float t_CamParam_Duration;
	
	EVENT Event;
	
	STRUCTURE_SURFACE_TYPE StructureSurfaceType;
	
	int LastMusicTime;
	
	/********************
	********************/
	INTERVAL_TIMER Timer_ContentsChange;
	
	/********************
	********************/
	FILE* fp_Scenario;
	
	THREAD_BASE* thread_TimeTable;

	/****************************************
	****************************************/
	void start_CurrentStructure();
	void CurrentStructure_changeStage();
	void Dice_Structure();
	void cal_CamParamCurrent();
	
	bool IsMusicLoop(int MusicTime_ms);
	bool checkEvent_TimeTableScenario(int MusicTime_ms);
	
public:
	/****************************************
	****************************************/
	STAGE_MANAGER();
	~STAGE_MANAGER();
	
	void exit();
	void setup();
	void update(int MusicTime_ms, float fftGain);
	void draw();
	
	void setEvent(int MusicTime_ms, EVENT _event);
};

