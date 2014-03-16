namespace bv {

// ***************************
//
inline unsigned int                 ShaderParameters::NumParameters     () const
{
    return m_shaderParams.size();   
}

// ***************************
//
inline const GenericShaderParam *   ShaderParameters::GetParam          ( unsigned int idx ) const
{
    assert( idx < NumParameters() );

    return m_shaderParams[ idx ];
}

// ***************************
//
inline const GenericShaderParam *   ShaderParameters::GetParam          ( const std::string & name ) const
{
    for( auto param : m_shaderParams )
    {
        if( param->Name() == name )
        {
            return param;
        }
    }

    return nullptr;    
}

// ***************************
//
unsigned int                        ShaderParameters::NumTextures       () const
{
    return m_textures.size();
}

// ***************************
//
const Texture2D *                   ShaderParameters::GetTexture        ( unsigned int idx ) const
{
    assert( idx < NumTextures() );

    return m_textures[ idx ];
}

} //bv
