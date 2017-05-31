/************************************************************
************************************************************/
#include <unistd.h>
#include "th_Color.h"

/************************************************************
function
************************************************************/

/******************************
******************************/
THREAD__COLOR_TIMETABLE::THREAD__COLOR_TIMETABLE()
{
	for(int i = 0; i < NUM_BUFFERS; i++){
		TimeTable[i] = new TIME_N_DATASET__COLOR[NUM_SAMPLES_PER_BUFFER];
	}
}

/******************************
******************************/
THREAD__COLOR_TIMETABLE::~THREAD__COLOR_TIMETABLE()
{
	for(int i = 0; i < NUM_BUFFERS; i++){
		delete[] TimeTable[i];
	}
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::set_LogFile_id()
{
	LogFile_id = THREAD_TIMETABLE__COLOR;
}

/******************************
******************************/
FILE* THREAD__COLOR_TIMETABLE::open_ScenarioFile()
{
	FILE* fp = fopen("../../../Dmx_Color.txt", "r");
	return fp;
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::print_ScenarioFileName()
{
	printf("Dmx_Color.txt not found\n\n");
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::SetTime_DataToCharge(int time)
{
	data_to_charge.time_ms = time;
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::chargeTimeTable_byCopying(int BufferId_toCharge, int Charge_id)
{
	TimeTable[BufferId_toCharge][Charge_id] = data_to_charge;
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::SetData_DataToCharge(FILE* fp)
{
	char buf[BUF_SIZE];
	
	fscanf(fp, "%s", buf);
	data_to_charge.R = atoi(buf);

	fscanf(fp, "%s", buf);
	data_to_charge.G = atoi(buf);

	fscanf(fp, "%s", buf);
	data_to_charge.B = atoi(buf);

	fscanf(fp, "%s", buf);
	data_to_charge.t_interval_ColorChange = atoi(buf);
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::set_outputData(int BufferId, int id)
{
	data_to_output = TimeTable[BufferId][id];
}

/******************************
******************************/
void THREAD__COLOR_TIMETABLE::set_outputData_None()
{
	TIME_N_DATASET__COLOR temp = {-1, -1, -1, -1, -1};
	data_to_output = temp;
}

/******************************
******************************/
int THREAD__COLOR_TIMETABLE::get_TimeData_from_TimeTable(int BufferId, int id)
{
	return TimeTable[BufferId][id].time_ms;
}

/******************************
******************************/
int THREAD__COLOR_TIMETABLE::checkEvent(TIME_N_DATASET__COLOR& _dataset)
{
	_dataset = data_to_output;
	return 0;
}




