/************************************************************
ビルトイン関数(一部)
	http://qiita.com/edo_m18/items/71f6064f3355be7e4f45
************************************************************/
#version 120
#extension GL_ARB_texture_rectangle: enable

/************************************************************
************************************************************/
uniform vec2 screenSize = vec2(1280.0, 720.0);
uniform int NumSplit = 3;

uniform sampler2DRect inputTexture;

/************************************************************
************************************************************/

/******************************
******************************/
void main(){
	vec2 texCoord = gl_TexCoord[0].xy;
	
	
	float x = mod(texCoord.x, (screenSize.x / NumSplit));
	float y = mod(texCoord.y, (screenSize.y / NumSplit));
	
	vec4 color = texture2DRect( inputTexture, vec2(x * NumSplit, y * NumSplit) );
	// color.a = 1.0;
	
	gl_FragColor = color;
}

