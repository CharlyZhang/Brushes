#define RENDER_BRUSH 1 

#if RENDER_BRUSH
attribute float alpha;
varying float varIntensity;
#endif

void main(void) 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;  
	gl_FrontColor = gl_Color;
    //gl_Position = ftransform();  
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; 
#if RENDER_BRUSH    
	varIntensity = alpha;
#endif
}
