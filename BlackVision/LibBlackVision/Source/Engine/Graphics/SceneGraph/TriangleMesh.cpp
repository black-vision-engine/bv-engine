#include "stdafx.h"

#include "TriangleMesh.h"

#include <cassert>


namespace bv
{

// *********************************
//
TriangleMesh::TriangleMesh   ()
    : Triangles( nullptr, nullptr, nullptr, RenderableType::RT_TRIANGLE_MESH )
{
    assert( false );
}

// *********************************
//
TriangleMesh::~TriangleMesh  ()
{
    assert( false );
}

}
