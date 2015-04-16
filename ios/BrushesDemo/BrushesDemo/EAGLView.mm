//
//  EAGLView.m
//  AppleCoder-OpenGLES-00
//
//  Created by Simon Maurice on 18/03/09.
//  Copyright Simon Maurice 2009. All rights reserved.
//



#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"
#import "gl_matrix.h"
#include "stamp/CZSpiralGenerator.h"
#include "graphic/CZTexture.h"
#include "basic/CZImage.h"
#include "CZActiveState.h"
#include "painting/CZPainting.h"
#include "tool/CZFreehandTool.h"
#include "brush/CZBrush.h"

#define REACT_PIC_TEX       0
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

// A class extension to declare private methods
@interface EAGLView ()
{
    CZTexture *stampTex;
    CZTool *freehand;
    CZPainting *painting;
    
}

@property (nonatomic, retain) EAGLContext *context;

- (BOOL) createFramebuffer;
- (void) destroyFramebuffer;

@end


@implementation EAGLView

@synthesize context;

// You must implement this method
// !!! because of this modification, drawRect cannot be avoked automatically when call setNeedsDisplay.
+ (Class)layerClass {
    return [CAEAGLLayer class];
}


-(id)initWithFrame:(CGRect)frame{
    if ((self = [super initWithFrame:frame])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8,
										kEAGLDrawablePropertyColorFormat, nil];
#if !SHOW_FREE_DRAW
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        if (!context || ![EAGLContext setCurrentContext:context]) {
            [self release];
            return nil;
        }
#endif
        
#if SHOW_PIC_TEX || REACT_PIC_TEX
        [self loadTexture];
#endif
        
        stampTex = NULL;
        painting = NULL;
        
#if SHOW_STAMP_TEX
        CZStampGenerator *gen = CZActiveState::getInstance()->getRandomGenerator();
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
        
#if SHOW_FREE_DRAW
        CGSize size = self.bounds.size;
        painting = new CZPainting(CZSize(size.width, size.height));
        freehand = CZActiveState::getInstance()->getActiveTool();
        freehand->ptrPainting = painting;
        context = (EAGLContext *) painting->getGLContext()->getRealContext();
        [self checkGLError:NO];
#endif
        
        [self loadShaders];
        [self setupView];
   
        glEnable(GL_TEXTURE_2D);        /// will emit gl error, but cause showing nothing if be deleted
        [self checkGLError:NO];
        glActiveTexture(GL_TEXTURE0);
        
        [self checkGLError:NO];
        locX = locY = 0.0f;
    }
    return self;
}

- (void)drawView {

    [EAGLContext setCurrentContext:context];    
    glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    glClearColor(1, 1, 1, 1);
#if SHOW_STAMP_TEX
    glClearColor(0, 0, 0, 0);
#endif
	glClear(GL_COLOR_BUFFER_BIT );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat data[20];
    CGSize size = self.bounds.size;
    GLfloat quad[8] =
    {
        0.0f, (GLfloat)size.height,
        0.0f, 0.0f,
        (GLfloat)size.width, 0.0f,
        (GLfloat)size.width, (GLfloat)size.height
    };
    
    // update the data
#if REACT_PIC_TEX
    // update the data
    for (int i=0; i<4; i++) {
        data[5*i+0] = gVertexData[5*i+0] + locX;
        data[5*i+1] = gVertexData[5*i+1] + locY;
        data[5*i+2] = gVertexData[5*i+2];
        data[5*i+3] = gVertexData[5*i+3];
        data[5*i+4] = gVertexData[5*i+4];
    }
#elif SHOW_STAMP_TEX
    GLfloat s = size.width < size.height ? (GLfloat)size.width : (GLfloat)size.height;
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
   
#if !SHOW_FREE_DRAW
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glBindVertexArrayOES(_vertexArray);
    
    // Render the object again with ES2
    glUseProgram(_program);
    
    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, GL_FALSE, _modelViewProjectionMatrix);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(uniforms[TEXTURE], 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#else
    CZMat4 proj;
    proj.SetOrtho(0,size.width, 0, size.height, -1.0f, 1.0f);
    painting->blit(proj);
#endif
    
    [self checkGLError:NO];
    
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
	[self checkGLError:NO];
    
    //delete tex;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint location =[touch locationInView:self];
    //NSLog(@"touchBegin %f, %f",location.x,location.y);
    locX = location.x;
    locY = self.bounds.size.height - location.y;
#if SHOW_FREE_DRAW
    freehand->moveBegin(locX,locY);
#endif
    [self drawView];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    //NSLog(@"touchMove %f, %f",location.x,location.y);
    //[self setNeedsDisplay];
    locX = location.x;
    locY = self.bounds.size.height - location.y;
#if SHOW_FREE_DRAW
    freehand->moving(locX, locY, 1.0);
#endif
    [self drawView];
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    //NSLog(@"touchEnd %f, %f",location.x,location.y);

    locX = location.x;
    locY = self.bounds.size.height - location.y;
#if SHOW_FREE_DRAW
    freehand->moveEnd(locX, locY);
#endif
    [self drawView];
}


- (void)layoutSubviews {
    [self checkGLError:NO];
    [EAGLContext setCurrentContext:context];
    [self destroyFramebuffer];
    [self checkGLError:NO];
    [self createFramebuffer];
    [self checkGLError:NO];
    [self drawView];
}


- (BOOL)createFramebuffer {
    
    glGenFramebuffers(1, &viewFramebuffer);
    glGenRenderbuffers(1, &viewRenderbuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, viewRenderbuffer);
    
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
    
    return YES;
}
- (void)destroyFramebuffer {
    if (viewFramebuffer) {
        glDeleteFramebuffers(1, &viewFramebuffer);
        viewFramebuffer = 0;
    }
    if (viewRenderbuffer) {
        glDeleteRenderbuffers(1, &viewRenderbuffer);
        viewRenderbuffer = 0;
    }
}

- (void)setupView {
    CGSize size = self.bounds.size;
    mat4f_LoadOrtho(0, size.width,0, size.height, -1, 1, _modelViewProjectionMatrix);
    
    [EAGLContext setCurrentContext:self.context];
    
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
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
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

- (void)dealloc {
    
    [EAGLContext setCurrentContext:self.context];
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteVertexArraysOES(1, &_vertexArray);
    
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    delete stampTex;
    delete painting;
    [context release];  
    [super dealloc];
}

@end
