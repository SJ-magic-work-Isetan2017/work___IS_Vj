/************************************************************
************************************************************/
#include "test_Stage.h"

/************************************************************
************************************************************/

/******************************
******************************/
TEST_STAGE::TEST_STAGE()
{
	stage = new STAGE(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0), 0);
}

/******************************
******************************/
TEST_STAGE::~TEST_STAGE()
{
	delete stage;
}

/******************************
******************************/
void TEST_STAGE::setup()
{
}

/******************************
******************************/
void TEST_STAGE::update(float fftGain)
{
	stage->update(fftGain);
}

/******************************
******************************/
void TEST_STAGE::draw()
{
	cam.setPosition(Gui_Cam->CamPosition);
	cam.lookAt( Gui_Cam->CamLookAt, Gui_Cam->CamUpper);
	cam.setFov(Gui_Cam->CamAngle);
	
	cam.setNearClip(1);
	cam.setFarClip(3000);
	
	cam.begin();
	stage->draw();
	cam.end();
}
