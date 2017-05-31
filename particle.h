/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"

#include "sj_common.h"
#include "th_Color.h"


/************************************************************
class
************************************************************/

/**************************************************
description
	particle positionの管理.
	描画機能は保持しない.
	
unit	
	ms
	pixel

note
	forceは、重力中心からの距離のみで決定され、時間は無関係。
	forceがvelocityに、velocityがpositionに、それぞれ反映される時は、dtが必要になる。
**************************************************/
class Particle {
private:
	/****************************************
	****************************************/
	/********************
	********************/
	ofVec2f position;
	ofVec2f velocity;
	
	/********************
	********************/
	float friction_DownPer_ms;
	ofVec2f force;
	
	
	/****************************************
	****************************************/
	void updateVel(float dt);
	void updatePos(float dt);
	void throughOfWalls();
	
public:
	/****************************************
	****************************************/
	Particle();

	void setup(ofVec2f position, ofVec2f velocity);
	void setup(float positionX, float positionY, float velocityX, float velocityY);

	void resetForce();

	void update(float dt);
	
	
	void addAttractionForce(float x, float y, float radius, float scale);
	
	float get_Speed()	{ return velocity.length(); }
	float get_pos_x()	{ return position.x; }
	float get_pos_y()	{ return position.y; }
	
	void set_friction(float val)	{ friction_DownPer_ms = val; }
};

/**************************************************
**************************************************/
class PARTICLE_SET {
private:
	/****************************************
	****************************************/
	enum{
		// NUM_PARTICLES = 100000,
		NUM_PARTICLES = 60000,
	};
	
	enum GRAVITY_POS{
		GRAVITY_POS__TOP_LEFT,
		GRAVITY_POS__TOP_CENTER,
		GRAVITY_POS__TOP_RIGHT,

		GRAVITY_POS__MID_LEFT,
		GRAVITY_POS__CENTER,
		GRAVITY_POS__MID_RIGHT,

		GRAVITY_POS__BOTTOM_LEFT,
		GRAVITY_POS__BOTTOM_CENTER,
		GRAVITY_POS__BOTTOM_RIGHT,
		
		
		NUM_GRAVITY_POS,
	};
	
	enum STATE{
		STATE_ON,
		STATE_OFF,
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	vector<Particle> particles;
	// ofVboMesh mesh;
	ofVbo Vbo;
	ofVec3f* Verts;
	ofFloatColor* Color;
	
	bool atraction;
	
	GRAVITY_POS GravityPos;
	int mouseX;
	int mouseY;
	
	/********************
	********************/
	THREAD_BASE* thread_TimeTable;
	int LastMusicTime;
	
	/********************
	********************/
	STATE State;
	bool b_End;
	
	
	
	/****************************************
	****************************************/
	/********************
	singleton
	********************/
	PARTICLE_SET();
	~PARTICLE_SET();
	PARTICLE_SET(const PARTICLE_SET&); // Copy constructor. no define.
	PARTICLE_SET& operator=(const PARTICLE_SET&); // コピー代入演算子. no define.
	
	/********************
	********************/
	void Refresh_friction();
	void clear_VboSetting_gl();
	void init_particleArray();
	void SetGravityPos(GRAVITY_POS pos);
	void StateChart_EnableAttraction();

	void RandomSet_GravityPosition();
	
	bool IsMusicLoop(int MusicTime_ms);
	void checkScenario_ColorChange(int MusicTime);
	void ColorTransition();
	void check_then_Transition_TimeTableOn();
	
public:
	/****************************************
	****************************************/
	static PARTICLE_SET* getInstance(){
		static PARTICLE_SET inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update(int MusicTime_ms, double Dj_a);
	void draw();
	
};



