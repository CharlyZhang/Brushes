
///  \file CZBristleGenerator.cpp
///  \brief This is the file implement the Class CZBristleGenerator.
///
///		(description)
///
///  \version	1.0.0
///	 \author	Charly Zhang<chicboi@hotmail.com>
///  \date		2015-07-24
///  \note

#include "CZBristleGenerator.h"
#include "../CZUtil.h"
#include "../brush/CZBrush.h"
#include "../basic/CZ2DPoint.h"
#include "../graphic/CZGLContext.h"
#include "../graphic/glDef.h"
#include <map>
#include <string>

#define PENCIL	0
#define CRAYON	1

using namespace std;

CZBristleGenerator::CZBristleGenerator(CZGLContext *ctx,BristleGenerateType t):CZStampGenerator(ctx),type(t)
{
    bristleDensity.title = "Bristle Density";
    bristleDensity.minimumValue = 0.01f;
    
    bristleSize.title = "Bristle Size";
    bristleSize.minimumValue = 0.01f;
    
    /// set by CharlyZhang
    switch (type) {
        case kCrayonBristle:
            bristleDensity.value = 0.16;
            bristleSize.value = 0.83;
            break;
        case kPencilBristle:
            bristleDensity.value = 0.02;
            bristleSize.value = 0.7;
            break;
        default:
            break;
    }
}

/// 拷贝一份当前生成器
CZBristleGenerator* CZBristleGenerator::copy()
{
    CZBristleGenerator *ret = new CZBristleGenerator(ptrGLContext,type);
    ret->seed = seed;
    ret->size = size;
    ret->scale = scale;
    ret->bristleDensity = bristleDensity;
    ret->bristleSize = bristleSize;
    return ret;
}

/// 绘制图案
void CZBristleGenerator::renderStamp(CZRandom* randomizer)
{
    if (!randomizer)
    {
        LOG_ERROR("randomizer is null\n");
        return;
    }
    
    if(ptrGLContext == NULL)
    {
        LOG_ERROR("ptrGLContext is NULL!\n");
        return;
    }
    
    CZShader *shader = getShader("drawCircle");
    if (shader == NULL)
    {
        ptrGLContext->setAsCurrent();
        vector<string> attributes, uniforms;
        attributes.push_back("inPosition");
        uniforms.push_back("mvpMat");
        uniforms.push_back("intensity");
        shader = new CZShader("basic","basicColor",attributes,uniforms);
        
        shaders.insert(make_pair("drawCircle",shader));
    }
    
    float  width = baseDimension;
    float  height = baseDimension;
    CZRect  bounds = CZRect(0, 0, width, height);
    CZ2DPoint center = bounds.getCenter();
    
    int numBristles = bristleDensity.value * 980 + 20;
    int s = bristleSize.value * (size.width * 0.05) + 5;
    
    
    GLuint mVertexBufferObject;
    // 装载顶点
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glEnableVertexAttribArray(0);
    
    shader->begin();
    
    glUniformMatrix4fv(shader->getUniformLocation("mvpMat"),1,GL_FALSE,projMat);
    
    // make some bristles
    long maxRadius = (long)width / 2;
    
    for (int i = 0; i < numBristles; i++) {
        int radius = randomizer->nextInt() % s;
        
        float tempMax = (maxRadius - (radius + 1));
        
        float n = randomizer->nextFloat();
        
        float r = n * tempMax;
        float a = randomizer->nextFloat() * (M_PI * 2);
        float x = center.x + cos(a) * r;
        float y = center.y + sin(a) * r;
        glUniform1f(shader->getUniformLocation("intensity"),randomizer->nextFloat());
        
        CZ2DPoint center(x,y);
        drawCircle(center, radius);
        
    }
    
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /// 消除
    glDeleteBuffers(1, &mVertexBufferObject);
    
    shader->end();
    CZCheckGLError();
}

/// 绘制螺旋线
void CZBristleGenerator::drawCircle(const CZ2DPoint &center_, float radius_)
{
    int segments = radius_ * 4;
    vector<CZ2DPoint> points;
    points.push_back(center_);
    
    for (int i = 0; i <= segments; i++)
    {
        float x = center_.x + radius_*cos(2*M_PI/segments*i);
        float y = center_.y + radius_*sin(2*M_PI/segments*i);

        CZ2DPoint P(x, y);
        points.push_back(P);
    }
    
    int n = points.size();

    glBufferData(GL_ARRAY_BUFFER, sizeof(CZ2DPoint)*n, &points[0].x, GL_STREAM_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CZ2DPoint),0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, n);

    CZCheckGLError();
    
    points.clear();
}

/// 配置笔刷参数
void CZBristleGenerator::configureBrush(CZBrush *brush)
{
    if(!brush)
    {
        LOG_ERROR("brush is null\n");
        return;
    }
    
    brush->weight.value = 10;//80;
    
    switch (type) {
        case kCrayonBristle:
            brush->intensity.value = 0.87;
            brush->angle.value = 164;
            brush->spacing.value = 0.06;
            brush->rotationalScatter.value =0.79;
            brush->positionalScatter.value =0.03;
            brush->angleDynamics.value = 0.05;
            brush->weightDynamics.value = 0.41;
            brush->intensityDynamics.value = 0.07;
            break;
        case kPencilBristle:
            brush->intensity.value = 1.0f;
            brush->angle.value = 142;
            brush->spacing.value = 0.0f;
            brush->rotationalScatter.value =0.46;
            brush->positionalScatter.value =0.8;
            brush->angleDynamics.value = 1.0f;
            brush->weightDynamics.value = -0.82;
            brush->intensityDynamics.value = -0.77f;
            break;
            
        default:
            break;
    }
    
}