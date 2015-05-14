#include "DrawingTestScene.h"

#include "Prototypes/DrawingCommands/Common/BVGLExt.h"

#include "Scenes\DrawingTestScene0.h"
#include "Scenes\DrawingTestScene1.h"

namespace bv {

DrawingTestScene *			DrawingTestScene::CreateDrawingTestScene	( unsigned int i)
{
	DrawingTestScene * res = nullptr;

    switch( i )
    {
        case 0:
            res = new DrawingTestScene0();
            break;
        default:
            ;
    }
    return res;
}

// *************************
//
							DrawingTestScene::~DrawingTestScene			()
{
    BVGLExt::bvglBindVertexArray( 0 );
    BVGLExt::bvglDeleteVertexArrays( 1, &m_vaoHandle );
	BVGLExt::bvglDeleteBuffers( 1, &m_triangleBuffer );
}

// *************************
//
void						DrawingTestScene::Bind						()
{
    BVGLExt::bvglBindVertexArray( m_vaoHandle );
}

// *************************
//
const DrawingModeData&		DrawingTestScene::GetDrawingModeData		()
{
	return m_drawingModeData;
}


} //bv
