//  Brick fragment shader
//  Derived from Orange Book Chapter 6 by Charles Gruenwald

//  Hardwire brick dimensions and color
const vec3 BrickColor  = vec3(1.0,0.3,0.2);
const vec3 MortarColor = vec3(0.85,0.86,0.84);
const vec2 BrickSize   = vec2(0.30,0.15);
const vec2 BrickPct    = vec2(0.90,0.85);

//  Model coordinates and light from vertex shader
varying float LightIntensity;
varying vec2  ModelPos;
uniform float time;
uniform float radius;
varying vec3 testData;

uniform int isAntiAlias;

float getMask(float x, float y){
	vec2 ModelPos = vec2(x, y);
	//get the position relative to corresponding gird points.
	vec2 coor = fract(ModelPos);
	coor = coor - vec2(0.5);
	//compare coor to gird points(0.5, 0.5)
	float d = coor.x*coor.x + coor.y*coor.y;
	// using step function to get ride of branching, maybe I need to compare the efficiency of this two versions later.
	return step(radius,d);
}

void main(){
	// shift every two row by 0.5
	float filterW = fwidth(ModelPos)*0.5;
	if(isAntiAlias==0){
		filterW = 0;
	}
	//float filterW = 0.02;
	if(fract((ModelPos.y)*0.5)>0.5){
		ModelPos.x += 0.5;
	}
	

	float bitMask= (getMask(ModelPos.x+filterW, ModelPos.y)+ getMask(ModelPos.x-filterW, ModelPos.y)+ getMask(ModelPos.x, ModelPos.y)+
	getMask(ModelPos.x+filterW, ModelPos.y+filterW)+ getMask(ModelPos.x-filterW, ModelPos.y+filterW)+ getMask(ModelPos.x, ModelPos.y+filterW)+
	getMask(ModelPos.x+filterW, ModelPos.y-filterW)+ getMask(ModelPos.x-filterW, ModelPos.y-filterW)+ getMask(ModelPos.x, ModelPos.y-filterW)
	)/9;

	vec3 color = mix(BrickColor, MortarColor, bitMask);
   color *= LightIntensity;
   //  Pad color with alpha
   gl_FragColor = vec4(color,1.0);
   //gl_FragColor = vec4(testData,1.0);
   //gl_FragColor = vec4(1.0, 1.0, 1.0,1.0);
}
