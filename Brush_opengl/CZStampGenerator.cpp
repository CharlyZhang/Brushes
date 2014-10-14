
///  \file CZStampGenerator.cpp
///  \brief This is the file implement the Class CZStampGenerator.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2014-09-19
///  \note

#include "CZStampGenerator.h"
#include <stdlib.h>				// for rand()
#include "CZFbo.h"
#include "CZBrush.h"
#include "CZTexture.h"
#include <iostream>

#define kSmallStampSize 64
#define kBrushDimension 512		///< 笔刷图案默认大小

CZStampGenerator::CZStampGenerator()
{
	seed = rand();

	size = CZSize(kBrushDimension, kBrushDimension);

	//rawProperties = [NSMutableDictionary dictionary];
	scale = 1.0f;

	baseDimension = kBrushDimension;

	stamp = NULL;

	randomizer = NULL;

	ptrDelegate = NULL;
}

CZStampGenerator::~CZStampGenerator()
{
	if(stamp != NULL)
	{
		delete stamp;
		stamp = NULL;
	}

	if(randomizer != NULL)
	{
		delete randomizer;
		randomizer = NULL;
	}
}

/// 创建属性（虚函数）
void CZStampGenerator::buildProperties()
{
	std::cout<<" CZStampGenerator::buildProperties - Unimplemented method \n";
}

/// 绘制图案（虚函数）
void CZStampGenerator::renderStamp()
{
	std::cout<<" CZStampGenerator::renderStamp - Unimplemented method \n";
}

/// 获取笔刷图案 ~
CZImage *CZStampGenerator::getStamp(bool isSmall /* = false */)
{
	if(stamp == NULL) stamp = generateStamp();
	return stamp;
}

/// 获取随机化器
CZRandom *CZStampGenerator::getRandomizer()
{
	if(randomizer == NULL) randomizer = new CZRandom(seed);
	return randomizer;
}
/// 配置笔刷参数
void CZStampGenerator::configureBrush(CZBrush *brush)
{

	/*
	/// 默认参数值
	brush->intensity.value = 0.2f;
	brush->angle.value = 0;
	brush->spacing.value = 0.02;
	brush->rotationalScatter.value = 0.0f;
	brush->positionalScatter.value = 0.0f;
	brush->angleDynamics.value = 0.0f;
	brush->weightDynamics.value = 0.0f;
	brush->intensityDynamics.value = 0.0f;
	*/

	brush->weight.value = 30;//80;
	brush->intensity.value = 0.3;
	brush->angle.value = 0;
	brush->spacing.value = 0.02;
	brush->rotationalScatter.value =0;
	brush->positionalScatter.value =0.5;
	brush->angleDynamics.value = 1;
	brush->weightDynamics.value = 0;
	brush->intensityDynamics.value = 1;
}

/// 生成笔刷图案
CZImage *CZStampGenerator::generateStamp()
{
	float  width = size.width;
	float  height = size.height;
	float  rowByteSize = width;
	CZRect  bounds(0, 0, width, height);
	//void    *data = calloc(sizeof(UInt8), width * height);

	/*
	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceGray();
	CGContextRef context = CGBitmapContextCreate(data, width, height, 8, rowByteSize, colorspace, kCGImageAlphaNone);
	CGColorSpaceRelease(colorspace);

	// make our bitmap context the current context
	UIGraphicsPushContext(context);

	// fill black
	CGContextSetGrayFillColor(context, 0.0f, 1.0f);
	CGContextFillRect(context, bounds);

	//WDRandom *random = [[WDRandom alloc] initWithSeed:self.seed];
	*/
	CZTexture *tex = new CZTexture(width,height);
	CZFbo *fbo =  new CZFbo;
	fbo->setTexture(tex);
	CZImage *ret = new CZImage(width,height,CZImage::RGBA);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) width, 0.0f, (GLfloat) height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	fbo->begin();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	if (scale != 1.0) 
	{
		/*
		CGContextSaveGState(context);
		CGContextScaleCTM(context, self.scale, self.scale);
		[self renderStamp:context randomizer:random];
		CGContextRestoreGState(context);
		*/
	} 
	else 
	{
		renderStamp();//:context randomizer:random];

		glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, ret->data);

		fbo->end();
		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		//fbo->showTextureOnScreen(0,0,600,600);
	}

	if (blurRadius != 0) 
	{
		/*
		uint32_t kernelDimension = self.blurRadius * 2 + 1; // must be odd
		void    *outData = calloc(sizeof(UInt8), width * height);
		size_t  rowBytes = width;

		vImage_Buffer src = { data, height, width, rowBytes };
		vImage_Buffer dest = { outData, height, width, rowBytes };
		vImage_Error err;

		err = vImageTentConvolve_Planar8(&src, &dest, NULL, 0, 0, kernelDimension, kernelDimension, 0, kvImageBackgroundColorFill);

		if (err != kvImageNoError) 
		{
			// NSLog something
		}

		// put the data back
		memcpy(data, outData, width * height);
		free(outData);
		*/
	}
/*
	// get image
	CGImageRef imageRef = CGBitmapContextCreateImage(context);
	UIImage *result = [[UIImage alloc] initWithCGImage:imageRef];
	CGImageRelease(imageRef);

	UIGraphicsPopContext();
	CGContextRelease(context);
	free(data);
	*/

	//return result;

	//CZTexture *ret = new CZTexture;
	//ret->texId = fbo->getTexID();
	//delete ret;
	return ret;
}