
uniform sampler2D tex;  
varying float varIntensity;
 
void main()  
{  
    vec4 color = texture2D(tex,gl_TexCoord[0].st); 
    gl_FragColor = color * /*vec4(1,0.5,0.5,1)*/ gl_Color; 
	gl_FragColor.a = varIntensity;
	
	//if(gl_Color.a >= 0.9) gl_FragColor = vec4(0.0,1.0 ,0, 1);
	//else					gl_FragColor = vec4(1.0,0.0 ,0, 1);

	//float f = texture2D(texture, varTexcoord.st, 0.0).a;
    //float v = varIntensity * f;
    
    //gl_FragColor = vec4(1, 0, 0, 1); 
}  