/************************************************************
************************************************************/
#include "StageStructure.h"
#include "StageStructure_Param.h"

/************************************************************
************************************************************/

/******************************
******************************/
STAGE_STRUCTURE::STAGE_STRUCTURE()
{
}

/******************************
******************************/
STAGE_STRUCTURE::~STAGE_STRUCTURE()
{
}

/******************************
******************************/
void STAGE_STRUCTURE::setup(STRUCTURE_TYPE type)
{
	StructureType = type;
	
	switch(StructureType){
		case STRUCTURE_TYPE__LAYER:
			NUM_STAGES = 6;
			Stage_Cam_Pose = Stage_Cam_Pose__Layer;
			break;
			
		case STRUCTURE_TYPE__SURFACE:
			NUM_STAGES = 9;
			Stage_Cam_Pose = Stage_Cam_Pose__Surface;
			break;
			
		case STRUCTURE_TYPE__CUBE:
			NUM_STAGES = 6;
			Stage_Cam_Pose = Stage_Cam_Pose__Cube;
			break;
	}
	
	for(int i = 0; i < NUM_STAGES; i++){
		stage[i] = new STAGE(Stage_Cam_Pose[i].pos, Stage_Cam_Pose[i].Rotate_vec, Stage_Cam_Pose[i].Rotate_deg);
	}

}

/******************************
******************************/
void STAGE_STRUCTURE::start(CAM_PARAM& CamParam)
{
	StageId = 0;
	CamParam = Stage_Cam_Pose[StageId].CamParam;
}

/******************************
******************************/
void STAGE_STRUCTURE::stop()
{
	changeEffect();
}

/******************************
******************************/
void STAGE_STRUCTURE::changeContents()
{
	for(int i = 0; i < NUM_STAGES; i++){
		delete stage[i];
		stage[i] = new STAGE(Stage_Cam_Pose[i].pos, Stage_Cam_Pose[i].Rotate_vec, Stage_Cam_Pose[i].Rotate_deg);
	}
}

/******************************
******************************/
void STAGE_STRUCTURE::changeEffect()
{
	for(int i = 0; i < NUM_STAGES; i++){
		stage[i]->changeEffect();
	}
}

/******************************
******************************/
void STAGE_STRUCTURE::update(float fftGain)
{
	for(int i = 0; i < NUM_STAGES; i++){
		stage[i]->update(fftGain);
	}
}

/******************************
******************************/
void STAGE_STRUCTURE::draw()
{
	/********************
	Z-buffering and blemding mode do not work together properly in a simple way.
	(oF Essentials. p98)
	
	参考URL
		https://www.sjbaker.org/steve/omniv/alpha_sorting.html
	********************/
	// z buffering
	ofDisableAlphaBlending();
	ofEnableDepthTest();
	
	for(int i = 0; i < NUM_STAGES; i++){
		stage[i]->draw();
	}
	
	ofDisableDepthTest();
}

/******************************
******************************/
void STAGE_STRUCTURE::getCamParam_CurrentStage(CAM_PARAM& dst)
{
	dst = Stage_Cam_Pose[StageId].CamParam;
}

/******************************
******************************/
bool STAGE_STRUCTURE::IsNextStage_OK(int NextId)
{
	if(NextId == StageId)	return false;
	
	switch(StructureType){
		case STRUCTURE_TYPE__LAYER:
			if(abs(NextId - StageId) < 3)	return true;
			else							return false;
			
		case STRUCTURE_TYPE__SURFACE:
			return true;
			
		case STRUCTURE_TYPE__CUBE:
			switch(StageId){
				case 0:
					if(NextId == 1)	return false;
					else			return true;
					
				case 1:
					if(NextId == 0)	return false;
					else			return true;

				case 2:
					if(NextId == 3)	return false;
					else			return true;

				case 3:
					if(NextId == 2)	return false;
					else			return true;

				case 4:
					if(NextId == 5)	return false;
					else			return true;

				case 5:
					if(NextId == 4)	return false;
					else			return true;

			}
			break;
	}
}

/******************************
******************************/
void STAGE_STRUCTURE::changeStage(CAM_PARAM& CamParam_To)
{
	const int MAX_TRY = 20;
	int temp;
	for(int i = 0; i < MAX_TRY; i++){
		temp = rand() % NUM_STAGES;
		if(IsNextStage_OK(temp)){
			StageId = temp;
			// printf("\n> change stage = %d\n", StageId);
			CamParam_To = Stage_Cam_Pose[StageId].CamParam;
			return;
		}
	}
	
	StageId = temp;
	CamParam_To = Stage_Cam_Pose[StageId].CamParam;
}
