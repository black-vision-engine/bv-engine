#include "stdafx.h"
#include "TabStop.h"

#include "Engine/Models/BVProjectEditor.h"


namespace bv
{

// ***********************
//
TabStop::TabStop( TabStopType type, const std::string& name )
    :   m_type( type )
    ,   m_name( name )
    ,   m_position( 0.0 )
{}

// ***********************
//
TabStop::~TabStop()
{}


}	// bv