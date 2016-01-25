const vec3 BrickColor  = vec3(1.0,0.3,0.2);
const vec3 MortarColor = vec3(0.85,0.86,0.84);
const vec2 BrickSize   = vec2(0.30,0.15);
const vec2 BrickPct    = vec2(0.90,0.85);

varying vec2  ModelPos;

varying vec3 testData;
varying vec3 lightDirT;
varying vec3 eyeDirT;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform float time;
uniform float radius;


void main(){
	// shift every two row by 0.5
	if(fract((ModelPos.y)*0.5)>0.5){
		ModelPos.x += 0.5;
	}

	//get the position relative to corresponding gird points.
	vec2 coor = fract(ModelPos);
	coor = coor - vec2(0.5);
	//compare coor to gird points(0.5, 0.5)
	float d = coor.x*coor.x + coor.y*coor.y;
	// using step function to get ride of branching, maybe I need to compare the efficiency of this two versions later.
	float bitMask = step(radius,d);
	vec3 colorDot = mix(BrickColor, MortarColor, bitMask);
	
	float f = inversesqrt(d+1.0);
	if (bitMask>=0.5){
		coor = vec2(0.0);
		f=1.0;
	}
	vec3 N = vec3(coor.x,coor.y,1.0)*f;
	vec3 L = lightDirT;
	vec3 R = reflect(-L, N);
	vec3 V = eyeDirT;
	float Id = max(dot(L,N) , 0.0);
	float Is = (Id>0.0) ? pow(max(dot(R,V) , 0.0) , shininess) : 0.0;
	vec3 colorDump = ambient + Id*diffuse + Is*specular;
	float LightIntensity = length(colorDump);

	vec3 color = colorDot*LightIntensity;

	gl_FragColor = vec4(color,1.0);
	//gl_FragColor = vec4(testData,1.0);
	//gl_FragColor = vec4(1.0, 1.0, 1.0,1.0);
}
