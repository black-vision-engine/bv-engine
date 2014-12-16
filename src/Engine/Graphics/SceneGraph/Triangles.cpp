#include "Triangles.h"

#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv
{

// *********************************
//
Triangles::Triangles               ( RenderableArrayDataSingleVertexBuffer * rad, RenderableEffectPtr effect, RenderableType type )
    : RenderableEntity( type, rad, effect )
{
}

// *********************************
//
Triangles::~Triangles              ()
{
}

}
