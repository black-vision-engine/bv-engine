#include "MemManagementInspector.h"

namespace bv {

// *****************************
//
MemManagementInspector::MemManagementInspector      ( Renderer * renderer )
    : m_renderer( renderer )
{
}

// *****************************
//
MemManagementInspector::~MemManagementInspector     ()
{
}

// *****************************
//
void    MemManagementInspector::Initialize          ()
{
}

// *****************************
//
void    MemManagementInspector::Update              ( TimeType t )
{
    { t; }
}

// *****************************
//
void    MemManagementInspector::Render              ()
{
    Render( m_renderer );
}

void    MemManagementInspector::Key                 ( unsigned char c )
{
    { c; }
}

// *****************************
//
void    MemManagementInspector::Resize              ( UInt32 w, UInt32 h )
{
    { w; h; }
}

// *****************************
//
void    MemManagementInspector::Render              ( Renderer * renderer )
{
    { renderer; }
}

}
