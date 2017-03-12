//
// Vertex Array Object wrapper
// Author: Alexey V. Boreskov, <steps3d@gmail.com>, <steps3d@narod.ru>
//

#pragma once
#ifndef	__VERTEX_ARRAY_OBJECT__
#define	__VERTEX_ARRAY_OBJECT__

#include	<GL/glew.h>

class	VertexArray
{
	GLuint	id;
	
								// disable copying
	VertexArray ( const VertexArray& );
	VertexArray& operator = ( const VertexArray& );
	
public:
	VertexArray ()
	{
		id = 0;
	}
	
	~VertexArray ()
	{
		destroy ();
	}
	
	bool	isOk () const
	{
		return glIsVertexArray ( id );
	}
	
	GLuint 	getId () const
	{
		return id;
	}
	
	void	create ()
	{
		glGenVertexArrays ( 1, &id );
	}
	
	void	destroy ()
	{
		if ( id != 0 )
			glDeleteVertexArrays ( 1, &id );

		id = 0;
	}

	void	bind ()
	{
		glBindVertexArray ( id );
	}
	
	void	unbind ()
	{
		glBindVertexArray ( 0 );
	}
};

#endif
