/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "Stage.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
struct CAM_PARAM{
	ofVec3f CamPosition;
	ofVec3f CamLookAt;
	ofVec3f CamUpper;
	float CamAngle;
};

struct STAGE_CAM_POSE{
	ofVec3f pos;
	ofVec3f Rotate_vec;
	float Rotate_deg;
	
	CAM_PARAM CamParam;
};

/**************************************************
**************************************************/
class STAGE_STRUCTURE{
public:
	/****************************************
	****************************************/
	enum STRUCTURE_TYPE{
		STRUCTURE_TYPE__LAYER,
		STRUCTURE_TYPE__SURFACE,
		STRUCTURE_TYPE__CUBE,
	};
	
private:
	/****************************************
	****************************************/
	enum{
		MAX_STAGES = 9,
	};
	
	/****************************************
	****************************************/
	STRUCTURE_TYPE StructureType;
	int NUM_STAGES;
	int StageId;
	
	STAGE_CAM_POSE* Stage_Cam_Pose;
	STAGE* stage[MAX_STAGES];

	/****************************************
	****************************************/
	void changeEffect();
	bool IsNextStage_OK(int NextId);

public:
	
	/****************************************
	****************************************/
	STAGE_STRUCTURE();
	~STAGE_STRUCTURE();
	
	void setup(STRUCTURE_TYPE type);
	void update(float fftGain);
	void draw();
	
	void start(CAM_PARAM& CamParam);
	void stop();
	void getCamParam_CurrentStage(CAM_PARAM& dst);
	void changeStage(CAM_PARAM& CamParam_To);
	void changeContents();
};

