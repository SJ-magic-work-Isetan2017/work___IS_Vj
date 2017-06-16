#version 120
#extension GL_ARB_texture_rectangle: enable

uniform vec2 screenCenter = vec2( 1280.0, 720.0 ) / 2.0;
uniform sampler2DRect inputTexture;

/******************************
******************************/
void main(){
	vec2 texCoord = gl_TexCoord[0].xy;
	
	if(screenCenter.y < texCoord.y){
		texCoord.y = screenCenter.y * 2 - texCoord.y;
	}
	
	vec4 color = texture2DRect( inputTexture, texCoord );
	// color.a = 1.0;
	
	gl_FragColor = color;
}
