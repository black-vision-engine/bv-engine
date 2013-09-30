#include "Triangles.h"

#include "Engine\Graphics\Resources\RenderableArrayData.h"
#include "Engine\Graphics\Shaders\RenderableEffect.h"

namespace bv
{

// *********************************
//
Triangles::Triangles               ( RenderableArrayData * rad, RenderableEffect * effect, RenderableType type )
    : RenderableEntity( type, rad, effect )
{
}

// *********************************
//
Triangles::~Triangles              ()
{
}

}
