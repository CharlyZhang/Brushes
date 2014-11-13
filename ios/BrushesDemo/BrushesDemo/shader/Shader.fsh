//
//  Shader.fsh
//  learn5
//
//  Created by CharlyZhang on 14-11-6.
//  Copyright (c) 2014年 CharlyZhang. All rights reserved.
//
precision highp float;

varying vec2 varTexcoord;
uniform sampler2D texture;

void main()
{
    gl_FragColor = texture2D(texture, varTexcoord.st, 0.0);
}
