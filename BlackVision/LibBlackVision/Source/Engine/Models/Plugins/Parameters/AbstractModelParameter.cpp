#include "AbstractModelParameter.h"
#include "ParametersFactory.h"


namespace bv { namespace model {

class KeyFrame : public ISerializable
{
public:
    std::string time, value; // FIXME

    KeyFrame( std::string t, std::string v ) : time( t ), value( v ) {}

    virtual void                Serialize       ( SerializeObject &/*doc*/ ) const {}
    static ISerializablePtr     Create          ( DeserializeObject &doc )
    {
        auto time = doc.GetValue( "time" );
        auto value = doc.GetValue( "value" );
        return std::make_shared< KeyFrame >( time, value );
    }
};

// ********************************************************************************************************************

ISerializablePtr AbstractModelParameter::Create( DeserializeObject& dob )
{
    ITimeEvaluatorPtr te = dob.m_tm->GetRootTimeline();

    std::string name = dob.GetValue( "name" );

    auto values = dob.LoadProperties< KeyFrame >( "timeval" );

    if( values.size() == 0 )
        values.push_back( std::make_shared< KeyFrame >( "0", dob.GetValue( "value" ) ) );

    auto param = ParametersFactory::CreateParameterFloat( name, te ); // FIXME

    try
    {
        for( auto value : values )
        {
            float val = std::stof( value->value );
            float t = std::stof( value->time );
            param->SetVal( val , t );
        }
    }catch( std::invalid_argument & )
    {
        return ParametersFactory::CreateParameterBool( name, te ); // FIXME
    }

    return param;
}

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
