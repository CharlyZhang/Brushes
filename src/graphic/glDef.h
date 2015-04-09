
#include "../Macro.h"

#if USE_OPENGL

//#include <gl/GL.h>
//#include <gl/GLU.h>
#include "gl/glew.h"
#include "gl/glut.h"

#elif USE_OPENGL_ES

#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif