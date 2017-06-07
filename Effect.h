/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"

/************************************************************
************************************************************/
#define TEXTMASK_SMALL

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class EFFECT{
private:
	/****************************************
	****************************************/
	enum RANDOM_SEL_WEIGHT{
		WEIGHT__NONE = 3,
		WEIGHT__SPLIT,
		WEIGHT__KALEIDOSCOPE = WEIGHT__SPLIT + 2,
		WEIGHT__MIRROR_LR,
		WEIGHT__MIRROR_VERT,
		WEIGHT__TEXTMASK,
		
		TOTAL_WEIGHT,
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	ofShader shader_Split;
	ofShader shader_Kaleido;
	ofShader shader_Mirror_LR;
	ofShader shader_Mirror_Vert;
	ofShader shader_Mask;
	
	/********************
	KaleideScope
	********************/
	float k_angle;
	
	/********************
	Text Mask
	********************/
	ofTrueTypeFont font;
	
	/********************
	********************/
	float LastInt_sec;
	
	/****************************************
	****************************************/
	void draw_None(ofFbo* fbo_src, ofFbo* fbo_dst);
	void draw_Split(ofFbo* fbo_src, ofFbo* fbo_dst, int NumSplit);
	void draw_Kaleido(ofFbo* fbo_src, ofFbo* fbo_dst);
	void draw_Mirror_Vert(ofFbo* fbo_src, ofFbo* fbo_dst);
	void draw_Mirror_LR(ofFbo* fbo_src, ofFbo* fbo_dst);
	void draw_TextMask(ofFbo* fbo_src, ofFbo* fbo_dst);
	
	int Dice_index(int *Weight, int NUM);
	
public:
	/****************************************
	****************************************/
	enum EFFECT_TYPE{
		EFFECT_TYPE__NONE,
		
		EFFECT_TYPE__SPLIT_2x2,
		EFFECT_TYPE__SPLIT_3x3,
		EFFECT_TYPE__SPLIT_4x4,
		EFFECT_TYPE__SPLIT_5x5,
		EFFECT_TYPE__SPLIT_6x6,
		EFFECT_TYPE__SPLIT_8x8,
		EFFECT_TYPE__SPLIT_10x10,
		EFFECT_TYPE__SPLIT_20x20,
		EFFECT_TYPE__SPLIT_30x30,
		
		EFFECT_TYPE__KALEIDOSCOPE,
		
		EFFECT_TYPE__MIRROR_LR,
		EFFECT_TYPE__MIRROR_VERT,
		
		EFFECT_TYPE__TEXTMASK,
		
		NUM_EFFECT_TYPE,
	};
	
	/****************************************
	****************************************/
	EFFECT();
	~EFFECT();
	
	void setup();
	void update();
	void draw(EFFECT_TYPE EffectType, ofFbo* fbo_src, ofFbo* fbo_dst = NULL);
	
	EFFECT_TYPE get_RandomSelect_EffectType();
};

