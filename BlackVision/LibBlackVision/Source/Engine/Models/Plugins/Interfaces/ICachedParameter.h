#pragma once

//#include "IParameter.h"
#include "CoreDEF.h"

namespace bv { namespace model {

    class ICachedParameter /*: public IParameter*/ // FIXME: maybe ICachedValue would be a better name
    {
    public:
        virtual bool                    Changed() const = 0;
    };

    //class ICachedParameter;
    DEFINE_PTR_TYPE(ICachedParameter)

    //class ICachedParameter;
    //typedef std::shared_ptr< const IParameter > IParameterConstPtr;


} //model
} //bv
