//
// Simple wrapper class for freeglut
//

#ifndef	__GLUT_WINDOW__
#define	__GLUT_WINDOW__

#include	<GL/glew.h>

#ifdef	_WIN32
	#include	<GL/wglew.h>
#else
	#include	<GL/glxew.h>
#endif

#include	<string>
#include	<GL/freeglut.h>

using namespace std;

class	GlutWindow
{
	int		width, height;
	string	caption;
	int		id;
	float	time;				// time since last redisplay ()
	
public:
	GlutWindow  ( int xo, int yo, int w, int h, const char * caption );
	~GlutWindow ();
	
	int	getWidth () const
	{
		return width;
	}
	
	int	getHeight () const
	{
		return height;
	}
	
	const string& getCaption () const
	{
		return caption;
	}
	
	float	getDeltaTime () const
	{
		return time - 0.001f * glutGet ( GLUT_ELAPSED_TIME );
	}
	
	float	getTime () const
	{
		return 0.001f * glutGet ( GLUT_ELAPSED_TIME );
	}
	
	void	setTime ()
	{
		time = 0.001f * glutGet ( GLUT_ELAPSED_TIME );
	}
	
	virtual	void	redisplay () {}
	
	virtual	void	reshape ( int w, int h )
	{
		width  = w;
		height = h;
	}
	
					// modifiers  - GLUT_ACTIVE_SHIFT, GLUT_ACTIVE_CTRL, GLUT_ACTIVE_ALT
					// button - GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
	virtual void	mouseClick         ( int button, int state, int modifiers, int x, int y ) {}
    virtual void	mouseWheel         ( int wheel, int modifiers, int x, int y ) {}
    virtual void	mouseMotion        ( int x, int y ) {}
    virtual void	mousePassiveMotion ( int x, int y ) {}
    virtual void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );
	}
	
    virtual void	keyReleased        ( unsigned char c, int modifiers, int x, int y ) {}
    virtual void	specialKey         ( int key, int modifier, int x, int y ) {}
    virtual void	specialKeyUp       ( int key, int modifier, int x, int y ) {}

	virtual	void	idle () 
	{
		glutPostRedisplay ();
	}
	
	static	void	run ()
	{
		glutMainLoop ();
	}
	
	static	void	init ( int& argc, char ** argv, int major = 3, int minor = 3, 
	                       int mode = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH, bool debug = true );
};


#endif
