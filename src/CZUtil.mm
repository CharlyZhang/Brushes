
///  \file CZUtil.cpp
///  \brief This is the file implement a series of utilities.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-11
///  \note

#include "CZUtil.h"
#include "CZBezierSegment.h"
#include "Macro.h"
#if USE_OPENGL
#include "gl/glut.h"
#elif USE_OPENGL_ES
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

using namespace std;

/// 取随机数[0,1]
float CZRandomFloat()
{
	return rand()*1.0f / RAND_MAX;
}

/// 不同颜色模式的转换
void HSVtoRGB(float h, float s, float v, float &r, float &g, float &b)
{
	if (s == 0) 
	{
		r = g = b = v;
	} 
	else 
	{
		float   f,p,q,t;
		int     i;

		h *= 360;

		if (h == 360.0f) 
		{
			h = 0.0f;
		}

		h /= 60;
		i = floor(h);

		f = h - i;
		p = v * (1.0 - s);
		q = v * (1.0 - (s*f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i) {
		case 0: r = v; g = t; b = p; break;
		case 1: r = q; g = v; b = p; break;
		case 2: r = p; g = v; b = t; break;
		case 3: r = p; g = q; b = v; break;
		case 4: r = t; g = p; b = v; break;
		case 5: r = v; g = p; b = q; break;
		}
	}
}   
void RGBtoHSV(float r, float g, float b, float &h, float &s, float &v)
{
	float max = Max(r, Max(g, b));
	float min = Min(r, Min(g, b));
	float delta = max - min;

	v = max;
	s = (max != 0.0f) ? (delta / max) : 0.0f;

	if (s == 0.0f) 
	{
		h = 0.0f;
	} 
	else
	{
		if (r == max) {
			h = (g - b) / delta;
		} else if (g == max) {
			h = 2.0f + (b - r) / delta;
		} else if (b == max) {
			h = 4.0f + (r - g) / delta;
		}

		h *= 60.0f;

		if (h < 0.0f) 
		{
			h += 360.0f;
		}
	}

	h /= 360.0f;
}

/// 将一连串结点打散，相邻结点用三次贝塞尔曲线连接
/// 
///		两个结点（nodes）形成一根三次贝塞尔曲线，再将曲线打散成若干个绘制点（points）
/// 
///		/param nodes		- 连续的三维结点
///		/param points		- 离散后得到的绘制点容器
///		/return				- 离散后得到的绘制点数目
unsigned int flattenNodes2Points(const vector<CZBezierNode> &nodes, bool closed, vector<CZ3DPoint> &points)
{
	int numNodes = nodes.size();

	points.clear();

	if (numNodes == 1)
	{
		CZBezierNode lonelyNode = nodes.back();
		points.push_back(lonelyNode.anchorPoint);
		return 1;
	}

	int numSegs = closed ? numNodes : numNodes - 1;

	CZBezierSegment   *segment = NULL;
	for (int i = 0; i < numSegs; i++) 
	{
		CZBezierNode a = nodes[i];
		CZBezierNode b = nodes[(i+1) % numNodes];

		segment = CZBezierSegment::segmentBetweenNodes(a,b);
		segment->flattenIntoArray(points);
		delete segment;
	}

	return points.size();
}

/// 判断是否支持深度颜色
bool CZcanUseHDTexture()
{
	return false;
}

/// 正弦函数,由[0,1]到[0,1] -CZFreehandTool类引用
float sineCurve(float input)
{
	float result;

	input *= M_PI; // move from [0.0, 1.0] tp [0.0, Pi]
	input -= M_PI_2; // shift back onto a trough

	result = sin(input) + 1; // add 1 to put in range [0.0,2.0]
	result /= 2; // back to [0.0, 1.0];

	return result;
}

#include <iostream>

void checkPixels(int w_, int h_)
{
	float *pix = new float[w_*h_*4];
	glReadPixels(0,0,w_,h_,GL_RGBA, GL_FLOAT,pix);
	
	bool over = false;
	for(int i=0; i<h_; i++)
	{
		for(int j=0; j<w_; j++)
		{
			int ind = i*w_+j;
			if( pix[4*ind+0] != 0 ||
				pix[4*ind+1] != 0 ||
				pix[4*ind+2] != 0 ||
				pix[4*ind+3] != 1.0)
				
				cout << i <<"\t" << j << endl;
				cout << pix[4*ind+0] << "\t"
						  << pix[4*ind+1] << "\t"
						  << pix[4*ind+2] << "\t"
						  << pix[4*ind+3] << "\n";
				over =true;
				//break;
		}
		//if(over) break;
	}

	cout << "finished!\n";

	delete [] pix;
}


void CZCheckGLError_(const char *file, int line)
{
	GLenum glErr;
	int    retCode = 0;
    
	glErr = glGetError();
    
    while (glErr != GL_NO_ERROR)
	{
#if USE_OPENGL
		const GLubyte* sError = gluErrorString(glErr);

		if (sError)
			cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << endl;
		else
			cout << "GL Error #" << glErr << " (no message available)" << " in File " << file << " at line: " << line << endl;

#elif USE_OPENGL_ES
        switch (glErr) {
            case GL_INVALID_ENUM:
                NSLog(@"GL Error: Enum argument is out of range");
                break;
            case GL_INVALID_VALUE:
                NSLog(@"GL Error: Numeric value is out of range");
                break;
            case GL_INVALID_OPERATION:
                NSLog(@"GL Error: Operation illegal in current state");
                break;
                //        case GL_STACK_OVERFLOW:
                //            NSLog(@"GL Error: Command would cause a stack overflow");
                //            break;
                //        case GL_STACK_UNDERFLOW:
                //            NSLog(@"GL Error: Command would cause a stack underflow");
                //            break;
            case GL_OUT_OF_MEMORY:
                NSLog(@"GL Error: Not enough memory to execute command");
                break;
            case GL_NO_ERROR:
                if (1) {
                    NSLog(@"No GL Error");
                }
                break;
            default:
                NSLog(@"Unknown GL Error");
                break;
        }
#endif

		retCode = 1;
		glErr = glGetError();
	}
	//return retCode;
};

/// 读取json文件内容
map<string,vector<string> > & readJsonContent(const char* jsonFile)
{
	static map<string,vector<string> >content;
	content.clear();

	return content;
}
