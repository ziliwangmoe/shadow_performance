uniform sampler2D depthTex;
varying vec2 texCoor;
void main()
{
	gl_FragColor=vec4(1.0);
	gl_FragColor.rgb = texture2D(depthTex,texCoor).rgb;
}
