#include "stdafx.h"

#include "Triangles.h"

#include "Engine/Graphics/Resources/RenderableArrayData.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"




#include "Memory/MemoryLeaks.h"



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

// **************************
//
int     Triangles::NumTriangles    ( unsigned int ccNum ) const
{
    return NumVertices( ccNum ) - 2;
}

// **************************
//
int     Triangles::NumVertices     ( unsigned int ccNum ) const
{
    return RAD()->GetNumVerticesInConnectedComponent( ccNum );
}

} //bv
