/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class THREAD_BASE : public ofThread{
protected:
	/****************************************
	****************************************/
	enum{
		NUM_BUFFERS = 2,
	};
	
	/****************************************
	****************************************/
	bool b_Empty[NUM_BUFFERS];
	
	int BufferId;
	
	FILE* fp;
	bool b_End;
	bool b_EOF;
	
	bool b_valid;
	
	/********************
	********************/
	int LogFile_id;
	
	
	/****************************************
	****************************************/
	/********************
	********************/
	void threadedFunction();
	bool Wait_NextBufferFilled(double timeout);
	int get_NextBufferId();
	
	/********************
	********************/
	virtual void set_LogFile_id() = 0;
	virtual void charge(int BufferId_toCharge) = 0;
	

public:	
	/****************************************
	****************************************/
	/********************
	********************/
	THREAD_BASE();
	virtual ~THREAD_BASE();
	
	bool IsReady();
	
	/********************
	********************/
	virtual void Reset();
	
	/********************
	********************/
	virtual void exit() = 0;
	virtual void setup() = 0;
	virtual void update(int now_ms) = 0;
};

/**************************************************
**************************************************/
class THREAD_BASE_STEPOVER : public THREAD_BASE{
protected:
	/****************************************
	enum
	****************************************/
	enum{
		NUM_SAMPLES_PER_BUFFER = 1000,
	};
	
	/****************************************
	****************************************/
	bool b_send;
	int id;
	
	float LastINT_sec;
	
	/****************************************
	function
	****************************************/
	
	/********************
	********************/
	void charge(int BufferId_toCharge);
	
	/********************
	********************/
	virtual FILE* open_ScenarioFile() = 0;
	virtual void SetTime_DataToCharge(int time) = 0;
	virtual void SetData_DataToCharge(FILE* fp) = 0;
	virtual void chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id) = 0;
	virtual void set_outputData(int BufferId, int id) = 0;
	virtual void set_outputData_None() = 0;
	virtual int get_TimeData_from_TimeTable(int BufferId, int id) = 0;
	
public:
	/****************************************
	****************************************/
	THREAD_BASE_STEPOVER();
	~THREAD_BASE_STEPOVER();
	
	/********************
	********************/
	void Reset();
	
	/********************
	********************/
	void exit();
	void setup();
	void update(int now_ms);
};



