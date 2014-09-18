
//attribute float alpha;
//varying float varIntensity;

void main(void) 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;  
	gl_FrontColor = gl_Color;
    //gl_Position = ftransform();  
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; 
    //varIntensity = alpha;
}
