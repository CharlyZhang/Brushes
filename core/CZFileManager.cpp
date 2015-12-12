//
//  CZFileManager.cpp
//  BrushesCore
//
//  Created by CharlyZhang on 15/11/24.
//  Copyright © 2015年 CharlyZhang. All rights reserved.
//

#include "CZFileManager.h"
#include "CZDefine.h"
#include "CZPainting.h"

using namespace std;

CZFileManager::CZFileManager()
{
    directoryPath = ".";
}

CZFileManager::~CZFileManager()
{
}


void CZFileManager::setDirectoryPath(const char* pathStr)
{
    if (pathStr == nullptr)
    {
        LOG_ERROR("pathStr is NULL\n");
        return;
    }
    
    directoryPath = string(pathStr);
}

bool CZFileManager::savePainting(CZPainting *painting, const char * filenameStr)
{
    if (painting == nullptr)
    {
        LOG_ERROR("painting is NULL\n");
        return false;
    }
    
    if (filenameStr == nullptr)
    {
        LOG_ERROR("filenameStr is NULL\n");
        return nullptr;
    }
    
    // open file
    string file = directoryPath + "/" + string(filenameStr);
    FILE *fp = fopen(file.c_str(), "wb");
    
    if (fp == NULL)
    {
        LOG_ERROR("file open failed\n");
        return false;
    }
    
    int activeLayerInd = painting->getActiveLayerIndex();
    fwrite((void*)&activeLayerInd, sizeof(int), 1, fp);
    CZSize size = painting->getDimensions();
    fwrite((void*)&(size.width), sizeof(float), 1, fp);
    fwrite((void*)&(size.height), sizeof(float), 1, fp);
    
    // layers
    int layersNum = painting->getLayersNumber();
    fwrite((void*)&layersNum, sizeof(int), 1, fp);
    for (int i = 0; i <  layersNum; i++ )
    {
        CZLayer *layer = painting->getLayer(i);
        
        bool temp = layer->isVisible();
        fwrite((void*)&temp, sizeof(bool), 1, fp);
        temp = layer->isLocked();
        fwrite((void*)&temp, sizeof(bool), 1, fp);
        
        float opacity = layer->getOpacity();
        fwrite((void*)&opacity, sizeof(float), 1, fp);
        
        // image
        CZImage *img = layer->imageData();
        img->saveToFile(fp);
        delete img;
    }
    
    fclose(fp);
    
    return true;
}

CZPainting* CZFileManager::createPainting(const char *filenameStr)
{
    if (filenameStr == nullptr)
    {
        LOG_ERROR("filenameStr is NULL\n");
        return nullptr;
    }
    
    // open file
    string file = directoryPath + "/" + string(filenameStr);
    
    return createPaintingWithURL(file.c_str());
}

CZPainting* CZFileManager::createPaintingWithURL(const char* urlStr)
{
    if (urlStr == nullptr)
    {
        LOG_ERROR("urlStr is NULL\n");
        return nullptr;
    }
    
    // open file
    FILE *fp = fopen(urlStr, "rb");
    
    if (fp == NULL)
    {
        LOG_WARN("file open failed\n");
        return nullptr;
    }
    
    int activeLayerInd;
    float width,height;
    fread((void*)&activeLayerInd, sizeof(int), 1, fp);
    fread((void*)&width, sizeof(float), 1, fp);
    fread((void*)&height, sizeof(float), 1, fp);
    
    CZPainting *newPainting = new CZPainting(CZSize(width,height));
    
    // layers
    int layersNum;
    fread((void*)&layersNum, sizeof(int), 1, fp);
    for (int i = 0; i <  layersNum; i++ )
    {
        newPainting->addNewLayer();
        CZLayer *layer = newPainting->getActiveLayer();
        
        bool temp;
        fread((void*)&temp, sizeof(bool), 1, fp);
        layer->setVisiblility(temp);
        fread((void*)&temp, sizeof(bool), 1, fp);
        layer->setLocked(temp);
        
        float opacity;
        fread((void*)&opacity, sizeof(float), 1, fp);
        layer->setOpacity(opacity);
        
        // image
        CZImage *img = CZImage::createFromFile(fp);
        layer->setImage(img);
        delete img;
    }
    
    fclose(fp);
    
    return newPainting;
}

