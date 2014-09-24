#define RENDER_BRUSH 1

uniform sampler2D tex;
#if RENDER_BRUSH  
varying float varIntensity;
#endif 
void main()  
{  	
#if RENDER_BRUSH
	float f = texture2D(tex, gl_TexCoord[0].st).a;
    float v = varIntensity * f;
    
    gl_FragColor = vec4(0, 0, 0, v); 
#else
    vec4 color = texture2D(tex,gl_TexCoord[0].st); 
	gl_FragColor = color;// * /*vec4(1,0.5,0.5,1)*/ gl_Color; 
	gl_FragColor.a = 0.8;//varIntensity;
#endif
}  