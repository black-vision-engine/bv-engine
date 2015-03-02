#include "IParameter.h"

namespace bv { namespace model {

    class ICachedParameter;
    DEFINE_PTR_TYPE(ICachedParameter)

    //class ICachedParameter;
    //typedef std::shared_ptr< const IParameter > IParameterConstPtr;

    class ICachedParameter : public IParameter
    {
    public:
        virtual bool                    Changed() = 0;
    };


} //model
} //bv
