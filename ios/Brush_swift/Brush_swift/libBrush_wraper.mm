//
//  libBrush_wraper.m
//  Brush_swift
//
//  Created by CharlyZhang on 15-7-2.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

#import "libBrush_wraper.h"
#import "Brush_swift-Swift.h"

#include "stamp/CZSpiralGenerator.h"
#include "graphic/CZTexture.h"
#include "basic/CZImage.h"
#include "CZActiveState.h"
#include "painting/CZPainting.h"
#include "tool/CZFreehandTool.h"
#include "brush/CZBrush.h"
#include "basic/CZMat4.h"


#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#define SHOW_PIC_TEX        0
#define SHOW_STAMP_TEX      0
#define SHOW_PREVIEW_TEX    0
#define SHOW_FREE_DRAW      1

// const
GLfloat gVertexData[20] =
{
    // positionX, positionY, positionZ,     u,v
    0.0f, 50.0f, 0,        0.0f, 1.0f,
    0.0f, 0.0f, 0,         0.0f, 0.0f,
    50.0f, 0.0f, 0,        1.0f, 0.0f,
    50.0f, 50.0f, 0,       1.0f, 1.0f
};

// Uniform index.
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    TEXTURE,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_TEXCOORD,
    NUM_ATTRIBUTES
};

///////////////

@interface libBrush_wraper ()
{
    CZTool *freehand;
    CZPainting *painting;
    CZMat4 proj;
    EAGLContext* context;
    
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
    GLuint viewRenderbuffer, viewFramebuffer;
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    GLuint _program;
    
    GLuint textures[10];
    
    CZTexture *stampTex;
    CZImage *screenImg;
}

@end

@implementation libBrush_wraper

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super init]) {
        self.size = CGSizeMake(768, 1024); //frame.size;
        
        painting = new CZPainting(CZSize(self.size.width, self.size.height));
        freehand = CZActiveState::getInstance()->getActiveTool();
        freehand->ptrPainting = painting;
        
        proj.SetOrtho(0,self.size.width, 0, self.size.height, -1.0f, 1.0f);
        context = (__bridge EAGLContext *) painting->getGLContext()->getRealContext();
        [self checkGLError:NO];
        
        ////////////////
        NSLog(@"sandbox path is:%@",NSHomeDirectory());
        stampTex = NULL;
        screenImg = NULL;
        [EAGLContext setCurrentContext:context];
#if SHOW_PIC_TEX
        [self loadTexture];
#endif
        
#if SHOW_STAMP_TEX
        CZStampGenerator *gen = CZActiveState::getInstance()->getGenerator();
        CZImage *img = gen->getStamp();
        [EAGLContext setCurrentContext:context];
        stampTex = CZTexture::produceFromImage(img);
        textures[0] = stampTex->texId;
        [self checkGLError:NO];
#endif
        
#if SHOW_PREVIEW_TEX
        CZBrush *brush = CZActiveState::getInstance()->getActiveBrush();
        CGSize size = frame.size;
        CZImage *img = brush->previewImageWithSize(CZSize(size.width,size.height));
        
        [EAGLContext setCurrentContext:context];
        stampTex = CZTexture::produceFromImage(img);
        textures[0] = stampTex->texId;
        [self checkGLError:NO];
        delete img;
#endif
        [self loadShaders];
        [self setupView];
        
        glEnable(GL_TEXTURE_2D);        /// will emit gl error, but cause showing nothing if be deleted
        [self checkGLError:NO];
        glActiveTexture(GL_TEXTURE0);
        
        [self checkGLError:NO];
    }
    return self;
}

- (EAGLContext*)getGLContext{
    return context;
}

- (void)draw{
    painting->blit(proj);
}

- (void)draw_test{
    GLfloat data[20];
    GLfloat quad[8] =
    {
        0.0f, (GLfloat)self.size.height,
        0.0f, 0.0f,
        (GLfloat)self.size.width, 0.0f,
        (GLfloat)self.size.width, (GLfloat)self.size.height
    };
    
#if SHOW_STAMP_TEX
    GLfloat s = self.size.width < self.size.height ? (GLfloat)self.size.width : (GLfloat)self.size.height;
    GLfloat f[8] = {0,1,0,0,1,0,1,1};
    for (int i=0; i<4; i++) {
        data[5*i+0] = f[i*2+0]*s;
        data[5*i+1] = f[i*2+1]*s;
        data[5*i+2] = gVertexData[5*i+2];
        data[5*i+3] = gVertexData[5*i+3];
        data[5*i+4] = gVertexData[5*i+4];
    }
#else
    for (int i=0; i<4; i++) {
        data[5*i+0] = quad[i*2+0];
        data[5*i+1] = quad[i*2+1];
        data[5*i+2] = gVertexData[5*i+2];
        data[5*i+3] = gVertexData[5*i+3];
        data[5*i+4] = gVertexData[5*i+4];
    }
#endif
    
#if SHOW_STAMP_TEX
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT );
#endif
    
    delete screenImg;
    delete stampTex;
    CZSize s(self.size.width,self.size.height);
    
    screenImg = painting->imageWithSize(s);
    stampTex = CZTexture::produceFromImage(screenImg);
    textures[0] = stampTex->texId;
    
#if SHOW_FREE_DRAW
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glBindVertexArrayOES(_vertexArray);
    
    // Render the object again with ES2
    glUseProgram(_program);
    
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, GL_FALSE, proj);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(uniforms[TEXTURE], 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#else
    painting->blit(proj);
#endif
    
    [self checkGLError:NO];
}

- (void)moveBeginFrom:(CGPoint)p {
    //freehand->moveBegin(p.x, p.y);
    freehand->moveBegin(100, 100);
    freehand->moving(150, 80, 1);
    freehand->moving(200, 100, 1);
    freehand->moving(250, 120, 1);
    freehand->moveEnd(300, 100);
}
- (void)movingAt:(CGPoint)p {
    freehand->moving(p.x, p.y, 1.0);
}
- (void)moveEndAt:(CGPoint)p {
    freehand->moveEnd(p.x, p.y);
}

- (void)dealloc {
    delete painting;
    delete stampTex;
}

//////////////////////////////////

- (void)setupView {
    [EAGLContext setCurrentContext:context];
    
    glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexData), gVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    
    glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_TRUE, 5*sizeof(GLfloat), (void*)12);
    glBindVertexArrayOES(0);
    
    [self checkGLError:NO];
}

- (BOOL)loadShaders{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vert"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"frag"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(_program, ATTRIB_TEXCOORD, "texcoord");
    
    // Link program.
    if (![self linkProgram:_program]) {
        NSLog(@"Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return NO;
    }
    
    // Get uniform locations.
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    uniforms[TEXTURE] = glGetUniformLocation(_program, "texture");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    
    [self checkGLError:NO];
    
    return YES;
    
}

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return NO;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return NO;
    }
    
    return YES;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

- (BOOL)validateProgram:(GLuint)prog
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

- (void)loadTexture {
    CGImageRef textureImage = [UIImage imageNamed:@"checkerplate.png"].CGImage;
    if (textureImage == nil) {
        NSLog(@"Failed to load texture image");
        return;
    }
    
    size_t texWidth = CGImageGetWidth(textureImage);
    size_t texHeight = CGImageGetHeight(textureImage);
    
    GLubyte *textureData = (GLubyte *)malloc(texWidth * texHeight * 4);
    
    CGContextRef textureContext = CGBitmapContextCreate(textureData,
                                                        texWidth, texHeight,
                                                        8, texWidth * 4,
                                                        CGImageGetColorSpace(textureImage),
                                                        kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(textureContext, CGRectMake(0.0, 0.0, (float)texWidth, (float)texHeight), textureImage);
    CGContextRelease(textureContext);
    
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)texWidth, (GLsizei)texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    
    free(textureData);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    
    [self checkGLError:NO];
}

- (void)checkGLError:(BOOL)visibleCheck {
    GLenum error = glGetError();
    
    switch (error) {
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
        if (visibleCheck) {
            NSLog(@"No GL Error");
        }
        break;
        default:
        NSLog(@"Unknown GL Error");
        break;
    }
}

@end
