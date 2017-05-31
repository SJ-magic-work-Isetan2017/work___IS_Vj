/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "thread_Base.h"

/************************************************************
class
************************************************************/

/**************************************************
**************************************************/
struct TIME_N_DATASET__COLOR{
	int time_ms;
	
	int R;
	int G;
	int B;
	
	int t_interval_ColorChange;
};

/**************************************************
Singleton
	https://ja.wikipedia.org/wiki/Singleton_%E3%83%91%E3%82%BF%E3%83%BC%E3%83%B3
**************************************************/
class THREAD__COLOR_TIMETABLE : public THREAD_BASE_STEPOVER
{
private:
	/****************************************
	****************************************/
	TIME_N_DATASET__COLOR *TimeTable[NUM_BUFFERS]; 
	TIME_N_DATASET__COLOR data_to_output;
	TIME_N_DATASET__COLOR data_to_charge;
	
	/****************************************
	function
	****************************************/
	/********************
	singleton
	********************/
	THREAD__COLOR_TIMETABLE();
	~THREAD__COLOR_TIMETABLE();
	THREAD__COLOR_TIMETABLE(const THREAD__COLOR_TIMETABLE&); // Copy constructor. no define.
	THREAD__COLOR_TIMETABLE& operator=(const THREAD__COLOR_TIMETABLE&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void set_LogFile_id();
	
	FILE* open_ScenarioFile();
	void SetTime_DataToCharge(int time);
	void chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id);
	void SetData_DataToCharge(FILE* fp);
	void set_outputData(int BufferId, int id);
	void set_outputData_None();
	int get_TimeData_from_TimeTable(int BufferId, int id);
	void print_ScenarioFileName();
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static THREAD__COLOR_TIMETABLE* getInstance(){
		static THREAD__COLOR_TIMETABLE inst;
		return &inst;
	}
	
	int checkEvent(TIME_N_DATASET__COLOR& _dataset);
};

