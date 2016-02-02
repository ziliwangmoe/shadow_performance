varying vec2 pos;
uniform float x_thres;
uniform float y_thres;
uniform int isInline;
uniform int isInt;
uniform int iterCount;

float func_float(float x, float y, float z){
	return (x+y+z)/3.0;
}

int func_int(int x, int y, int z){
	return (x+y+z)/3;
}

void main()
{
	float re_f;
	int re_i;

	int input_i_x =1; int input_i_y=1; int input_i_z = 1;
	float input_f_x =1.0; float input_f_y=1.0; float input_f_z = 1.0;
	//if (true){
	if (pos.x < x_thres && pos.y < y_thres){
		for(int i=0; i<iterCount;i++){
				if(isInt==1){
					if(isInline == 1){
						re_i = (input_i_x+ input_i_y+ input_i_z)/6;
						gl_FragColor=vec4(0.0, re_i, 0.0, 1.0); //black
					}else{
						re_i = func_int(input_i_x,input_i_y,input_i_z);
						gl_FragColor=vec4(0.0, 0.0, re_i, 1.0); // blue
					}
					
				}else{
					if(isInline == 1){
						re_f = (input_f_x+ input_f_y+ input_f_z)/6.0;
						gl_FragColor=vec4(0.0, re_f, 0.0, 1.0); //green
					}else{
						re_f = func_float(input_f_x,input_f_y,input_f_z);
						gl_FragColor=vec4(re_f, 0.0, 0.0, 1.0); // red
					}
					
				}
		}
	}
	else{
		gl_FragColor=vec4(1.0, 1.0, 1.0, 1.0); //white
	}
	
}
