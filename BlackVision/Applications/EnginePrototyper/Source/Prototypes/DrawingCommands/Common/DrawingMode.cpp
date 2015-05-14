#include "DrawingMode.h"

#include "Prototypes/DrawingCommands/Common/Modes/DrawingModes.h"

namespace  bv {

// **************************
//
DrawingMode *    DrawingMode::CreateDrawingMode  ( unsigned int i )
{
    DrawingMode * res = nullptr;

    switch( i )
    {
        case 0:
            //res = new DrawingModeArrays();
            //res = new DrawingModeElements();

            //res = new DrawingModeArraysInstanced();
            res = new DrawingModeElementsInstanced();
           
			//----------------

            //res = new DrawingModeElementsBaseVertex();

			//res = new DrawingModeArraysInstancedBaseInstance();
            //res = new DrawingModeElementsInstancedBaseVertexBaseInstance();
            
			//res = new DrawingModeElementsInstancedBaseVertex();

            //res = new DrawingModeArraysIndirect();
            //res = new DrawingModeElementsIndirect();
            //res = new DrawingModeMultiArraysIndirect();
            //res = new DrawingModeMultiElementsIndirect();
            break;
        default:
            ;
    }

    return res;
}

// **************************
//
DrawingMode::DrawingMode			()
{
}

// **************************
//
DrawingMode::~DrawingMode			()
{
}

} //bv
