#include "stdafx.h"
#include "CameraLayout.h"


#include "Engine/Graphics/Renderers/OGLRenderer/PdrGLSLProgram.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrUniformBufferObject.h"


namespace bv
{

std::string          CameraLayout::UNIFORM_BLOCK_NAME       = "Camera";
std::string          CameraLayout::UNIFORM_BLOCK_DEF_PATH   = "Assets/Shaders/UniformBlockDefs/camera";


// ***********************
//
CameraLayout::CameraLayout()
{
    UniformBlockLayout * layout = nullptr;

    PdrGLSLProgram program;

    if( program.CompileShaderFromFile( UNIFORM_BLOCK_DEF_PATH, GLSLShader::VERTEX ) && program.Link() )
    {
       program.Use();
       layout = PdrUniformBufferObject::GetUniformBlockLayout( program.GetHandle(), UNIFORM_BLOCK_NAME );
    }

    m_blockLayout = std::unique_ptr< UniformBlockLayout >( layout );
}

// ***********************
//
CameraLayout::~CameraLayout()
{}

// *************************************
//
CameraLayout &                  CameraLayout::Instance                  ()
{
    static CameraLayout instance;
    return instance;
}

// *************************************
//
const UniformBlockLayout *      CameraLayout::GetBlockLayout            () const
{
    return m_blockLayout.get();
}


}	// bv