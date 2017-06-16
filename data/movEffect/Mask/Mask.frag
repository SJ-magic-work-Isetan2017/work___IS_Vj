/************************************************************
ビルトイン関数(一部)
	http://qiita.com/edo_m18/items/71f6064f3355be7e4f45
************************************************************/
#version 120
#extension GL_ARB_texture_rectangle: enable

/************************************************************
************************************************************/
uniform sampler2DRect texture0;
uniform sampler2DRect texture1;	//Second texture

/************************************************************
************************************************************/

/******************************
******************************/
void main(){
	vec2 pos = gl_TexCoord[0].xy;
	
	vec4 color0 = texture2DRect(texture0, pos);
	vec4 color1 = texture2DRect(texture1, pos);
	
	//Compute resulted color
	vec4 color;
	if(0 < color1.a){
		color.rgb = color0.rgb;
		color.a = color0.a;
	}else{
		color.rgb = vec3(0, 0, 0);
		color.a = 0;
	}

	gl_FragColor = color;
}

