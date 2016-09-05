#include "stdafx.h"

#include "LightsLayout.h"

#include <algorithm>

#include "Engine/Graphics/Renderers/OGLRenderer/PdrGLSLProgram.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrUniformBufferObject.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{


const UInt32         LightsLayout::MAX_LIGTHS               = 8;
std::string          LightsLayout::UNIFORM_BLOCK_NAME       = "Lights";
std::string          LightsLayout::UNIFORM_BLOCK_DEF_PATH   = "Assets/Shaders/UniformBlockDefs/lights";


// *************************************
//
                                LightsLayout::LightsLayout              ()
{
    UniformBlockLayout * layout = nullptr;

    PdrGLSLProgram program;

    if( program.CompileShaderFromFile( UNIFORM_BLOCK_DEF_PATH, GLSLShader::VERTEX ) && program.Link() )
    {
       program.Use();
       layout = PdrUniformBufferObject::GetUniformBlockLayout( program.GetHandle(), UNIFORM_BLOCK_NAME );
    }

    assert( ( UInt32 )LightType::LT_TOTAL == 3 );
    
    std::string lightsStr[] = { "directionalLight", "pointLight", "spotLight" };
    std::string numLightsStr[] = { "directionalLightNum", "pointLightNum", "spotLightNum" };

    for( UInt32 i = 0; i < ( UInt32 )LightType::LT_TOTAL; ++i )
    {
        auto type = LightType( i );
        m_lightsNumOffsetIdx[ type ] = ( UInt32 )std::distance( layout->uniformDescs.begin(), 
            std::find_if( layout->uniformDescs.begin(), layout->uniformDescs.end(), [ & ]( const UniformDesc & desc ) 
            { return desc.name == numLightsStr[ i ]; } ) );

        m_lightOffsetIdx[ type ].resize( MAX_LIGTHS );
        for( UInt32 j = 0; j < MAX_LIGTHS; ++j )
        {
            std::string str = lightsStr[ i ] + "[" + std::to_string( j ) + "]";
            m_lightOffsetIdx[ type ][ j ] = ( UInt32 )std::distance( layout->uniformDescs.begin(), 
                std::find_if( layout->uniformDescs.begin(), layout->uniformDescs.end(), [ & ]( const UniformDesc & desc ) 
                { return desc.name.compare( 0, str.length(), str ) == 0; } ) );
        }
    }

    m_blockLayout = std::unique_ptr< UniformBlockLayout >( layout );
}

// *************************************
//
LightsLayout &                  LightsLayout::Instance                  ()
{
    static LightsLayout instance;
    return instance;
}

// *************************************
//
const UniformBlockLayout *      LightsLayout::GetBlockLayout            () const
{
    return m_blockLayout.get();
}

// *************************************
//
UInt32                          LightsLayout::GetLightOffsetIdx         ( LightType type, UInt32 idx ) const
{
    assert( idx < MAX_LIGTHS );
    return m_lightOffsetIdx.at( type )[ idx ];
}

// *************************************
//
UInt32                          LightsLayout::GetLightsNumOffsetIdx     ( LightType type ) const
{
    return m_lightsNumOffsetIdx.at( type );
}

} // bv
