/************************************************************
************************************************************/
#include "character.h"

/* for dir search */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h> 
#include <dirent.h>
#include <string>

using namespace std;


/************************************************************
************************************************************/
char CHARACTER::path[BUF_SIZE];
vector<string> CHARACTER::FileTable;
int CHARACTER::FileId;
bool CHARACTER::b_FileTable_Ready;


/************************************************************
************************************************************/

/******************************
******************************/
CHARACTER::CHARACTER(int _NUM_CHARACTERS_IN_MY_PARENT, int _CHARACTER_ID_IN_MY_PARENT)
: NUM_CHARACTERS_IN_MY_PARENT(_NUM_CHARACTERS_IN_MY_PARENT)
, CHARACTER_ID_IN_MY_PARENT(_CHARACTER_ID_IN_MY_PARENT)
{
	/********************
	最初のobjectがnewされた時のみ、Characterに用いるFileのTable Arrayを作成する。
	以降、このtableを使い続ける.
	********************/
	if(!CHARACTER::b_FileTable_Ready) prepare_FileTable();
	
	/********************
	********************/
	img.load(FileTable[FileId].c_str());
	FileId_toNext();
	
	/********************
	********************/
	p[0].setup( ofVec3f(-img.getWidth()/2, -img.getHeight()/2) );
	p[1].setup( ofVec3f(-img.getWidth()/2,  img.getHeight()/2 - 1) );
	p[2].setup( ofVec3f( img.getWidth()/2 - 1,  img.getHeight()/2 - 1) );
	p[3].setup( ofVec3f( img.getWidth()/2 - 1, -img.getHeight()/2) );
	
	const float RandomHeight = 5;
	if(NUM_CHARACTERS_IN_MY_PARENT == 1){
		center.setup( ofVec3f(0, ofRandom(-HEIGHT/RandomHeight, HEIGHT/RandomHeight)) );
		
	}else if(NUM_CHARACTERS_IN_MY_PARENT == 2){
		switch(CHARACTER_ID_IN_MY_PARENT){
			case 0:
				center.setup( ofVec3f(-WIDTH/4, ofRandom(-HEIGHT/RandomHeight, HEIGHT/RandomHeight)) );
				break;
			case 1:
				center.setup( ofVec3f( WIDTH/4, ofRandom(-HEIGHT/RandomHeight, HEIGHT/RandomHeight)) );
				break;
			default:
				ERROR_MSG(); std::exit(1);
				break;
		}
		
	}else if(NUM_CHARACTERS_IN_MY_PARENT == 3){
		switch(CHARACTER_ID_IN_MY_PARENT){
			case 0:
				center.setup( ofVec3f(-2 * WIDTH/6, ofRandom(-HEIGHT/RandomHeight, HEIGHT/RandomHeight)) );
				break;
			case 1:
				center.setup( ofVec3f(0, ofRandom(-HEIGHT/RandomHeight, HEIGHT/RandomHeight)) );
				break;
			case 2:
				center.setup( ofVec3f( 2 * WIDTH/6, ofRandom(-HEIGHT/RandomHeight, HEIGHT/RandomHeight)) );
				break;
			default:
				ERROR_MSG(); std::exit(1);
		}
		
	}else{
		ERROR_MSG(); std::exit(1);
	}
	
	/********************
	********************/
	for(int i = 0; i < NUM_POINTS; i++){
		Verts[i].set(p[i].get_center());
		Color[i].set(1.0, 1.0, 1.0, 1.0);
	}
	TexCoord[0].set(0, img.getHeight());
	TexCoord[1].set(0, 0);
	TexCoord[2].set(img.getWidth(), 0);
	TexCoord[3].set(img.getWidth(), img.getHeight());
	
	Vbo.setVertexData(Verts, NUM_POINTS, GL_DYNAMIC_DRAW);
	Vbo.setColorData(Color, NUM_POINTS, GL_DYNAMIC_DRAW);
	Vbo.setTexCoordData(TexCoord, NUM_POINTS, GL_DYNAMIC_DRAW);  
}

/******************************
******************************/
CHARACTER::~CHARACTER()
{
}

/******************************
******************************/
void CHARACTER::update(float fftGain)
{
	glDisable(GL_CULL_FACE);
	
	for(int i = 0; i < NUM_POINTS; i++){
		float fftVertNoiseScale;
		if( Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->b_vertNoise_fftSync)	fftVertNoiseScale = ofMap(fftGain, 0, Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->fftGain_Range, 1, 1.5);
		else																		fftVertNoiseScale = 1;
		Verts[i].set(p[i].cal_CurrentPosition(Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->vert_NoiseAmp * fftVertNoiseScale, ElapsedTime, Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->vert_NoiseFreq));
		
		Color[i].set(1.0, 1.0, 1.0, Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->Alpha);
	}
	
	Vbo.updateVertexData(Verts, NUM_POINTS);
	Vbo.updateColorData(Color, NUM_POINTS);
	Vbo.updateTexCoordData(TexCoord, NUM_POINTS);
}

/******************************
******************************/
void CHARACTER::draw(float fftGain)
{
	ofPushMatrix();
	ofPushStyle();
	
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		
		// ofBackground(0); // 親側でclear. character x 2以上のcaseでBlack上書きしないように.
		ofSetColor(255);
		
		ofTranslate(WIDTH/2, HEIGHT/2);
		ofScale(1, -1, 1);
		
		ofTranslate(center.cal_CurrentPosition(Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->center_NoiseAmp, ElapsedTime, Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->center_NoiseFreq));
		ofScale(Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->BaseScale, Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->BaseScale);
		
		float fftScale = ofMap(fftGain, 0, Gui_Character[NUM_CHARACTERS_IN_MY_PARENT - 1]->fftGain_Range, 1, 2);
		ofScale(fftScale, fftScale);
		
		/********************
		********************/
		Vbo.bind();
		img.bind();
		Vbo.draw(GL_QUADS, 0, NUM_POINTS);
		img.unbind();
		Vbo.unbind();
	
	ofPopStyle();
	ofPopMatrix();
}

/******************************
******************************/
void CHARACTER::FileId_toNext()
{
	FileId++;
	if(FileTable.size() <= FileId) FileId = 0;
	
}

/******************************
******************************/
void CHARACTER::ReadConfig()
{
	/********************
	********************/
	FILE* fp;
	fp = fopen("../../../data/config.txt", "r");
	if(fp == NULL) { ERROR_MSG(); std::exit(1); }
	
	char buf[BUF_SIZE];
	while(fscanf(fp, "%s", buf) != EOF){
		if(strcmp(buf, "<front>") == 0){
			/********************
			スキャン集合
				http://wisdom.sakura.ne.jp/programming/c/c58.html
			********************/
			fscanf(fp, "%[ \t]", buf); // space & tab 読み捨て
			fscanf(fp, "%[^\n]", buf); // \n以外を読み取る -> \nが来るまで読み込み(space also)
			sprintf(path, "%s", buf);
			
		}
	}
	
	fclose(fp);
	
	/********************
	********************/
	printf("\n> CHARACTER:config data\n");
	printf("path of Front Image:%s\n", path);
}

/******************************
******************************/
void CHARACTER::prepare_FileTable()
{
	if(!b_FileTable_Ready){
		
		ReadConfig();
		
		makeup_mov_table(path, FileTable);
		shuffle_Table(FileTable);
		FileId = 0;
		
		/*
		for(int i = 0; i < FileTable.size(); i++){
			printf("%s\n", FileTable[i].c_str());
		}
		*/
		
		b_FileTable_Ready = true;
	}
}

/******************************
******************************/
void CHARACTER::makeup_mov_table(const string dirname, vector<string>& FileTable)
{
	/********************
	********************/
	DIR *pDir;
	struct dirent *pEnt;
	struct stat wStat;
	string wPathName;

	pDir = opendir( dirname.c_str() );
	if ( NULL == pDir ) { ERROR_MSG(); std::exit(1); }

	pEnt = readdir( pDir );
	while ( pEnt ) {
		// .と..は処理しない
		if ( strcmp( pEnt->d_name, "." ) && strcmp( pEnt->d_name, ".." ) ) {
		
			wPathName = dirname + "/" + pEnt->d_name;
			
			// ファイルの情報を取得
			if ( stat( wPathName.c_str(), &wStat ) ) {
				printf( "Failed to get stat %s \n", wPathName.c_str() );
				break;
			}
			
			if ( S_ISDIR( wStat.st_mode ) ) {
				// nothing.
			} else {
			
				vector<string> str = ofSplitString(pEnt->d_name, ".");
				if(str[str.size()-1] == "png"){
					string Table_NewVal;
					
					Table_NewVal = wPathName;
					
					FileTable.push_back(Table_NewVal);
				}
			}
		}
		
		pEnt = readdir( pDir ); // 次のファイルを検索する
	}

	closedir( pDir );
}

/******************************
description
	fisher yates法
	偏りをなくすため、回を追うごとに乱数範囲を狭めていくのがコツ
	http://www.fumiononaka.com/TechNotes/Flash/FN0212003.html
******************************/
void CHARACTER::shuffle_Table(vector<string>& FileTable)
{
	int i = FileTable.size();
	
	while(i--){
		int j = rand() % (i + 1);
		
		string temp = FileTable[i];
		FileTable[i] = FileTable[j];
		FileTable[j] = temp;
	}
}

