/************************************************************
************************************************************/
#include "UseFftResult.h"

/************************************************************
************************************************************/

/******************************
C++ Vector size is returning zero
	http://stackoverflow.com/questions/31372809/c-vector-size-is-returning-zero
	
	contents
		v1.reserve(30);
		does not change the number of elements in the vector, it just makes sure the vector has enough space to save 30 elements without reallocation.
		so now, v1.size() returns zero.
		
		Use
		v1.resize(30);
		to change to number of elements in the vector.
******************************/
USE_FFT_RESULT::USE_FFT_RESULT()
: b_NoMessageYet(true)
{
	/********************
	********************/
#ifdef SJ_DEBUG__MEASTIME
	fp_Log = fopen("../../../Log.csv", "w");
#endif	
}

/******************************
******************************/
USE_FFT_RESULT::~USE_FFT_RESULT()
{
	/********************
	********************/
#ifdef SJ_DEBUG__MEASTIME
	fclose(fp_Log);
#endif	
}

/******************************
******************************/
void USE_FFT_RESULT::setup()
{
	/********************
	********************/
	udpConnection.Create();
	udpConnection.Bind(UDP_RECEIVE_PORT);
	udpConnection.SetNonBlocking(true);
}

/******************************
******************************/
void USE_FFT_RESULT::setup_on_1stMessage()
{
	if(b_NoMessageYet){
		/********************
		********************/
		b_NoMessageYet = false;
		
		/********************
		********************/
		for(int i = 0; i < NUM_FFT_GRAPHS; i++){
			VboVerts[i].resize(N * 4); // square
			VboColor[i].resize(N * 4); // square
		}
		
		fft_Gain.resize(N);
		AudioSample_Rev.resize(N);
		
		/********************
		********************/
		for(int i = 0; i < fft_Gain.size(); i++)			fft_Gain[i] = 0;
		for(int i = 0; i < AudioSample_Rev.size(); i++)		AudioSample_Rev[i] = 0;
		
		RefreshVerts();
		Refresh_BarColor();
		
		/********************
		********************/
		for(int i = 0; i < NUM_FFT_GRAPHS; i++){
			Vbo[i].setVertexData(&VboVerts[i][0], VboVerts[i].size(), GL_DYNAMIC_DRAW);
			Vbo[i].setColorData(&VboColor[i][0], VboColor[i].size(), GL_DYNAMIC_DRAW);
		}
	}
}

/******************************
******************************/
void USE_FFT_RESULT::RefreshVerts()
{
	/********************
	********************/
	const double d_theta = 360.0 / double(N);
	
	for(int id = 0; id < NUM_FFT_GRAPHS; id++){
		for(int i = 0; i < N; i++){
			AXIS axis;
			
			axis.rotate(-(Gui_FFT[id]->Rotaion_deg + i * d_theta), ofVec3f(0, 0, 1));
			axis.translate(0, Gui_FFT[id]->GraphRadius, 0);
			
			/********************
			わざと、尖らせる
			********************/
			ofVec3f target;
			
			axis.get(ofVec3f(-Gui_FFT[id]->BarWidth_Bottom/2, 0, 0), target);
			VboVerts[id][i * 4 + 0].set(target);
	
			axis.get(ofVec3f(-Gui_FFT[id]->BarWidth_Top/2, abs( ofMap(AudioSample_Rev[i], -Gui_FFT[id]->AudioSample_Amp, Gui_FFT[id]->AudioSample_Amp, -Gui_FFT[id]->BarHeight, Gui_FFT[id]->BarHeight)), 0), target);
			VboVerts[id][i * 4 + 1].set(target);
			
			axis.get(ofVec3f(Gui_FFT[id]->BarWidth_Top/2, abs( ofMap(AudioSample_Rev[i], -Gui_FFT[id]->AudioSample_Amp, Gui_FFT[id]->AudioSample_Amp, -Gui_FFT[id]->BarHeight, Gui_FFT[id]->BarHeight)), 0), target);
			VboVerts[id][i * 4 + 2].set(target);
			
			axis.get(ofVec3f(Gui_FFT[id]->BarWidth_Bottom/2, 0, 0), target);
			VboVerts[id][i * 4 + 3].set(target);
		}
	}
}

/******************************
******************************/
void USE_FFT_RESULT::Refresh_BarColor()
{
	for(int i = 0; i < NUM_FFT_GRAPHS; i++){
		ofColor color = Gui_FFT[i]->BarColor;
		for(int j = 0; j < VboColor[i].size(); j++) { VboColor[i][j].set( double(color.r)/255, double(color.g)/255, double(color.b)/255, double(color.a)/255); }
	}
}

/******************************
******************************/
float USE_FFT_RESULT::get_Gain(int id)
{
	if(b_NoMessageYet)	return 0;
	else				return fft_Gain[id];
}

/******************************
******************************/
void USE_FFT_RESULT::update()
{
	/********************
	********************/
	char udpMessage[SIZE_UPD];
	udpConnection.Receive(udpMessage,SIZE_UPD);
	string message=udpMessage;
	if(message!=""){
		vector<string> params = ofSplitString(message, "</p>");
		
		vector<string> _AudioSample_Rev = ofSplitString(params[0], ",");
		vector<string> _fft_Gain = ofSplitString(params[1], ",");
		
		if(b_NoMessageYet){
			N = _AudioSample_Rev.size() - 1; // "0" is added at last value.
			printf("\n> UseFftResult\n");
			printf("N = %d\n", N);
			
			setup_on_1stMessage();
		}
		
		for(int i = 0; i < N; i++){
			AudioSample_Rev[i] = atof(_AudioSample_Rev[i].c_str());
			fft_Gain[i] = atof(_fft_Gain[i].c_str());
		}
		
		while(message != ""){
			udpConnection.Receive(udpMessage,SIZE_UPD);
			message = udpMessage;
		}
	}

	/********************
	********************/
	if(b_NoMessageYet == false){
		/********************
		********************/
		RefreshVerts();
		Refresh_BarColor();
		
		/********************
		以下は、audioOutからの呼び出しだと segmentation fault となってしまった.
		********************/
		for(int i = 0; i < NUM_FFT_GRAPHS; i++){
			Vbo[i].updateVertexData(&VboVerts[i][0], VboVerts[i].size());
			Vbo[i].updateColorData(&VboColor[i][0], VboColor[i].size());
		}
	}
}

/******************************
******************************/
void USE_FFT_RESULT::draw()
{
	if(b_NoMessageYet == false){
		/********************
		********************/
		ofPushStyle();
		ofPushMatrix();
			
			/********************
			********************/
			ofEnableAlphaBlending();
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			
			/********************
			********************/
			ofTranslate(WIDTH/2, HEIGHT/2);
			ofScale(1, -1, 1);
			
			/********************
			********************/
			glPointSize(1.0);
			glLineWidth(1);
			
			for(int i = 0; i < NUM_FFT_GRAPHS; i++){
				ofPushMatrix();
					ofTranslate(Gui_FFT[i]->pos->x, Gui_FFT[i]->pos->y);
					Vbo[i].draw(GL_QUADS, 0, VboVerts[i].size());
					
					if(Gui_FFT[i]->b_GainAdjust){
						ofSetColor(0, 0, 255, 200);
						ofDrawCircle(0, 0, Gui_FFT[i]->GraphRadius + Gui_FFT[i]->BarHeight);
					}
				ofPopMatrix();
			}
			
		ofPopMatrix();
		ofPopStyle();
	}
}

/******************************
******************************/
void USE_FFT_RESULT::threadedFunction()
{
	while(isThreadRunning()) {
		lock();
		
		unlock();
		
		sleep(THREAD_SLEEP_MS);
	}
}



