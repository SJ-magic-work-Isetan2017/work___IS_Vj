/************************************************************
************************************************************/
#include "Particle.h"

/************************************************************
************************************************************/
/****************************************
****************************************/
class TIMETABLE_ON{
private:
	int h;
	int m;
	
public:
	TIMETABLE_ON(int _h, int _m)
	: h(_h), m(_m)
	{
	}
	
	float get_ClockInOneExpression(){
		return h + float(m)/60;
	}
};
	
/************************************************************
************************************************************/
/********************
********************/
static const ofFloatColor ParticleColor_init(0.2, 0.5, 1.0, 0.5);
static ofFloatColor ParticleColor_From(ParticleColor_init);
static ofFloatColor ParticleColor_To(ParticleColor_init);
static ofFloatColor ParticleColor(ParticleColor_init);
static float et_interval_ColorChange = 0;
static float et_ColorChange_Start = 0;

/********************
********************/
TIMETABLE_ON TimeTable_On[] = {
	TIMETABLE_ON(19,  0),
	TIMETABLE_ON(19, 30),
	TIMETABLE_ON(20,  0),
	TIMETABLE_ON(20, 30),
	TIMETABLE_ON(21,  0),
	TIMETABLE_ON(21, 30),
	TIMETABLE_ON(22,  0),
	
	TIMETABLE_ON(23, 30),
};

const int NUM_TIMETABLE_ON = sizeof(TimeTable_On) / sizeof(TimeTable_On[0]);
int id_TimeTableOn = 0;

/************************************************************
Particle methods
************************************************************/

/******************************
******************************/
Particle::Particle(){
	set_friction(1);
}

/******************************
******************************/
void Particle::setup(ofVec2f _position, ofVec2f _velocity){
	position = _position;
	velocity = _velocity;
}

/******************************
******************************/
void Particle::setup(float positionX, float positionY, float velocityX, float velocityY){
	position = ofVec2f(positionX, positionY);
	velocity = ofVec2f(velocityX, velocityY);
}

/******************************
******************************/
void Particle::resetForce(){
	force.set(0, 0);
}

/******************************
******************************/
void Particle::updateVel(float dt){
	velocity = velocity * (1 - friction_DownPer_ms * dt);
	velocity += force * dt;
}

/******************************
******************************/
void Particle::updatePos(float dt){
	position += velocity * dt;
}

/******************************
******************************/
void Particle::update(float dt){
	updateVel(dt);
	updatePos(dt);
	
	throughOfWalls();
}

/******************************
******************************/
void Particle::throughOfWalls(){
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	if (position.x < minx) {
		position.x = maxx;
	}
	if (position.y < miny) {
		position.y = maxy;
	}
	if (position.x > maxx) {
		position.x = minx;
	}
	if (position.y > maxy) {
		position.y = miny;
	}
}

/******************************
description
	引き付けあう力
	
param
	scale
		中心からの距離によってForceが決まるが、これに対するscaling.
		時間は無関係である点に注意。
******************************/
void Particle::addAttractionForce(float x, float y, float radius, float scale)
{
	/********************
	********************/
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	
	ofVec2f diff = position - posOfForce;
	float length = diff.length();

	bool bAmCloseEnough = true;
	if (0 < radius){
		if (radius < length)	bAmCloseEnough = false;
		else					bAmCloseEnough = true;
	}else{
		bAmCloseEnough = false;
	}

	if (bAmCloseEnough == true){
		// 距離から点にかかる力ベクトルを計算
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = - diff.x * scale * pct;
		force.y = - diff.y * scale * pct;
	}
}


/************************************************************
PARTICLE_SET methods
************************************************************/
/******************************
******************************/
PARTICLE_SET::PARTICLE_SET()
: LastMusicTime(-1)
, State(STATE_ON)
, b_End(false)
{
	Verts = new ofVec3f[NUM_PARTICLES];
	Color = new ofFloatColor[NUM_PARTICLES];
	
	if(BootMode == BOOT_MODE__AUTOPLAY){
		thread_TimeTable = THREAD__COLOR_TIMETABLE::getInstance();
	}
}

/******************************
******************************/
PARTICLE_SET::~PARTICLE_SET()
{
	delete[] Verts;
	delete[] Color;
}

/******************************
******************************/
void PARTICLE_SET::exit()
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
void PARTICLE_SET::setup()
{
	/********************
	********************/
	atraction = false;
	
	/********************
	********************/
	init_particleArray();
	
	/********************
	********************/
	for(int i = 0; i < NUM_PARTICLES; i++){
		Color[i].set(1.0, 1.0, 1.0, 1.0);
		Verts[i].set(ofVec3f(0.0, 0.0, 0.0));
	}
	Vbo.setVertexData(Verts, NUM_PARTICLES, GL_DYNAMIC_DRAW);
	Vbo.setColorData(Color, NUM_PARTICLES, GL_DYNAMIC_DRAW);
	
	/********************
	********************/
	SetGravityPos(GRAVITY_POS__CENTER);
	
	/********************
	Recはしない.
	Dmxで作成したScenarioを転用.
	********************/
	if(BootMode == BOOT_MODE__AUTOPLAY){
		thread_TimeTable->setup();
		thread_TimeTable->startThread(true, false); // blocking, non verboss
	}
}

/******************************
******************************/
void PARTICLE_SET::RandomSet_GravityPosition()
{
	SetGravityPos(GRAVITY_POS(rand() % NUM_GRAVITY_POS));
	
	/*
	switch(GravityPos){
		case GRAVITY_POS__TOP_LEFT:
			printf("GRAVITY_POS__TOP_LEFT\n");
			break;
		case GRAVITY_POS__TOP_CENTER:
			printf("GRAVITY_POS__TOP_CENTER\n");
			break;
		case GRAVITY_POS__TOP_RIGHT:
			printf("GRAVITY_POS__TOP_RIGHT\n");
			break;
			
		case GRAVITY_POS__MID_LEFT:
			printf("GRAVITY_POS__MID_LEFT\n");
			break;
		case GRAVITY_POS__CENTER:
			printf("GRAVITY_POS__CENTER\n");
			break;
		case GRAVITY_POS__MID_RIGHT:
			printf("GRAVITY_POS__MID_RIGHT\n");
			break;
			
		case GRAVITY_POS__BOTTOM_LEFT:
			printf("GRAVITY_POS__BOTTOM_LEFT\n");
			break;
		case GRAVITY_POS__BOTTOM_CENTER:
			printf("GRAVITY_POS__BOTTOM_CENTER\n");
			break;
		case GRAVITY_POS__BOTTOM_RIGHT:
			printf("GRAVITY_POS__BOTTOM_RIGHT\n");
			break;
		default:
			break;
	}
	*/
}

/******************************
******************************/
void PARTICLE_SET::SetGravityPos(GRAVITY_POS pos)
{
	const double x_ofs = 0.01;
	const double y_ofs = 0.01;
	
	GravityPos = pos;
	
	switch(pos){
		case GRAVITY_POS__TOP_LEFT:
			mouseX = int(ofGetWidth() * x_ofs);
			mouseY = int(ofGetHeight() * y_ofs);
			break;
		case GRAVITY_POS__TOP_CENTER:
			mouseX = int(ofGetWidth()/2);
			mouseY = int(ofGetHeight() * y_ofs);
			break;
		case GRAVITY_POS__TOP_RIGHT:
			mouseX = int(ofGetWidth() * (1 - x_ofs));
			mouseY = int(ofGetHeight() * y_ofs);
			break;
			
		case GRAVITY_POS__MID_LEFT:
			mouseX = int(ofGetWidth() * x_ofs);
			mouseY = int(ofGetHeight()/2);
			break;
		case GRAVITY_POS__CENTER:
			mouseX = int(ofGetWidth()/2);
			mouseY = int(ofGetHeight()/2);
			break;
		case GRAVITY_POS__MID_RIGHT:
			mouseX = int(ofGetWidth() * (1 - x_ofs));
			mouseY = int(ofGetHeight()/2);
			break;
			
		case GRAVITY_POS__BOTTOM_LEFT:
			mouseX = int(ofGetWidth() * x_ofs);
			mouseY = int(ofGetHeight() * (1 - y_ofs));
			break;
		case GRAVITY_POS__BOTTOM_CENTER:
			mouseX = int(ofGetWidth()/2);
			mouseY = int(ofGetHeight() * (1 - y_ofs));
			break;
		case GRAVITY_POS__BOTTOM_RIGHT:
			mouseX = int(ofGetWidth() * (1 - x_ofs));
			mouseY = int(ofGetHeight() * (1 - y_ofs));
			break;
		default:
			break;
	}
}

/******************************
******************************/
void PARTICLE_SET::Refresh_friction()
{
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i].set_friction(Gui_Particle->Particle_friction_DownPer_sec/1000);
	}
}

/******************************
******************************/
void PARTICLE_SET::StateChart_EnableAttraction()
{
	enum STATE{
		STATE_RELEASE,
		STATE_ATTRACT,
	};
	
	static STATE State = STATE_RELEASE;
	
	static int counter = 0;
	static float t_Enter = 0;
	float now = ofGetElapsedTimef();
	
	const float t_Interval = 5.0;
	switch(State){
		case STATE_RELEASE:
			if(t_Interval < now - t_Enter){
				State = STATE_ATTRACT;
				t_Enter = now;
			}
			break;
			
		case STATE_ATTRACT:
			if(t_Interval < now - t_Enter){
				State = STATE_RELEASE;
				t_Enter = now;
				
				counter++;
			}
			break;
	}
	
	if(State == STATE_RELEASE)	atraction = false;
	else						atraction = true;
	
	if(4 < counter){
		counter = 0;
		RandomSet_GravityPosition();
	}
}

/******************************
******************************/
void PARTICLE_SET::check_then_Transition_TimeTableOn()
{
	/********************
	********************/
	if(b_End){
		State = STATE_OFF;
		return;
	}
	
	/********************
	********************/
	int h = ofGetHours();
	int m = ofGetMinutes();
	float Clock_InOneExpression = h + float(m)/60;
	
	State = STATE_OFF;
	while( !b_End && (TimeTable_On[id_TimeTableOn].get_ClockInOneExpression() <= Clock_InOneExpression) ){
		State = STATE_ON;
		
		id_TimeTableOn++;
		if(NUM_TIMETABLE_ON <= id_TimeTableOn){
			b_End = true;
		}
	}
}

/******************************
******************************/
void PARTICLE_SET::update(int MusicTime_ms, double Dj_a)
{
	/********************
	********************/
	if( IsMusicLoop(MusicTime_ms) && (BootMode == BOOT_MODE__AUTOPLAY) ){
		thread_TimeTable->stopThread();
		thread_TimeTable->waitForThread(true);
		thread_TimeTable->Reset();
		thread_TimeTable->startThread(true, false);
		
		check_then_Transition_TimeTableOn();
		
	}else{
		if(BootMode == BOOT_MODE__AUTOPLAY){
			if(thread_TimeTable->IsReady()){
				checkScenario_ColorChange(MusicTime_ms);
			}
		}
		
		ColorTransition();
	}
	
	LastMusicTime = MusicTime_ms;
	
	
	/********************
	********************/
	if(State == STATE_ON){
		/********************
		********************/
		static float LastINT = ofGetElapsedTimef();
		float now = ofGetElapsedTimef();
		float dt = ofClamp(now - LastINT, 0, 0.1) * 1000;
		
		LastINT = now;
		
		/********************
		********************/
		StateChart_EnableAttraction();
		
		/********************
		各particleのfrictionは本来一定だが、
		guiにてparameter調整するため、ここにrefreshを入れる。
		********************/
		Refresh_friction();
		
		/********************
		update the position of all particle.
		********************/
		for (int i = 0; i < particles.size(); i++){
			particles[i].resetForce();
		}
		
		for (int i = 0; i < particles.size(); i++){
			if (atraction)	particles[i].addAttractionForce(mouseX, mouseY, ofGetWidth(), Gui_Particle->Particle_forceScale );
	
			particles[i].update(dt);
		}
		
		/********************
		********************/
		const double Alpha_max = Gui_Particle->ParticleColor->w;
		double tan = Alpha_max / Gui_Particle->ParticleSpeedThresh;
		
		/********************
		colorはSpeed downと共にαが下がっていくようにした。
		********************/
		for (int i = 0; i < particles.size(); i++) {
			/* */
			Verts[i] = ofVec3f(particles[i].get_pos_x(), particles[i].get_pos_y(), 0);
			
			
			/* */
			float Alpha;
			if(Gui_Particle->ParticleSpeedThresh < particles[i].get_Speed()){
				Alpha = Alpha_max;
			}else{
				Alpha = tan * particles[i].get_Speed();
			}
			
			if(BootMode == BOOT_MODE__AUTOPLAY){
				Color[i].set(ParticleColor.r, ParticleColor.g, ParticleColor.b, Alpha * Dj_a);
			}else{
				// Color[i].set(ParticleColor->x, ParticleColor->y, ParticleColor->z, ParticleColor->w);
				Color[i].set(Gui_Particle->ParticleColor->x, Gui_Particle->ParticleColor->y, Gui_Particle->ParticleColor->z, Alpha * Dj_a);
			}
		}
		
		Vbo.updateVertexData(Verts, NUM_PARTICLES);
		Vbo.updateColorData(Color, NUM_PARTICLES);
	}
}

/******************************
******************************/
void PARTICLE_SET::checkScenario_ColorChange(int MusicTime)
{
	if(BootMode == BOOT_MODE__AUTOPLAY){
		thread_TimeTable->update(MusicTime);
		
		TIME_N_DATASET__COLOR ret;
		((THREAD__COLOR_TIMETABLE*)thread_TimeTable)->checkEvent(ret);
		
		if(ret.R != -1){
			ParticleColor_From = ParticleColor;
			ParticleColor_To = ofFloatColor(float(ret.R)/255, float(ret.G)/255, float(ret.B)/255);
			
			et_interval_ColorChange = ret.t_interval_ColorChange;
			
			et_ColorChange_Start = ElapsedTime;
		}
	}
}

/******************************
******************************/
void PARTICLE_SET::ColorTransition()
{
	/********************
	********************/
	if(BootMode != BOOT_MODE__AUTOPLAY) return;
	
	/********************
	********************/
	if(et_interval_ColorChange == 0){
		ParticleColor = ParticleColor_To;
		return;
	}

	/********************
	********************/
	enum{
		MAXPROGRESS = 10000,
	};

	int Progress;
	if(ElapsedTime <= et_ColorChange_Start){
		Progress = 0;
	}else if(et_interval_ColorChange <= ElapsedTime - et_ColorChange_Start){
		Progress = MAXPROGRESS;
	}else{
		Progress = (ElapsedTime - et_ColorChange_Start) * MAXPROGRESS / et_interval_ColorChange;
	}
	
	float R = ParticleColor_From.r + (ParticleColor_To.r - ParticleColor_From.r) * Progress / MAXPROGRESS;
	float G = ParticleColor_From.g + (ParticleColor_To.g - ParticleColor_From.g) * Progress / MAXPROGRESS;
	float B = ParticleColor_From.b + (ParticleColor_To.b - ParticleColor_From.b) * Progress / MAXPROGRESS;
	
	ParticleColor = ofFloatColor(R, G, B);

	/******************************
	******************************/
	if(MAXPROGRESS <= Progress){
		et_interval_ColorChange = 0;
	}
}

/******************************
******************************/
bool PARTICLE_SET::IsMusicLoop(int MusicTime_ms)
{
	if(MusicTime_ms < LastMusicTime)	return true;
	else								return false;
}

/******************************
******************************/
void PARTICLE_SET::draw()
{
	/********************
	********************/
	if(State == STATE_OFF) return;
	
	
	/********************
	********************/
	ofPushStyle();
	
	/********************
	********************/
	// ofBackground(0, 0, 0, 0);
	
	/********************
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	/********************
	********************/
	ofSetColor(255, 255, 255, 255);
	glPointSize(Gui_Particle->ParticleSize);
	glEnable(GL_POINT_SMOOTH);
	Vbo.draw(GL_POINTS, 0, NUM_PARTICLES);
	
	/********************
	********************/
	clear_VboSetting_gl();
	
	/*
	// 重力の点を描く
	if (atraction) {
		ofSetColor(255, 0, 0);
	} else {
		ofSetColor(0, 255, 255);
	}
	ofCircle(mouseX, mouseY, 4);
	
	//文字の背景
	ofSetColor(0, 127);
	ofRect(0, 0, 200, 60);
	
	// ログを表示
	ofSetColor(255);
	ofDrawBitmapString("fps = " + ofToString(ofGetFrameRate()), 10, 20);
	ofDrawBitmapString("Particle num = " + ofToString(particles.size()), 10, 40);
	*/
	
	ofPopStyle();
}

/******************************
descrition
	ofVboで描画すると、openGlの設定が何らか変わるようだ。
	この結果、次に来る描画が所望の動作とならないケース多数。
		次のfunctionが描画されないなど
			ofDrawBitmapString()
			image.draw()
			
	この対応として、
		ofCircle(). ofRect().
	等を1発いれてやることで、OKとなった。
	おそらく、この関数内で、openGl設定が、また変わるのだろう。
		α = 0;
	にて描画する。
******************************/
void PARTICLE_SET::clear_VboSetting_gl()
{
	ofSetColor(255, 255, 255, 0);
	ofCircle(0, 0, 1);
	
	ofSetColor(255, 255, 255, 255);
}

/******************************
******************************/
void PARTICLE_SET::init_particleArray()
{
	particles.clear();
	for (int i = 0; i < NUM_PARTICLES; i++) {
		Particle p;
		
		p.set_friction(0.1/1000);
		p.setup(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())), ofVec2f(0, 0));
				
		particles.push_back(p);
	}
}
