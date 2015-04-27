#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec2      outPosition;
out vec4     fragColor;
#else
varying vec2 outPosition;
#endif

uniform float hardness;

void main (void)
{
	float h = clamp(hardness, 0.0f, 1.0f);	
	float factor = length(outPosition);

	if (factor <= h)				gl_FragColor = vec4(1,1,1,1);
	else if(factor <= 1.0f)			gl_FragColor = vec4(1,1,1,1) * (1-factor)/(1-h);
    else							gl_FragColor = vec4(0,0,0,0) ;

}
