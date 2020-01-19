#ifndef _OPENGL_
#define _OPENGL_

typedef void (APIENTRY* glBegin_t) (GLenum mode);
typedef	void (APIENTRY* glPopMatrix_t)();
typedef	void (APIENTRY* glPushMatrix_t)();
typedef	void (APIENTRY* glShadeModel_t)(GLenum);
typedef void (APIENTRY* glClear_t) (GLbitfield mask);
typedef void (APIENTRY* glEnable_t) (GLenum mode);
typedef void (APIENTRY* glDisable_t) (GLenum mode);
typedef void (APIENTRY* glEnd_t) (void);
typedef void (APIENTRY* glPolygonOffset_t) (GLfloat factor, GLfloat units);
typedef void (APIENTRY* glTranslatef_t) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* glVertex2f_t) (GLfloat x, GLfloat y);
typedef void (APIENTRY* glVertex3f_t) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* glVertex3fv_t) (const GLfloat* v);
typedef void (APIENTRY* glViewport_t) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef BOOL(APIENTRY* wglSwapBuffers_t)(HDC);
typedef void (APIENTRY* glFrustum_t)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
typedef void (APIENTRY* glBlendFunc_t)(GLenum, GLenum);
typedef void (APIENTRY* glColor4f_t)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY* glReadPixels_t)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*);

extern glBegin_t			pglBegin;
extern glPopMatrix_t	 	pglPopMatrix;
extern glPushMatrix_t	 	pglPushMatrix;
extern glClear_t			pglClear;
extern glEnable_t			pglEnable;
extern glDisable_t			pglDisable;
extern glEnd_t				pglEnd;
extern glPolygonOffset_t	pglPolygonOffset;
extern glTranslatef_t		pglTranslatef;
extern glVertex2f_t			pglVertex2f;
extern glVertex3f_t	 		pglVertex3f;
extern glVertex3fv_t	 	pglVertex3fv;
extern glViewport_t	 		pglViewport;
extern wglSwapBuffers_t 	pwglSwapBuffers;
extern glShadeModel_t	 	pglShadeModel;
extern glFrustum_t          pglFrustum;
extern glBlendFunc_t        pglBlendFunc;
extern glColor4f_t          pglColor4f;
extern glReadPixels_t       pglReadPixels;

extern void InitOpenGL();

#endif