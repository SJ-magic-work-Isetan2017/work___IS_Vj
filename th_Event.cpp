/************************************************************
************************************************************/
#include <unistd.h>
#include "th_Event.h"

/************************************************************
function
************************************************************/

/******************************
******************************/
THREAD__EVENT_TIMETABLE::THREAD__EVENT_TIMETABLE()
{
	for(int i = 0; i < NUM_BUFFERS; i++){
		TimeTable[i] = new TIME_N_DATASET__EVENT[NUM_SAMPLES_PER_BUFFER];
	}
}

/******************************
******************************/
THREAD__EVENT_TIMETABLE::~THREAD__EVENT_TIMETABLE()
{
	for(int i = 0; i < NUM_BUFFERS; i++){
		delete[] TimeTable[i];
	}
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::set_LogFile_id()
{
	LogFile_id = THREAD_TIMETABLE__EVENT;
}

/******************************
******************************/
FILE* THREAD__EVENT_TIMETABLE::open_ScenarioFile()
{
	FILE* fp = fopen("../../../vj.txt", "r");
	return fp;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::SetTime_DataToCharge(int time)
{
	data_to_charge.time_ms = time;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id)
{
	TimeTable[BufferId_toCharge][Charge_id] = data_to_charge;
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::SetData_DataToCharge(FILE* fp)
{
	char buf[BUF_SIZE];
	fscanf(fp, "%s", buf);
	
	data_to_charge.EventId = atoi(buf);
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::set_outputData(int BufferId, int id)
{
	data_to_output = TimeTable[BufferId][id];
}

/******************************
******************************/
void THREAD__EVENT_TIMETABLE::set_outputData_None()
{
	TIME_N_DATASET__EVENT temp = {0, 0/* EVENT__NONE(StageManager.h) */};
	data_to_output = temp;
}

/******************************
******************************/
int THREAD__EVENT_TIMETABLE::get_TimeData_from_TimeTable(int BufferId, int id)
{
	return TimeTable[BufferId][id].time_ms;
}

/******************************
******************************/
int THREAD__EVENT_TIMETABLE::checkEvent()
{
	return data_to_output.EventId;
}




