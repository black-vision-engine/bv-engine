#include "stdafx.h"

#include "AbstractModelParameter.h"
#include "ParametersFactory.h"

//#include "Serialization/SerializationObjects.inl"



#include "Memory/MemoryLeaks.h"



namespace bv { 

// serialization stuff
//template std::vector< std::shared_ptr< model::AbstractModelParameter > >                         DeserializeArray( const IDeserializer& pimpl, std::string name );

    
namespace model {

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
AbstractModelParameter::AbstractModelParameter  ( const std::string & name, ModelParamType type, ITimeEvaluatorPtr evaluator )
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
void                AbstractModelParameter::SetName ( const std::string & name )
{
    m_name = name;
}

// *******************************
//
ModelParamType      AbstractModelParameter::GetType () const
{
    return m_type;
}

//// *******************************
////
//void                AbstractModelParameter::SetInterpolationMethod ( IParameter::InterpolationMethod method )
//{
//    m_method = method;
//}
//
//// *******************************
////
//IParameter::InterpolationMethod AbstractModelParameter::GetInterpolationMethod () const
//{
//    return m_method;
//}



// *******************************
//
void    SetParamTimeline( IParameterPtr param, ITimeEvaluatorPtr timeline )
{
    AbstractModelParameterPtr typedParam = std::static_pointer_cast< AbstractModelParameter >( param );

    typedParam->SetTimeEvaluator( timeline );
}

} //model
} //bv
