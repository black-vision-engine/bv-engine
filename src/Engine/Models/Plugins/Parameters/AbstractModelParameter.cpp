#include "AbstractModelParameter.h"


namespace bv { namespace model {

// ********************************************************************************************************************

namespace
{

class DefaultTimeEvaluator : public ITimeEvaluator
{
public:

    virtual const std::string & GetName () const
    {
        static std::string name = "DefaultTimeEvaluator";
        return name;
    }

    virtual TimeType    Evaluate        ( TimeType t ) const
    {
        return t;
    }

    static const ITimeEvaluator * GetDefaultEvaluator   ()
    {
        static DefaultTimeEvaluator instance;

        return &instance;
    }
};

} //anonynous

// ********************************************************************************************************************

// *******************************
//
AbstractModelParameter::AbstractModelParameter  ( const std::string & name, ModelParamType type, const ITimeEvaluator * evaluator )
    : m_timeEvaluator( evaluator )
    , m_name( name )
    , m_type( type )
{
    if( evaluator == nullptr )
    {
        m_timeEvaluator = DefaultTimeEvaluator::GetDefaultEvaluator();
    }
}

// *******************************
//
AbstractModelParameter::~AbstractModelParameter ()
{
}

// *******************************
//
const std::string & AbstractModelParameter::GetName () const
{
    return m_name;
}

// *******************************
//
ModelParamType      AbstractModelParameter::GetType () const
{
    return m_type;
}

} //model
} //bv
