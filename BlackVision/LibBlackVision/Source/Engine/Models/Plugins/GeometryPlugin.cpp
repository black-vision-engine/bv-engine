#include "stdafx.h"

#include "GeometryPlugin.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model { 

// ***************************** PLUGIN ********************************** 
// *******************************
//
GeometryPlugin::GeometryPlugin  ( model::VertexAttributesChannel * vaChannel )
    : BasePlugin( "dupa", "dupa", nullptr, nullptr )
    , m_vaChannel( vaChannel )
{
}

// *******************************
//
GeometryPlugin::~GeometryPlugin ()
{
}

// *******************************
//
IVertexAttributesChannelConstPtr   GeometryPlugin::GetVertexAttributesChannel          () const
{
    return m_vaChannel;
}

// *******************************
//
void  GeometryPlugin::Update          ( TimeType t )
{
    m_vaChannel->Update( t );
}

// *******************************
//
void  GeometryPlugin::Print           ( std::ostream & out, int tabs ) const
{
    { tabs; out; } // FIXME: suppress unused variable
}

}
}
