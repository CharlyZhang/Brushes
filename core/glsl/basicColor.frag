#ifdef GL_ES
precision highp float;
#endif

uniform float intensity;

void main (void)
{
	float i = clamp(intensity, 0.0f, 1.0f);
    gl_FragColor = vec4(i,i,i,i);
}
