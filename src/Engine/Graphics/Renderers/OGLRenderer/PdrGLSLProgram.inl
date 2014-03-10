namespace bv {

// *******************************
//
inline int  PdrGLSLProgram::GetUniformLocation( const string & name )
{
    auto it = m_uniformLocations.find( name );
    
    if ( it == m_uniformLocations.end() )
    {
        int loc = glGetUniformLocation( m_Handle, name.c_str() );
        m_uniformLocations[ name ] = loc;

        return loc;
    }

    return it->second;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, float x, float y, float z )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, x, y, z );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, const vec3 & v )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, v );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, const vec4 & v )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, v );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, const vec2 & v )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, v );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, const mat2 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, m );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, const mat3 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, m );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, const mat4 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, m );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, float val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, val );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, int val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, val );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline int  PdrGLSLProgram::SetUniform( const string & name, bool val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        SetUniformImpl( loc, val );
    } 
    else 
    {
        PostSetUniformFail( name );
    }

    return loc;
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, float x, float y, float z )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, x, y, z );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, const vec3 & v )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, v );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, const vec4 & v )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, v );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, const vec2 & v )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, v );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, const mat2 & m )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, m );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, const mat3 & m )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, m );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, const mat4 & m )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, m );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, float val )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, val );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, int val )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, val );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniform( int loc, bool val )
{
    if( loc >= 0 )
    {
        SetUniformImpl( loc, val );
    } 
    else 
    {
        PostSetUniformFail( loc );
    }
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, float x, float y, float z )
{
    glUniform3f( loc, x, y, z );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, const vec3 & v )
{
    glUniform3f( loc, v.x, v.y, v.z );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, const vec4 & v )
{
    glUniform4f( loc, v.x, v.y ,v.z, v.w );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, const vec2 & v )
{
    glUniform2f( loc, v.x ,v.y );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, const mat2 & m )
{
    glUniformMatrix2fv( loc, 1, GL_FALSE, &m[0][0] );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, const mat3 & m )
{
    glUniformMatrix3fv( loc, 1, GL_FALSE, &m[0][0] );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, const mat4 & m )
{
    glUniformMatrix4fv( loc, 1, GL_FALSE, &m[0][0] );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, float val )
{
    glUniform1f( loc, val );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, int val )
{
    glUniform1i( loc, val );
}

// *******************************
//
inline void PdrGLSLProgram::SetUniformImpl( int loc, bool val )
{
    glUniform1i( loc, val );
}

} //bv
