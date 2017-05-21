/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class CHARACTER{
private:
	/****************************************
	****************************************/
	class META_NOISE{
	private:
		ofVec3f center;
		ofVec3f NoiseOfs; // ofNoise()の引数に使用するoffset.
		
	public:
		void setup(ofVec3f _center){
			center = _center;
			
			NoiseOfs.x = ofRandom(0, 10000);
			NoiseOfs.y = ofRandom(0, 10000);
			NoiseOfs.z = 0;
		}
		
		ofVec3f get_center(){
			return center;
		}
		
		ofVec3f cal_CurrentPosition(float amp, float ElapsedTime, float NoiseFreq){
			ofVec3f Noise;
			
			Noise.x = amp * ofSignedNoise(ElapsedTime/NoiseFreq + NoiseOfs.x);
			Noise.y = amp * ofSignedNoise(ElapsedTime/NoiseFreq + NoiseOfs.y);
			Noise.z = 0;
			
			return center + Noise;
		}
	};
	
	/****************************************
	****************************************/
	enum{
		NUM_POINTS = 4,
	};
	
	/****************************************
	****************************************/
	/********************
	Front image:File table
	********************/
	static char path[BUF_SIZE];
	static vector<string> FileTable;
	static int FileId;
	static bool b_FileTable_Ready;
	
	/********************
	********************/
	ofImage img;
	
	const int NUM_CHARACTERS_IN_MY_PARENT;
	const int CHARACTER_ID_IN_MY_PARENT;
	
	ofVbo Vbo;
	ofVec3f Verts[NUM_POINTS];
	ofVec2f TexCoord[NUM_POINTS];
	ofFloatColor Color[NUM_POINTS];
	
	META_NOISE p[NUM_POINTS];
	META_NOISE center;
	
	
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
	
public:
	/****************************************
	****************************************/
	CHARACTER(int _NUM_CHARACTERS_IN_MY_PARENT, int _CHARACTER_ID_IN_MY_PARENT);
	~CHARACTER();
	
	void update(float fftGain);
	void draw(float fftGain);
};

