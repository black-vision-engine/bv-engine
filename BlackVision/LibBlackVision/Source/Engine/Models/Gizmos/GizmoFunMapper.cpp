#include "stdafx.h"
#include "GizmoFunMapper.h"




namespace bv
{



// ***********************
//
GizmoFunMapper::GizmoFunctionsList      GizmoFunMapper::ListFunctionalities     () const
{
    GizmoFunctionsList functionsList;

    for( auto & fun : m_gizmoLogicsMap )
    {
        functionsList.push_back( fun.first );
    }

    return functionsList;
}

// ***********************
//
GizmoFunMapper::GizmoLogicsList         GizmoFunMapper::ListUsedGizmoLogics     () const
{
    GizmoLogicsList logicsList;

    for( auto & fun : m_gizmoLogicsMap )
    {
        logicsList.insert( fun.second );
    }

    return logicsList;
}

// ***********************
//
std::string                             GizmoFunMapper::QueryGizmoLogicName     ( const std::string & functionalityName ) const
{
    auto gizmoLogicIter = m_gizmoLogicsMap.find( functionalityName );
    
    if( gizmoLogicIter != m_gizmoLogicsMap.end() )
        return gizmoLogicIter->second;

    return "";
}

// ***********************
//
void                                    GizmoFunMapper::RegisterFunctionality   ( const std::string & function, const std::string & logicName )
{
    m_gizmoLogicsMap[ function ] = logicName;
}


}	// bv


