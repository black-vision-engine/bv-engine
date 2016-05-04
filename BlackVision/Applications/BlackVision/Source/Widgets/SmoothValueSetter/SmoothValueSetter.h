#pragma once

#include "CoreDEF.h"
#include "../NodeLogicBase.h"           // Widgets/NodeLogicBase.h doesn't work

#include "ParameterBinding.h"

#include <vector>

namespace bv
{

namespace model {

class BasicNode;
DEFINE_PTR_TYPE( BasicNode )

} // model

namespace nodelogic
{




class SmoothValueSetter
{
private:

    std::string                         m_sceneName;
    bv::model::BasicNodePtr	            m_parentNode;

    std::vector< ParameterBinding >     m_paramBindings;
    

public:
    SmoothValueSetter   ();
    ~SmoothValueSetter  ();
};



}   // nodelogic
}	// bv

