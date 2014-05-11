#include "AbstractModelParameter.h"


namespace bv { namespace model {

// ********************************************************************************************************************

namespace
{

//class DefaultTimeEvaluator : public ITimeEvaluator
//{
//public:
//
//    virtual const std::string & GetName () const
//    {
//        static std::string name = "DefaultTimeEvaluator";
//        return name;
//    }
//
//    virtual TimeType    Evaluate        ( TimeType t ) const
//    {
//        return t;
//    }
//
//    static const ITimeEvaluator * GetDefaultEvaluator   ()
//    {
//        static DefaultTimeEvaluator instance;
//
//        return &instance;
//    }
//};

} //anonynous

// ********************************************************************************************************************

// *******************************
//
AbstractModelParameter::AbstractModelParameter  ( const std::string & name, ModelParamType type, const ITimeEvaluator * evaluator )
    : m_timeEvaluator( evaluator )
    , m_name( name )
    , m_type( type )
{
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

// *******************************
//
void    SetParamTimeline( IParameter * param, const ITimeEvaluator * timeline )
{
    AbstractModelParameter * typedParam = static_cast< AbstractModelParameter * >( param );

    typedParam->SetTimeEvaluator( timeline );
}

} //model
} //bv
