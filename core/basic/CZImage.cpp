
///  \file CZTexture.cpp
///  \brief This is the file implement the Class CZImage.
///
///		This file includes CZImage.
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-10-09
///  \note

#include "CZImage.h"
#include <string.h>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

typedef struct Position {int x,y; Position(int x_, int y_) {x = x_; y = y_;}} Position;

CZImage::CZImage(int w_/* =0 */, int h_/* =0 */, StorageMode mode_ /* = DEFAULT_STORAGEMODE */, void* data_ /*= NULL*/)
{
	width = w_;
	height = h_;
	mode = mode_;

	int n, type = 0;
	switch (mode)
	{
	case RGB_BYTE:		n = 3;	type = 0;	break;
	case RGB_FLOAT:		n = 3;	type = 1;	break;
	case RGBA_BYTE:		n = 4;	type = 0;	break;
	case RGBA_FLOAT:	n = 4;	type = 1;	break;
	default:
		LOG_ERROR("ImageMode is illegal!\n");
		n = 0;
	}

	if(type == 0)
	{
		data = (void*) new unsigned char[n*width*height];
		if(data_)	memcpy(data,data_,width*height*n*sizeof(unsigned char));
	}
	else if(type ==1)
	{
		data = (void*) new float[n*width*height];
		if(data_)	memcpy(data,data_,width*height*n*sizeof(float));
	}
	else
		data = NULL;
}

CZImage::~CZImage()
{
	if(data != NULL)
	{
		delete [] data;
		data = NULL;
	}
}

void CZImage::ScanLineFill(int x,int y, float r, float g, float b, float a)

{   
	int xl,xr,i;

	bool spanNeedFill;

	stack<Position> Stack;
	float compareColor[4],fillColor[4];
	getColorAt(x,y,compareColor);
	fillColor[0] = r; fillColor[1] = g; fillColor[2] = b; fillColor[3] = a;

	Stack.push(Position(x,y));

	while(!Stack.empty())
	{ 
		Position pos = Stack.top();
		Stack.pop();

		x = pos.x;	y = pos.y;
		while(x < width && needsModify(x,y,compareColor)) //向右填充
		{ 
			modifyData(x,y,fillColor);
			x++;
		}

		xr = x-1;

		x = pos.x-1;
		while(x >= 0 && needsModify(x,y,compareColor)) //向左填充
		{ 
			modifyData(x,y,fillColor);
			x--;
		}

		xl = x+1;

		//处理上面一条扫描线
		x = xl;	y = y+1;
		while(y<height && x<=xr)
		{  
			spanNeedFill=false;
			while(x < xr && needsModify(x,y,compareColor))
			{ 
				spanNeedFill=true;
				x++;
			}

			if(spanNeedFill)
			{
				if(needsModify(x,y,compareColor) && x==xr)	Stack.push(Position(x,y));
				else										Stack.push(Position(x-1,y));
				spanNeedFill=false;
			}

			int xspan = 0;
			while(xspan+x<=xr)
			while(x<=xr && !needsModify(x,y,compareColor)) x++;
		}//End of while(i<xr)

		//处理下面一条扫描线，代码与处理上面一条扫描线类似
		x = xl;	y = y-2;
		while(y>=0 && x<=xr)
		{
			spanNeedFill=false;
			while(x < xr && needsModify(x,y,compareColor))
			{ 
				spanNeedFill=true;
				x++;
			}

			if(spanNeedFill)
			{
				if(needsModify(x,y,compareColor) && x==xr)	Stack.push(Position(x,y));
				else										Stack.push(Position(x-1,y));
				spanNeedFill=false;
			}

			while(x<=xr && needsModify(x,y,compareColor)) x++;
		}//End of while(i<xr)

	}//End of while(!isstackempty())

}

/// 填充（采用广搜）
bool CZImage::modifyDataFrom(int x,int y, float r, float g, float b, float a)
{
	//ScanLineFill(x,y,r,g,b,a);
	//return true;
	queue<Position> myQueue;
	float compareColor[4],fillColor[4];
	getColorAt(x,y,compareColor);
	fillColor[0] = r; fillColor[1] = g; fillColor[2] = b; fillColor[3] = a;

	isSearched = new bool[width*height];
	memset(isSearched,0,width*height*sizeof(bool));

	for(int i=0; i<width; i++)
		for(int j=0; j<height; j++)
		{
			if (needsModify(i,j,compareColor)) isSearched[j*width+i] = true;
		}

	myQueue.push(Position(x,y));
	modifyData(x,y,fillColor);
	isSearched[y*width+x] = false;
	static long step = 0;
	while(!myQueue.empty())
	{
		step ++;
		Position pos = myQueue.front();	myQueue.pop();
	
		for(int i=pos.x+1; i<width; i++)	
			if(isSearched[pos.y*width+i]) 
			{
				modifyData(i,pos.y,fillColor);
				isSearched[pos.y*width+i] = false;
				myQueue.push(Position(i,pos.y));
			}
			else break;
		for(int i=pos.x-1; i>=0;	i--)
			if(isSearched[pos.y*width+i])
			{
				modifyData(i,pos.y,fillColor);
				isSearched[pos.y*width+i] = false;
				myQueue.push(Position(i,pos.y));
			}
			else break;
		for(int j=pos.y+1; j<height;j++)
			if(isSearched[j*width+pos.x])
			{
				modifyData(pos.x,j,fillColor);
				isSearched[j*width+pos.x] = false;
				myQueue.push(Position(pos.x,j));
			}
			else break;
		for(int j=pos.y-1; j>=0;j--)
			if(isSearched[j*width+pos.x])
			{
				modifyData(pos.x,j,fillColor);
				isSearched[j*width+pos.x] = false;
				myQueue.push(Position(pos.x,j));
			}
			else break;
	}

	printf("total steps %ld\n",step);
	delete [] isSearched;
	return true;
}

bool CZImage::modifyDataFrom1(int x,int y, float r, float g, float b, float a)
{
	//ScanLineFill(x,y,r,g,b,a);
	//return true;
	queue<Position> myQueue;
	float compareColor[4],fillColor[4];
	getColorAt(x,y,compareColor);
	fillColor[0] = r; fillColor[1] = g; fillColor[2] = b; fillColor[3] = a;

	isSearched = new bool[width*height];
	memset(isSearched,0,width*height*sizeof(bool));

	myQueue.push(Position(x,y));
	modifyData(x,y,fillColor);
	isSearched[y*width+x] = true;
	static long step = 0;
	while(!myQueue.empty())
	{
		step ++;
		Position pos = myQueue.front();	myQueue.pop();

		for(int i=pos.x+1; i<width; i++)	
			if(!isSearched[pos.y*width+i] && needsModify(i,pos.y,compareColor)) 
			{
				modifyData(i,pos.y,fillColor);
				isSearched[pos.y*width+i] = true;
				myQueue.push(Position(i,pos.y));
			}
			else break;
			for(int i=pos.x-1; i>=0;	i--)
				if(!isSearched[pos.y*width+i] && needsModify(i,pos.y,compareColor))
				{
					modifyData(i,pos.y,fillColor);
					isSearched[pos.y*width+i] = true;
					myQueue.push(Position(i,pos.y));
				}
				else break;
				for(int j=pos.y+1; j<height;j++)
					if(!isSearched[j*width+pos.x] && needsModify(pos.x,j,compareColor))
					{
						modifyData(pos.x,j,fillColor);
						isSearched[j*width+pos.x] = true;
						myQueue.push(Position(pos.x,j));
					}
					else break;
					for(int j=pos.y-1; j>=0;j--)
						if(!isSearched[j*width+pos.x] && needsModify(pos.x,j,compareColor))
						{
							modifyData(pos.x,j,fillColor);
							isSearched[j*width+pos.x] = true;
							myQueue.push(Position(pos.x,j));
						}
						else break;
	}

	printf("total steps %ld\n",step);
	delete [] isSearched;
	return true;
}

/// 修改
void CZImage::modifyData(int x,int y, float fillcolor[])
{
	float			*f_data = NULL;
	unsigned char	*b_data = NULL;

	switch (mode)
	{
	case RGB_BYTE:	
		b_data = &((unsigned char*)data)[(y*width+x)*3];
		for(int i=0; i<3; i++) b_data[i] = (unsigned char)(fillcolor[i]*256);
		break;
	case RGB_FLOAT:
		f_data = &((float*)data)[(y*width+x)*3];
		for(int i=0; i<3; i++) f_data[i] = fillcolor[i];
		break;
	case RGBA_BYTE:		
		b_data = &((unsigned char*)data)[(y*width+x)*4];
		for(int i=0; i<4; i++) b_data[i] = (unsigned char)(fillcolor[i]*256);
		break;
	case RGBA_FLOAT:
		f_data = &((float*)data)[(y*width+x)*4];
		for(int i=0; i<4; i++) f_data[i] = fillcolor[i];
		break;
	default:
		LOG_ERROR("ImageMode is illegal!\n");
	}
}

static float	epslon = 1.0f / 256;
/// 是否需要修改
bool CZImage::needsModify(int x,int y, float compareColor[])
{
	float	color[4];
	
	getColorAt(x,y,color);

	for(int i=0; i<3; i++)						if(fabs(color[i]-compareColor[i]) >= epslon) return false;
	if(mode == RGBA_FLOAT || mode == RGBA_BYTE) if(fabs(color[3]-compareColor[3]) >= epslon) return false;
	return true;
}

/// 获取某点颜色
bool CZImage::getColorAt(int x, int y, float color[])
{
	float			*f_data = NULL;
	unsigned char	*b_data = NULL;

	switch (mode)
	{
	case RGB_BYTE:	
		b_data = &((unsigned char*)data)[(y*width+x)*3];
		for(int i=0; i<3; i++)	color[i] = b_data[i] / 256.0f;
		color[3] = 0.0f;
		break;
	case RGB_FLOAT:
		f_data = &((float*)data)[(y*width+x)*3];
		for(int i=0; i<3; i++)	color[i] = f_data[i];
		color[3] = 0.0f;
		break;
	case RGBA_BYTE:		
		b_data = &((unsigned char*)data)[(y*width+x)*4];
		for(int i=0; i<4; i++)	color[i] = b_data[i] / 256.0f;
		break;
	case RGBA_FLOAT:
		f_data = &((float*)data)[(y*width+x)*4];
		for(int i=0; i<4; i++)	color[i] = f_data[i];
		break;
	default:
		LOG_ERROR("ImageMode is illegal!\n");
		return false;
	}

	return true;
}