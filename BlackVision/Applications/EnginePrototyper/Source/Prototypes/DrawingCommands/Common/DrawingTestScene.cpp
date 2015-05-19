#include "DrawingTestScene.h"

#include "Prototypes/DrawingCommands/Common/BVGLExt.h"

#include "Scenes\DrawingTestScene0.h"
//#include "Scenes\DrawingTestScene1.h"

namespace bv {

// *************************
//
							DrawingTestScene::~DrawingTestScene			()
{
    BVGLExt::bvglBindVertexArray( 0 );
    BVGLExt::bvglDeleteVertexArrays( 1, &m_vaoHandle );
	BVGLExt::bvglDeleteBuffers( 1, &m_vboHandle );
	BVGLExt::bvglDeleteBuffers( 1, &m_indexBuffer );
	BVGLExt::bvglDeleteBuffers( 1, &m_indirectArraysBuffer );
	BVGLExt::bvglDeleteBuffers( 1, &m_indirectElementsBuffer );
}

// *************************
//
void						DrawingTestScene::Bind						()
{
    BVGLExt::bvglBindVertexArray( m_vaoHandle );
	m_prog.Use();
}

// *************************
//
void						DrawingTestScene::BindIndirectArrays		()
{
	BVGLExt::bvglBindBuffer( GL_DRAW_INDIRECT_BUFFER, m_indirectArraysBuffer );
}

// *************************
//
void						DrawingTestScene::BindIndirectElements		()
{
	BVGLExt::bvglBindBuffer( GL_DRAW_INDIRECT_BUFFER, m_indirectElementsBuffer );
}

// *************************
//
const DrawingModeData&		DrawingTestScene::GetDrawingModeData		()
{
	return m_drawingModeData;
}

// **************************
//
bool						DrawingTestScene::PrepareShader				( const char * vname, const char * fname)
{
    std::string shadersRoot = config::PROTOTYPES_SHADERS_ROOT + "DrawingCommands/";

    std::string vsFile = shadersRoot + vname + ".vert";
    std::string psFile = shadersRoot + fname + ".frag";

    if( !m_prog.CompileShaderFromFile( vsFile.c_str(), GL_VERTEX_SHADER ) )
    {
        printf( "Vertex shader failed to compile!\n%s", m_prog.Log().c_str());
        return false;
    }

    if( !m_prog.CompileShaderFromFile( psFile.c_str(), GL_FRAGMENT_SHADER ) )
    {
        printf( "Pixel shader failed to compile!\n%s", m_prog.Log().c_str());
        return false;
    }

    if( !m_prog.Link() )
    {
        printf( "Shader program failed to link!\n%s", m_prog.Log().c_str() );
        return false;
    }

    return true;
}

// *************************
//

std::map<unsigned int, DrawingTestScene*(*)()>	DrawingTestScene::m_scenes;

unsigned int				DrawingTestScene::m_currentScene;

// *************************
//
template<typename T> 
DrawingTestScene *			DrawingTestScene::CreateInstance()
{
	return new T;
}

// *************************
//
DrawingTestScene *			DrawingTestScene::GetNextScene				()
{
	m_currentScene = (m_currentScene + 1) % m_scenes.size();
	return m_scenes[ m_currentScene ]();
}

// *************************
//
void						DrawingTestScene::Initialize				()
{
	m_scenes[0] = &DrawingTestScene::CreateInstance< DrawingTestScene0 >;
	//m_scenes[1] = &DrawingTestScene::CreateInstance< DrawingTestScene1 >;
	//add more scenes...

	m_currentScene = (unsigned int)m_scenes.size() - 1;
}


} //bv
