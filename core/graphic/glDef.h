
#ifndef _GLDEF_H_
#define _GLDEF_H_

#include "../Macro.h"

#include "CZFbo.h"
#include "CZShader.h"
#include "CZTexture.h"

#if USE_OPENGL

 //#include <gl/GL.h>
 //#include <gl/GLU.h>
# include "glew.h"
# include "glut.h"

# define GL_GEN_VERTEXARRAY(n,arr)	glGenVertexArrays(n, arr)
# define GL_BIND_VERTEXARRAY(id)	glBindVertexArray(id)
# define GL_DEL_VERTEXARRAY(n,arr)	glDeleteVertexArrays(n,arr)
# define GL_DRAW_BUF(arr)           glDrawBuffer(arr)
# define GL_PUSH_ATTR(arr)          glPushAttrib(arr)
# define GL_POP_ATTR()              glPopAttrib()

#elif USE_OPENGL_ES

# include <OpenGLES/EAGL.h>
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>

# define GL_GEN_VERTEXARRAY(n,arr)	glGenVertexArraysOES(n, arr)
# define GL_BIND_VERTEXARRAY(id)	glBindVertexArrayOES(id)
# define GL_DEL_VERTEXARRAY(n,arr)	glDeleteVertexArraysOES(n,arr)
# define GL_DRAW_BUF(arr)
# define GL_PUSH_ATTR(arr)
# define GL_POP_ATTR()

#endif

#endif