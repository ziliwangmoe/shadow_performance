//  Model coordinates and light from vertex shader
varying float LightIntensity;
varying vec2 tex_f;
varying vec2  ModelPos;
uniform float time;
varying vec3 testData;

float rand(vec2 co, float seed){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233)*seed)) * 43758.5453);
}

float randColor(vec2 ModelPos, float seed){
	vec2 bl= floor(ModelPos);
	vec2 br= vec2(bl.x+1.0,bl.y);
	vec2 tl= vec2(bl.x,bl.y+1.0);
	vec2 tr= vec2(bl.x+1.0,bl.y+1.0);
	float rbl = rand(bl, seed);
	float rbr = rand(br, seed);
	float rtl = rand(tl, seed);
	float rtr = rand(tr, seed);
	vec2 interp = fract(ModelPos);
	float ampx1 = mix(rbl,rbr,interp.x);
	float ampx2 = mix(rtl,rtr,interp.x);
	return mix(ampx1,ampx2,interp.y);
}

void main(){
	vec3 amp= vec3(randColor(ModelPos, 1.1),randColor(ModelPos, 1.0),randColor(ModelPos, 0.9));
	vec3 color = amp*LightIntensity;
	gl_FragColor = vec4(color,1.0);
}
