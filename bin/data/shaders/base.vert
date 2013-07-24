#version 120
uniform sampler2DRect map;
uniform vec4 startColor;
uniform vec4 endColor;

uniform float maxPointSize = 15.;
uniform float time = 0.;
uniform float frameRate = .016;
uniform float size = 3.;

uniform vec3 gravity = vec3( 0., -98., 0. );
uniform vec3 cameraPosition;

varying vec3 ecPosition3;
varying vec3 eye;
varying vec4 color;
varying float age;
varying vec2 uv;

float PI = 3.14159265359;
float HALF_PI = 1.57079632679;


void main(){
	
	
//	gl_TexCoord[0] = gl_MultiTexCoord0;
	float birthTime = gl_MultiTexCoord0.x;
	float lifeSpan = gl_MultiTexCoord0.y;

	
//	age = time - birthTime / lifeSpan;
	float ageSec = (time - birthTime);
	age = min( 1., ageSec / lifeSpan );
	
	vec3 vel = gl_Normal.xyz;
	vec3 posOffset = vel * sin( age * age * HALF_PI );
//	posOffset += gravity * ageSec;
	
	vec4 pos = gl_Vertex + vec4(posOffset, 0.);
	
	vec4  ecPosition = gl_ModelViewMatrix * pos;
	ecPosition3 = ecPosition.xyz / ecPosition.w;
	
	gl_Position = gl_ProjectionMatrix * ecPosition;
	
	eye = -normalize( ecPosition3 );
	
	float attenuation = 500./distance(pos.xyz, cameraPosition);
	gl_PointSize = min( maxPointSize, size * attenuation * (1. - age) );
	
	color = mix( startColor, endColor, age );
	
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
