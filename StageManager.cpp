/************************************************************
************************************************************/
#include "StageManager.h"

/************************************************************
************************************************************/

/******************************
******************************/
STAGE_MANAGER::STAGE_MANAGER()
: CurrentStructureType(STAGE_STRUCTURE::STRUCTURE_TYPE__LAYER)
, t_CamParam_ChangeFrom(0)
, t_CamParam_Duration(1.0)
, Event(EVENT__NONE)
, fp_Scenario(NULL)
, StructureSurfaceType(STRUCTURE_SURFACE_STATIC)
, LastMusicTime(-1)
{
	if(BootMode == BOOT_MODE__AUTOPLAY){
		thread_TimeTable = THREAD__EVENT_TIMETABLE::getInstance();
	}
}

/******************************
******************************/
STAGE_MANAGER::~STAGE_MANAGER()
{
	if(fp_Scenario) fclose(fp_Scenario);
}

/******************************
******************************/
void STAGE_MANAGER::exit()
{
	if(BootMode == BOOT_MODE__AUTOPLAY){
		thread_TimeTable->exit();
		try{
			/********************
			stop済みのthreadをさらにstopすると、Errorが出るようだ。
			********************/
			if(thread_TimeTable->isThreadRunning()){
				thread_TimeTable->stopThread();
				while(thread_TimeTable->isThreadRunning()){
					thread_TimeTable->waitForThread(true);
				}
			}
			
		}catch(...){
			printf("Thread exiting Error\n");
		}
	}
}

/******************************
******************************/
void STAGE_MANAGER::setup()
{
	if(BootMode == BOOT_MODE__REC){
		fp_Scenario = fopen("../../../vj(Rec).txt", "w");
	}else{
		thread_TimeTable->setup();
		thread_TimeTable->startThread(true, false); // blocking, non verboss
	}
	
	StageStructure[0].setup(STAGE_STRUCTURE::STRUCTURE_TYPE__LAYER);
	StageStructure[1].setup(STAGE_STRUCTURE::STRUCTURE_TYPE__SURFACE);
	StageStructure[2].setup(STAGE_STRUCTURE::STRUCTURE_TYPE__CUBE);
	
	start_CurrentStructure();
}

/******************************
******************************/
void STAGE_MANAGER::start_CurrentStructure()
{
	StageStructure[CurrentStructureType].start(CamParam_Current);
	
	CamParam_From = CamParam_Current;
	CamParam_To = CamParam_Current;
}

/******************************
******************************/
void STAGE_MANAGER::CurrentStructure_changeStage()
{
	StageStructure[CurrentStructureType].changeStage(CamParam_To);
	t_CamParam_ChangeFrom = ElapsedTime;
}

/******************************
******************************/
void STAGE_MANAGER::Dice_Structure()
{
	const int MAX_TRY = 20;
	STAGE_STRUCTURE::STRUCTURE_TYPE temp;
	for(int i = 0; i < MAX_TRY; i++){
		temp = (STAGE_STRUCTURE::STRUCTURE_TYPE)(rand() % NUM_STRUCTURES);
		if(temp != CurrentStructureType){
			CurrentStructureType = temp;
			
			if(CurrentStructureType == STRUCTURE_SURFACE){
				StructureSurfaceType = (STRUCTURE_SURFACE_TYPE)(rand() % NUM_STRUCTURE_SURFACE_TYPES);
			}
			return;
		}
	}
	
	CurrentStructureType = temp;
}

/******************************
******************************/
void STAGE_MANAGER::setEvent(int MusicTime_ms, EVENT _event)
{
	/********************
	********************/
	if(BootMode == BOOT_MODE__REC){
		Event = _event;
		fprintf(fp_Scenario, "<time_ms> %10d %d\n", MusicTime_ms, Event);
	}
}

/******************************
******************************/
void STAGE_MANAGER::update(int MusicTime_ms, float fftGain)
{
	/********************
	********************/
	if( IsMusicLoop(MusicTime_ms) && (BootMode == BOOT_MODE__AUTOPLAY) ){
		thread_TimeTable->stopThread();
		thread_TimeTable->waitForThread(true);
		thread_TimeTable->Reset();
		thread_TimeTable->startThread(true, false);
	}
	
	/********************
	********************/
	if(Timer_ContentsChange.IsBell(ElapsedTime)){
		for(int i = 0; i < NUM_STRUCTURES; i++){
			StageStructure[i].changeContents();
		}
	}
	
	/********************
	********************/
	if(BootMode == BOOT_MODE__AUTOPLAY){
		if(thread_TimeTable->IsReady()){
			/********************
			もしNotReadyの時は、Skipしてしまうが、すぐ処理終わるので、多分そんなcaseはない.
			IsReady()をLoopしてここで粘って待ってしまうと、IsReady()の向こう側でlock()しているので、高速にlockが回ってしまい、
			threadedFunctionに処理が回らなくなる
			その結果、フリーズするので、注意.
			********************/
			checkEvent_TimeTableScenario(MusicTime_ms);
		}
	}
	
	if(Event == EVENT__CHANGE_STRUCTURE){
		StageStructure[CurrentStructureType].stop();
		Dice_Structure();
		start_CurrentStructure();
		
	}else if(Event == EVENT__CHANGE_STAGE){
		CamParam_From = CamParam_Current;
		CurrentStructure_changeStage();
		
		/********************
		"STRUCTURE_SURFACE_CAMPOS_FAST"の時、最後の所で"クッ"とした動きが気になったので、
		cam angleもanimationすることで、柔らかい動きにしてみた.
		********************/
		if( (CurrentStructureType == STAGE_STRUCTURE::STRUCTURE_TYPE__SURFACE) && (StructureSurfaceType == STRUCTURE_SURFACE_CAMPOS_FAST) ){
			CamParam_From.CamAngle = CamParam_To.CamAngle * 2.5;
		}
		
	}else if(Event == EVENT__CHANGE_STAGE_IMMEDIATE){
		CurrentStructure_changeStage();
		CamParam_From = CamParam_To;
	}
	
	Event = EVENT__NONE;
	
	/********************
	********************/
	StageStructure[CurrentStructureType].update(fftGain);
	
	/********************
	********************/
	LastMusicTime = MusicTime_ms;
}

/******************************
******************************/
bool STAGE_MANAGER::IsMusicLoop(int MusicTime_ms)
{
	if(MusicTime_ms < LastMusicTime)	return true;
	else								return false;
}

/******************************
******************************/
bool STAGE_MANAGER::checkEvent_TimeTableScenario(int MusicTime_ms)
{
	if(BootMode == BOOT_MODE__AUTOPLAY){
		thread_TimeTable->update(MusicTime_ms);
		Event = EVENT( ((THREAD__EVENT_TIMETABLE*)thread_TimeTable)->checkEvent());
	}
}

/******************************
******************************/
void STAGE_MANAGER::cal_CamParamCurrent()
{
	float dt = ElapsedTime - t_CamParam_ChangeFrom;
	
	if(dt < t_CamParam_Duration){
		/********************
		********************/
		if( (CurrentStructureType == STAGE_STRUCTURE::STRUCTURE_TYPE__SURFACE) && (StructureSurfaceType == STRUCTURE_SURFACE_CAMPOS_FAST) ){
			CamParam_Current.CamPosition = CamParam_To.CamPosition; // すぐ移動完了.
		}else{
			CamParam_Current.CamPosition = CamParam_From.CamPosition + (CamParam_To.CamPosition - CamParam_From.CamPosition) * dt/t_CamParam_Duration;
		}
		
		/********************
		********************/
		if( (CurrentStructureType == STAGE_STRUCTURE::STRUCTURE_TYPE__SURFACE) && (StructureSurfaceType == STRUCTURE_SURFACE_TARGET_FAST) ){
			CamParam_Current.CamLookAt = CamParam_To.CamLookAt; // すぐ移動完了.
		}else{
			CamParam_Current.CamLookAt = CamParam_From.CamLookAt + (CamParam_To.CamLookAt - CamParam_From.CamLookAt) * dt/t_CamParam_Duration;
		}
		
		/********************
		********************/
		CamParam_Current.CamAngle = CamParam_From.CamAngle + (CamParam_To.CamAngle - CamParam_From.CamAngle) * dt/t_CamParam_Duration;
		
		/********************
		********************/
		if(CamParam_From.CamUpper == CamParam_To.CamUpper){
			CamParam_Current.CamUpper = CamParam_To.CamUpper;
		}else{
			float d_deg = CamParam_From.CamUpper.angle(CamParam_To.CamUpper)/t_CamParam_Duration;
			ofVec3f vec_along = CamParam_From.CamUpper.getCrossed(CamParam_To.CamUpper);
			CamParam_Current.CamUpper = CamParam_From.CamUpper.rotate(d_deg * dt, vec_along);
		}
		
	}else{
		CamParam_Current = CamParam_To;
	}
}

/******************************
******************************/
void STAGE_MANAGER::draw()
{
	/*
	EasyCam.begin();
	StageStructure[CurrentStructureType].draw();
	EasyCam.end();
	return;
	*/
	
	/********************
	********************/
	cal_CamParamCurrent();
	
	/********************
	********************/
	cam.setPosition(CamParam_Current.CamPosition);
	cam.lookAt( CamParam_Current.CamLookAt, CamParam_Current.CamUpper);
	cam.setFov(CamParam_Current.CamAngle);
	
	cam.setNearClip(1);
	cam.setFarClip(3000);
	
	/********************
	********************/
	cam.begin();
	StageStructure[CurrentStructureType].draw();
	cam.end();
}
