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


void main(){
	// shift every two row by 0.5
	if(fract((ModelPos.y-0.5)*0.5)>0.5){
		ModelPos.x += 0.5;
	}

	//get the position relative to corresponding gird points.
	vec2 coor = fract(ModelPos);
	//compare coor to gird points(0, 0)
	float xx = coor.x*coor.x;
	float yy = coor.y*coor.y;
	float xxInv = (1-coor.x)*(1-coor.x);
	float yyInv = (1-coor.y)*(1-coor.y);
	// using step function to get ride of branching, maybe I need to compare the efficiency of this two versions later.
	float bitMask = step(radius,xx+yy)*step(radius,xxInv+yyInv)*step(radius,xxInv+yy)*step(radius,xx+yyInv);

	vec3 color = mix(BrickColor, MortarColor, bitMask);
   color *= LightIntensity;
   //  Pad color with alpha
   gl_FragColor = vec4(color,1.0);
   //gl_FragColor = vec4(testData,1.0);
   //gl_FragColor = vec4(1.0, 1.0, 1.0,1.0);
}
