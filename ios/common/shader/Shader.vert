//
//  Shader.vsh
//  learn5
//
//  Created by CharlyZhang on 14-11-6.
//  Copyright (c) 2014年 CharlyZhang. All rights reserved.
//
precision highp float;

attribute vec4 position;
attribute vec2 texcoord;
varying vec2 varTexcoord;

uniform mat4 modelViewProjectionMatrix;

void main()
{
    varTexcoord = texcoord;
    gl_Position = modelViewProjectionMatrix * position;
}
