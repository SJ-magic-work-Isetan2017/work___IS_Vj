/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "character.h"
#include "Effect.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class STAGE{
private:
	/****************************************
	****************************************/
	enum{
		MAX_NUM_CHARACTERS = 3,
	};
	
	/****************************************
	****************************************/
	/********************
	Back image:File table
	********************/
	static char path[BUF_SIZE];
	static vector<string> FileTable;
	static int FileId;
	static bool b_FileTable_Ready;
	static EFFECT* EffectStage;
	
	/********************
	********************/
	ofImage img;
	const ofVec3f pos;
	const ofVec3f Rotate_vec;
	const float Rotate_deg;
	
	int NUM_CHARACTERS;
	CHARACTER* character[MAX_NUM_CHARACTERS];
	
	ofFbo fbo;
	ofFbo fbo_char_src;
	ofFbo fbo_char_dst;
	
	/********************
	********************/
	EFFECT::EFFECT_TYPE EffectType;
	
	/****************************************
	****************************************/
	/********************
	Front image:File table.
	********************/
	static void prepare_FileTable();
	static void ReadConfig();
	static void makeup_mov_table(const string dirname, vector<string>& FileTable);
	static void shuffle_Table(vector<string>& FileTable);
	static void FileId_toNext();
	
	/********************
	********************/
	static void prepare_EffectStage();
	
	/********************
	********************/
	void setup();
	
public:
	/****************************************
	****************************************/
	STAGE(ofVec3f _pos, ofVec3f _Rotate_vec, float _Rotate_deg);
	~STAGE();
	
	void update(float fftGain);
	void draw();
	
	void changeEffect();
};

