//
//  OpenGLView.swift
//  Swift_Learn
//
//  Created by CharlyZhang on 15-6-30.
//  Copyright (c) 2015年 Founder. All rights reserved.
//

import Foundation
import UIKit
import QuartzCore
import OpenGLES

class OpenGLView: UIView {
    
    var eaglLayer: CAEAGLLayer!
    var context: EAGLContext!
    var colorRenderBuffer: GLuint = GLuint()
    var brush:libBrush_wraper?
    
    required init(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
        
        self.setupLayer()
        
        
        self.frame.size = CGSizeMake(768, 1024);
        self.brush = libBrush_wraper(frame: self.frame)
        
        self.context = self.brush!.getGLContext()
        if !EAGLContext.setCurrentContext(self.context) {
            println("Failed to set current OpenGL context!")
        }
        
        self.setupRenderBuffer()
        self.setupFrameBuffer()
        self.render()
    }
    
    override class func layerClass() -> AnyClass {
        return CAEAGLLayer.self
    }
    
    /*override class func drawRect(rect: CGRect) {
    
    }*/
    
    func setupLayer() {
        self.eaglLayer = self.layer as CAEAGLLayer
        self.eaglLayer.opaque = true
    }
    
    func setupRenderBuffer() {
        glGenRenderbuffers(1, &self.colorRenderBuffer)
        glBindRenderbuffer(GLenum(GL_RENDERBUFFER), self.colorRenderBuffer)
        self.context.renderbufferStorage(Int(GL_RENDERBUFFER), fromDrawable: self.eaglLayer)
    }
    
    func setupFrameBuffer() {
        var frameBuffer: GLuint = GLuint()
        glGenFramebuffers(1, &frameBuffer)
        glBindFramebuffer(GLenum(GL_FRAMEBUFFER), frameBuffer)
        glFramebufferRenderbuffer(GLenum(GL_FRAMEBUFFER), GLenum(GL_COLOR_ATTACHMENT0), GLenum(GL_RENDERBUFFER), self.colorRenderBuffer)
    }
    
    // function renders elements
    func render() {
        if !EAGLContext.setCurrentContext(self.context) {
            println("Failed to set current OpenGL context!")
        }
        
        glClearColor(1, 1, 1, 1)
        glClear(GLbitfield(GL_COLOR_BUFFER_BIT))
        
        glEnable(GLenum(GL_BLEND))
        glBlendFunc(GLenum(GL_ONE), GLenum(GL_ONE_MINUS_SRC_ALPHA))
        
        glViewport(0, 0, GLint(self.frame.size.width), GLint(self.frame.size.height))
        self.brush!.draw_test()
        self.context.presentRenderbuffer(Int(GL_RENDERBUFFER))
    }
    
    func checkGL(visibleCheck:Bool) {
        var error:GLenum  = glGetError()
        switch (error) {
        case GLenum(GL_INVALID_ENUM):
            println("GL Error: Enum argument is out of range")
        case GLenum(GL_INVALID_VALUE):
            println("GL Error: Numeric value is out of range")
        case GLenum(GL_INVALID_OPERATION):
            println("GL Error: Operation illegal in current state")
        case GLenum(GL_OUT_OF_MEMORY):
            println("GL Error: Not enough memory to execute command")
        case GLenum(GL_NO_ERROR):
            if (visibleCheck) {
                println("No GL Error")
            }
        default:
            println("Unknown GL Error")
        }
    }
    
    override func drawRect(rect: CGRect) {
        render()
    }
    
    override func touchesBegan(touches: NSSet, withEvent event: UIEvent){
        var touch:UITouch = touches.anyObject() as UITouch
        var location:CGPoint = touch.locationInView(self)
        
        self.brush!.moveBeginFrom(CGPointMake(location.x,self.bounds.size.height-location.y))
        self.render()
    }
    
    override func touchesMoved(touches: NSSet, withEvent event: UIEvent) {
        var touch:UITouch = touches.anyObject() as UITouch
        var location:CGPoint = touch.locationInView(self)
        
        self.brush!.movingAt(CGPointMake(location.x,self.bounds.size.height-location.y))
        self.render()
    }
    
    override func touchesEnded(touches: NSSet, withEvent event: UIEvent) {
        var touch:UITouch = touches.anyObject() as UITouch
        var location:CGPoint = touch.locationInView(self)
        
        self.brush!.moveEndAt(CGPointMake(location.x,self.bounds.size.height-location.y))
        self.render()
    }
}