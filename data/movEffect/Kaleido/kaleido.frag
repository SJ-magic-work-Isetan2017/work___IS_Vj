#version 120
#extension GL_ARB_texture_rectangle: enable

#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)

uniform int ksectors = 10;
uniform float kangleRad = 0.0;
uniform vec2 kcenter = vec2( 1280.0, 720.0 ) / 2.0;
uniform vec2 screenCenter = vec2( 1280.0, 720.0 ) / 2.0;

uniform sampler2DRect inputTexture;
uniform float alpha = 1.0;

/******************************
******************************/
void main(){
	vec2 texCoord = gl_TexCoord[0].xy;
	vec2 v = texCoord - screenCenter;
	float r = length( v );
	
	/********************
	atan range : -pi - pi
	********************/
	float a = atan( v.y, v.x );
	
	/********************
	ビルトイン関数(一部)
		http://qiita.com/edo_m18/items/71f6064f3355be7e4f45
	
		floor(x)	x以下の最大の整数を返す
		mod(x, y)	x-y*floor(x/y)
	********************/
	float A = TWO_PI / float(ksectors);
	a = mod( a, A );
	if ( a > A/2.0 ) a = A - a;
	a -= kangleRad;
	
	vec2 u = vec2( cos(a), sin(a) ) * r;
	u += kcenter;
	gl_FragColor = texture2DRect( inputTexture, u ) * alpha;
}
