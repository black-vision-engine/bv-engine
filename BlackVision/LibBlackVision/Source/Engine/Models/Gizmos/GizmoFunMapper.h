#pragma once



#include <map>
#include <vector>
#include <string>


namespace bv
{


// ***********************
//
class GizmoFunMapper
{
public:

    typedef std::map< std::string, std::string >    GizmoLogicsMap;
    typedef std::vector< std::string >              GizmoFunctionsList;
    typedef std::set< std::string >                 GizmoLogicsList;

private:

    GizmoLogicsMap          m_gizmoLogicsMap;       // Maps functionalities to gizmo logics names.

public:

    GizmoFunctionsList              ListFunctionalities     () const;
    GizmoLogicsList                 ListUsedGizmoLogics     () const;
    std::string                     QueryGizmoLogicName     ( const std::string & functionalityName ) const;

    void                            RegisterFunctionality   ( const std::string & function, const std::string & logicName );
};



}	// bv

