uniform float lifespan;

varying vec3 ecPosition3;
varying vec3 eye;
varying vec3 color;
varying float age;

//TODO: near and far clip planes


void main(){
	float depthVal = 1. - pow(gl_FragCoord.z, 50.);
	gl_FragColor = vec4( color, 1.  ) * depthVal; //gl_Color;
}