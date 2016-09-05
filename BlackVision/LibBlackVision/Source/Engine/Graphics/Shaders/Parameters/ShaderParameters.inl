namespace bv {

// ***************************
//
inline unsigned int                 ShaderParameters::NumParameters     () const
{
    return (unsigned int) m_shaderParams.size();   
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
    {\
        if( param->Name() == name )
        {
            return param;
        }
    }

    return nullptr;    
}

// ***************************
//
inline GenericShaderParam *         ShaderParameters::AccessParam       ( unsigned int idx )
{
    assert( idx < NumParameters() );

    return m_shaderParams[ idx ];
}

// ***************************
//
inline GenericShaderParam *         ShaderParameters::AccessParam       ( const std::string & name )
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
inline unsigned int                 ShaderParameters::NumTextures       () const
{
    return (unsigned int) m_textures.size();
}

// ***************************
//
inline Texture2DPtr                 ShaderParameters::GetTexture        ( unsigned int idx )
{
    assert( idx < NumTextures() );

    return m_textures[ idx ].first;
}

// ***************************
//
inline SamplerShaderParametersPtr	ShaderParameters::GetSamplerParameters        ( unsigned int idx ) const
{
    assert( idx < NumTextures() );

    return m_textures[ idx ].second;
}

} //bv
