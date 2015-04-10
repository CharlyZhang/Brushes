
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
#include "path/CZBezierSegment.h"
#include "basic/CZ3DPoint.h"
#include "CZDefine.h"
#include "graphic/glDef.h"

using namespace std;

/// 不同颜色模式的转换
void CZUtil::HSVtoRGB(float h, float s, float v, float &r, float &g, float &b)
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
void CZUtil::RGBtoHSV(float r, float g, float b, float &h, float &s, float &v)
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

/// 生UUID
char* CZUtil::generateUUID()
{
	char *buf = new char[37];

	const char *c = "89ab";
	char *p = buf;
	int n;
	for( n = 0; n < 16; ++n )
	{
		int b = rand()%255;
		switch( n )
		{
		case 6:
			sprintf(p, "4%x", b%15 );
			break;
		case 8:
			sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
			break;
		default:
			sprintf(p, "%02x", b);
			break;
		}

		p += 2;
		switch( n )
		{
		case 3:
		case 5:
		case 7:
		case 9:
			*p++ = '-';
			break;
		}
	}
	*p = 0;
	return buf;
}

/// 将一连串结点打散，相邻结点用三次贝塞尔曲线连接
/// 
///		两个结点（nodes）形成一根三次贝塞尔曲线，再将曲线打散成若干个绘制点（points）
/// 
///		/param nodes		- 连续的三维结点
///		/param points		- 离散后得到的绘制点容器
///		/return				- 离散后得到的绘制点数目
unsigned int CZUtil::flattenNodes2Points(const vector<CZBezierNode> &nodes, bool closed, vector<CZ3DPoint> &points)
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
bool CZUtil::canUseHDTexture()
{
	return true;
}

void CZUtil::CZCheckGLError_(const char *file, int line)
{
	int    retCode = 0;
	GLenum glErr = glGetError();

	while (glErr != GL_NO_ERROR) 
	{
		const GLubyte* sError = gluErrorString(glErr);

		if (sError)
			LOG_INFO("GL Error #%d (%s) in File %s at line: %d\n",glErr,gluErrorString(glErr),file,line);
		else
			LOG_INFO("GL Error #%d (no message available) in File %s at line: %d\n",glErr,file,line);
		retCode = 1;
		glErr = glGetError();
	}
	//return retCode;
};

/// 正弦函数,由[0,1]到[0,1] -CZFreehandTool类引用
float CZUtil::sineCurve(float input)
{
	float result;

	input *= M_PI; // move from [0.0, 1.0] tp [0.0, Pi]
	input -= M_PI_2; // shift back onto a trough

	result = sin(input) + 1; // add 1 to put in range [0.0,2.0]
	result /= 2; // back to [0.0, 1.0];

	return result;
}


void CZUtil::checkPixels(int w_, int h_)
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

				LOG_INFO("(%d,%d):%f\t%f\t%f\t%f\n",i,j,pix[4*ind+0],pix[4*ind+1],
				pix[4*ind+2],pix[4*ind+3]);
			over =true;
			//break;
		}
		//if(over) break;
	}

	LOG_INFO("finished!\n");

	delete [] pix;
}


