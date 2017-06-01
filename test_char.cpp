/************************************************************
************************************************************/
#include "test_char.h"

/************************************************************
************************************************************/

/******************************
******************************/
TEST_CHARACTER::TEST_CHARACTER()
: NUM_CHARACTERS(3)
{
	for(int i = 0; i < NUM_CHARACTERS; i++){
		character[i] = new CHARACTER(NUM_CHARACTERS, i);
	}
}

/******************************
******************************/
TEST_CHARACTER::~TEST_CHARACTER()
{
	for(int i = 0; i < NUM_CHARACTERS; i++){
		delete character[i];
	}
}

/******************************
******************************/
void TEST_CHARACTER::setup()
{
	fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
}

/******************************
******************************/
void TEST_CHARACTER::update(float fftGain)
{
	for(int i = 0; i < NUM_CHARACTERS; i++){
		character[i]->update(fftGain);
	}
}

/******************************
******************************/
void TEST_CHARACTER::draw(float fftGain)
{
	fbo.begin();
	ofBackground(0, 0, 0, 0);
	for(int i = 0; i < NUM_CHARACTERS; i++){
		character[i]->draw(fftGain);
	}
	fbo.end();
	
	ofSetColor(255);
	fbo.draw(0, 0);
}
