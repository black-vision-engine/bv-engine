#pragma once

#include "Engine/Models/Interfaces/IGizmoLogic.h"
#include "Engine/Models/Interfaces/IModelNode.h"



#include <vector>


namespace bv {
namespace model
{



// ***********************
//
class GizmoContainer
{
private:

    std::vector< IModelNodePtr >        m_gizmoRoots;

public:



};


DEFINE_UPTR_TYPE( GizmoContainer );


}
}	// bv

