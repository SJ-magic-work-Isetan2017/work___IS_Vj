/************************************************************
************************************************************/
#include "Stage.h"

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
char STAGE::path[BUF_SIZE];
vector<string> STAGE::FileTable;
int STAGE::FileId;
bool STAGE::b_FileTable_Ready;
EFFECT* STAGE::EffectStage;

/************************************************************
************************************************************/

/******************************
******************************/
STAGE::STAGE(ofVec3f _pos, ofVec3f _Rotate_vec, float _Rotate_deg)
: pos(_pos)
, Rotate_vec(_Rotate_vec)
, Rotate_deg(_Rotate_deg)
{
	/********************
	最初のobjectがnewされた時のみ、backに用いるFileのTable Arrayを作成する。
	以降、このtableを使い続ける.
	********************/
	if(!STAGE::b_FileTable_Ready){
		prepare_FileTable();
		prepare_EffectStage();
	}
	
	setup();
	
	/********************
	********************/
	img.load(FileTable[FileId].c_str());
	FileId_toNext();
	
	/********************
	********************/
	NUM_CHARACTERS = rand() % MAX_NUM_CHARACTERS + 1;
	for(int i = 0; i < NUM_CHARACTERS; i++){
		character[i] = new CHARACTER(NUM_CHARACTERS, i);
	}
	
	/********************
	********************/
	EffectType = EffectStage->get_RandomSelect_EffectType();
}

/******************************
******************************/
STAGE::~STAGE()
{
	for(int i = 0; i < NUM_CHARACTERS; i++){
		delete character[i];
	}
}

/******************************
******************************/
void STAGE::changeEffect()
{
	const int MAX_RETRY = 5;
	
	for(int i = 0; i < MAX_RETRY; i++){
		EffectType = EffectStage->get_RandomSelect_EffectType();
		
		if( (NUM_CHARACTERS == 2) && (EffectType == EFFECT::EFFECT_TYPE__KALEIDOSCOPE) ){
			/*
				真ん中にcharacterが配置されていないので、何も絵が出ない時間が生じてしまう.
				retry routineを追加した代わりに、"WEIGHT__KALEIDOSCOPE"を重くしてBalanceを取った.
			*/
		}else{
			break;
		}
	}
}

/******************************
******************************/
void STAGE::setup()
{
	fbo.allocate(WIDTH, HEIGHT, GL_RGBA);
	fbo_char_src.allocate(WIDTH, HEIGHT, GL_RGBA);
	fbo_char_dst.allocate(WIDTH, HEIGHT, GL_RGBA);
}

/******************************
******************************/
void STAGE::update(float fftGain)
{
	/********************
	********************/
	fbo_char_src.begin();
	ofBackground(0, 0, 0, 0);
	for(int i = 0; i < NUM_CHARACTERS; i++){
		character[i]->update(fftGain);
		character[i]->draw(fftGain);
	}
	fbo_char_src.end();
	
	/********************
	********************/
	EffectStage->update();
	EffectStage->draw(EffectType, &fbo_char_src, &fbo_char_dst);
	
	/********************
	********************/
	fbo.begin();
	ofPushStyle();
	ofPushMatrix();
		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofBackground(0, 0, 0, 0);
			
		ofTranslate(WIDTH/2, HEIGHT/2);
		
		// center:noise
		
		// fft sync.
		float fftScale = ofMap(fftGain, 0, Gui_Stage->fftGain_Range, 1, 2);
		ofScale(fftScale, fftScale);
		
		ofColor color = Gui_Stage->BackImageColor;
		ofSetColor(color);
		img.setAnchorPercent(0.5, 0.5);
		img.draw(0, 0);
	
	ofPopMatrix();
		ofSetColor(255);
		fbo_char_dst.draw(0, 0);
	
	ofPopStyle();
	fbo.end();
}

/******************************
******************************/
void STAGE::draw()
{
	ofPushMatrix();
	ofPushStyle();
		ofDisableAlphaBlending();
		
		fbo.setAnchorPercent(0.5, 0.5);
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(Rotate_deg, Rotate_vec.x, Rotate_vec.y, Rotate_vec.z);
		
		ofSetColor(255);
		fbo.draw(0, 0);
	ofPopStyle();
	ofPopMatrix();
}

/******************************
******************************/
void STAGE::FileId_toNext()
{
	FileId++;
	if(FileTable.size() <= FileId) FileId = 0;
	
}

/******************************
******************************/
void STAGE::ReadConfig()
{
	/********************
	********************/
	FILE* fp;
	fp = fopen("../../../data/config.txt", "r");
	if(fp == NULL) { ERROR_MSG(); std::exit(1); }
	
	char buf[BUF_SIZE];
	while(fscanf(fp, "%s", buf) != EOF){
		if(strcmp(buf, "<back>") == 0){
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
	printf("\n> STAGE:config data\n");
	printf("path of Back Image:%s\n", path);
}

/******************************
******************************/
void STAGE::prepare_FileTable()
{
	/********************
	********************/
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
void STAGE::prepare_EffectStage()
{
	EffectStage = new EFFECT;
	EffectStage->setup();
}

/******************************
******************************/
void STAGE::makeup_mov_table(const string dirname, vector<string>& FileTable)
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
void STAGE::shuffle_Table(vector<string>& FileTable)
{
	int i = FileTable.size();
	
	while(i--){
		int j = rand() % (i + 1);
		
		string temp = FileTable[i];
		FileTable[i] = FileTable[j];
		FileTable[j] = temp;
	}
}

