uniform float lifespan;

uniform sampler2DRect map;
uniform vec2 mapDim;

varying vec3 ecPosition3;
varying vec3 eye;
varying vec4 color;
varying float age;
varying vec2 uv;

//TODO: near and far clip planes


void main(){
	vec4 texCol =  vec4( gl_TexCoord[0].st , 1., 1.);// texture2DRect( map, mapDim * vec2(facingRatio);
	float depthVal = 1. - pow(gl_FragCoord.z, 50.);
	gl_FragColor = texCol;// color * texCol * depthVal; //gl_Color;
}