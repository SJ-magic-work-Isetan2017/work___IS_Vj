/************************************************************
************************************************************/
#include "Effect.h"

/************************************************************
************************************************************/

/******************************
******************************/
EFFECT::EFFECT()
: LastInt_sec(0)
, k_angle(0)
{
}

/******************************
******************************/
EFFECT::~EFFECT()
{
}
	
/******************************
******************************/
void EFFECT::setup()
{
#ifdef TEXTMASK_SMALL
	font.loadFont("Glamor-Bold.ttf", 20);
#else
	font.loadFont("FTY DELIRIUM NCV.ttf", 650);
#endif

	shader_Split.load( "movEffect/Split/Split.vert", "movEffect/Split/Split.frag");
	shader_Kaleido.load( "movEffect/Kaleido/kaleido.vert", "movEffect/Kaleido/kaleido.frag");
	shader_Mirror_LR.load( "movEffect/Mirror_LR/Mirror_LR.vert", "movEffect/Mirror_LR/Mirror_LR.frag");
	shader_Mirror_Vert.load( "movEffect/Mirror_Vertical/Mirror_Vertical.vert", "movEffect/Mirror_Vertical/Mirror_Vertical.frag");
	shader_Mask.load( "movEffect/Mask/Mask.vert", "movEffect/Mask/Mask.frag");
}

/******************************
******************************/
void EFFECT::update()
{
}

/******************************
******************************/
void EFFECT::draw(EFFECT_TYPE EffectType, ofFbo* fbo_src, ofFbo* fbo_dst)
{
	/********************
	********************/
	ofPushStyle();
	
	/********************
	********************/
	switch(EffectType){
		case EFFECT_TYPE__NONE:
			draw_None(fbo_src, fbo_dst);
			break;
			
		case EFFECT_TYPE__SPLIT_2x2:
			draw_Split(fbo_src, fbo_dst, 2);
			break;
			
		case EFFECT_TYPE__SPLIT_3x3:
			draw_Split(fbo_src, fbo_dst, 3);
			break;
			
		case EFFECT_TYPE__SPLIT_5x5:
			draw_Split(fbo_src, fbo_dst, 5);
			break;
			
		case EFFECT_TYPE__KALEIDOSCOPE:
			draw_Kaleido(fbo_src, fbo_dst);
			break;
			
		case EFFECT_TYPE__MIRROR_LR:
			draw_Mirror_LR(fbo_src, fbo_dst);
			break;
			
		case EFFECT_TYPE__MIRROR_VERT:
			draw_Mirror_Vert(fbo_src, fbo_dst);
			break;
			
		case EFFECT_TYPE__TEXTMASK:
			draw_TextMask(fbo_src, fbo_dst);
			break;
	}
	
	/********************
	********************/
	ofPopStyle();
}

/******************************
******************************/
void EFFECT::draw_TextMask(ofFbo* fbo_src, ofFbo* fbo_dst)
{
	/********************
	draw to local fbo that has same size of fbo_src.
	********************/
	/* */
	ofFbo fbo_Text;
	fbo_Text.allocate(fbo_src->getWidth(), fbo_src->getHeight(), GL_RGBA);
	fbo_Text.begin();
		ofBackground(0, 0, 0, 0);
		// ofSetColor(255, 255, 255, 255);
		ofSetColor(255);
		ofDisableAlphaBlending();
		
#ifdef TEXTMASK_SMALL
		const float ofs_x = 10;
		const float ofs_y = 10;
		int counter = 0;
		string text = "ISETAN JAPAN SENSES";
		for(float y = 0; y < HEIGHT; y+=font.stringHeight(text) + ofs_y){
			for(float x = counter * (-50); x < WIDTH; x+=font.stringWidth(text) + ofs_x){
				font.drawString(text, x, y);
			}
			counter+=1;
		}
#else		

		string text = "ISETAN";
		float offset_x = font.stringWidth(text) / 2;
		float offset_y = font.stringHeight(text) / 2;
		font.drawString(text, fbo_Text.getWidth()/2 - offset_x, fbo_Text.getHeight()/2 + offset_y);

#endif

	fbo_Text.end();
	
	/* */
	ofFbo fbo_dst_local;
	fbo_dst_local.allocate(fbo_src->getWidth(), fbo_src->getHeight(), GL_RGBA);
	fbo_dst_local.begin();
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		/* // comment out this line to check "Text size".
		ofSetColor(100);
		fbo_Text.draw(0, 0);
		/*/
	shader_Mask.begin();
		shader_Mask.setUniformTexture( "texture1", fbo_Text.getTextureReference(), 1 );
		fbo_src->draw(0, 0, fbo_dst_local.getWidth(), fbo_dst_local.getHeight());
	shader_Mask.end();
	//*/
	fbo_dst_local.end();
	
	/********************
	copy(stretch if needed).
	********************/
	if(fbo_dst){
		fbo_dst->begin();
		
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		fbo_dst_local.draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		// fbo_Text.draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		
		fbo_dst->end();
		
	}else{
		ofSetColor(255, 255, 255, 255);
		ofEnableAlphaBlending();
		fbo_dst_local.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

/******************************
******************************/
void EFFECT::draw_Mirror_Vert(ofFbo* fbo_src, ofFbo* fbo_dst)
{
	/********************
	draw to local fbo that has same size of fbo_src.
	********************/
	ofFbo fbo_dst_local;
	fbo_dst_local.allocate(fbo_src->getWidth(), fbo_src->getHeight(), GL_RGBA);
	
	fbo_dst_local.begin();
	shader_Mirror_Vert.begin();
	
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		shader_Mirror_Vert.setUniform2f( "screenCenter", fbo_dst_local.getWidth()/2, fbo_dst_local.getHeight()/2 );
		
		fbo_src->draw(0, 0, fbo_dst_local.getWidth(), fbo_dst_local.getHeight());
	
	shader_Mirror_Vert.end();
	fbo_dst_local.end();
	
	/********************
	copy(stretch if needed).
	********************/
	if(fbo_dst){
		fbo_dst->begin();
		
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		fbo_dst_local.draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		fbo_dst->end();
		
	}else{
		ofSetColor(255, 255, 255, 255);
		ofEnableAlphaBlending();
		fbo_dst_local.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

/******************************
******************************/
void EFFECT::draw_Mirror_LR(ofFbo* fbo_src, ofFbo* fbo_dst)
{
	/********************
	draw to local fbo that has same size of fbo_src.
	********************/
	ofFbo fbo_dst_local;
	fbo_dst_local.allocate(fbo_src->getWidth(), fbo_src->getHeight(), GL_RGBA);
	
	fbo_dst_local.begin();
	shader_Mirror_LR.begin();
	
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		shader_Mirror_LR.setUniform2f( "screenCenter", fbo_dst_local.getWidth()/2, fbo_dst_local.getHeight()/2 );
		
		fbo_src->draw(0, 0, fbo_dst_local.getWidth(), fbo_dst_local.getHeight());
	
	shader_Mirror_LR.end();
	fbo_dst_local.end();
	
	/********************
	copy(stretch if needed).
	********************/
	if(fbo_dst){
		fbo_dst->begin();
		
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		fbo_dst_local.draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		fbo_dst->end();
		
	}else{
		ofSetColor(255, 255, 255, 255);
		ofEnableAlphaBlending();
		fbo_dst_local.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

/******************************
******************************/
void EFFECT::draw_Kaleido(ofFbo* fbo_src, ofFbo* fbo_dst)
{	/********************
	********************/
	float dt = ofClamp(ElapsedTime - LastInt_sec, 0, 0.1);
	
	// const float deg_per_sec = 10;
	const float deg_per_sec = 15;
	k_angle += deg_per_sec * dt;
	if(360 <= k_angle) k_angle -= 360;
	
	LastInt_sec = ElapsedTime;
	
	/********************
	draw to local fbo that has same size of fbo_src.
	********************/
	ofFbo fbo_dst_local;
	fbo_dst_local.allocate(fbo_src->getWidth(), fbo_src->getHeight(), GL_RGBA);
	
	fbo_dst_local.begin();
	shader_Kaleido.begin();
	
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		shader_Kaleido.setUniform2f( "screenCenter", fbo_dst_local.getWidth()/2, fbo_dst_local.getHeight()/2 );
		shader_Kaleido.setUniform2f( "kcenter", fbo_src->getWidth()/2, fbo_src->getHeight()/2 );
		shader_Kaleido.setUniform1i( "ksectors", 10 );
		
		shader_Kaleido.setUniform1f( "kangleRad", ofDegToRad(k_angle) );
		// shader_Kaleido.setUniform1f( "kangleRad", 0 );
		
		shader_Kaleido.setUniform1f( "alpha", 1.0 );
		fbo_src->draw(0, 0, fbo_dst_local.getWidth(), fbo_dst_local.getHeight());
	
	shader_Kaleido.end();
	fbo_dst_local.end();
	
	/********************
	copy(stretch if needed).
	********************/
	if(fbo_dst){
		fbo_dst->begin();
		
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		ofDisableAlphaBlending();
		
		fbo_dst_local.draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		fbo_dst->end();
		
	}else{
		ofSetColor(255, 255, 255, 255);
		ofEnableAlphaBlending();
		fbo_dst_local.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

/******************************
******************************/
void EFFECT::draw_None(ofFbo* fbo_src, ofFbo* fbo_dst)
{
	/********************
	copy(stretch if needed).
	********************/
	if(fbo_dst){
		fbo_dst->begin();
		
		ofDisableAlphaBlending();
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		
		fbo_src->draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		fbo_dst->end();
		
	}else{
		ofSetColor(255, 255, 255, 255);
		ofEnableAlphaBlending();
		fbo_src->draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

/******************************
******************************/
void EFFECT::draw_Split(ofFbo* fbo_src, ofFbo* fbo_dst, int NumSplit)
{
	/********************
	draw to local fbo that has same size of fbo_src.
	********************/
	ofFbo fbo_dst_local;
	fbo_dst_local.allocate(fbo_src->getWidth(), fbo_src->getHeight(), GL_RGBA);
	
	fbo_dst_local.begin();
	shader_Split.begin();
	
		ofDisableAlphaBlending();
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		
		shader_Split.setUniform2f( "screenSize", fbo_dst_local.getWidth(), fbo_dst_local.getHeight() );
		shader_Split.setUniform1i( "NumSplit", NumSplit);
		
		fbo_src->draw(0, 0, fbo_dst_local.getWidth(), fbo_dst_local.getHeight());
	
	shader_Split.end();
	fbo_dst_local.end();
	
	/********************
	copy(stretch if needed).
	********************/
	if(fbo_dst){
		fbo_dst->begin();
		
		ofDisableAlphaBlending();
		ofBackground(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		
		fbo_dst_local.draw(0, 0, fbo_dst->getWidth(), fbo_dst->getHeight());
		fbo_dst->end();
		
	}else{
		ofSetColor(255, 255, 255, 255);
		ofEnableAlphaBlending();
		fbo_dst_local.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
}

/******************************
******************************/
EFFECT::EFFECT_TYPE EFFECT::get_RandomSelect_EffectType()
{
	int RandomNum = rand() % TOTAL_WEIGHT;
	
	if(RandomNum <= WEIGHT__NONE){
		return EFFECT_TYPE__NONE;
		
	}else if(RandomNum <= WEIGHT__SPLIT){
		int detail = rand() % 3;
		switch(detail){
			case 0:
				return EFFECT_TYPE__SPLIT_2x2;
			case 1:
				return EFFECT_TYPE__SPLIT_3x3;
			case 2:
				return EFFECT_TYPE__SPLIT_5x5;
		}
		
	}else if(RandomNum <= WEIGHT__KALEIDOSCOPE){
		return EFFECT_TYPE__KALEIDOSCOPE;
		
	}else if(RandomNum <= WEIGHT__MIRROR_LR){
		return EFFECT_TYPE__MIRROR_LR;
		
	}else if(RandomNum <= WEIGHT__MIRROR_VERT){
		return EFFECT_TYPE__MIRROR_VERT;
		
	}else if(RandomNum <= WEIGHT__TEXTMASK){
		return EFFECT_TYPE__TEXTMASK;
		
	}else{
		ERROR_MSG(); std::exit(1);
	}
}



