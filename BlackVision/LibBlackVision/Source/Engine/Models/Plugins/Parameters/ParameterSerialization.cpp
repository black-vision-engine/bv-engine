#include "AbstractModelParameter.h"
#include "ParametersFactory.h"
#include "SimpleTypedParameters.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include <sstream>

#include "Serialization/SerializationObjects.inl" // FIXME there is no apparent reason for this to be here

namespace bv { namespace model {

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

class KeyFrame : public ISerializable // FIXME: to remove!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
public:
    std::string time, value; // FIXME

    KeyFrame( std::string t, std::string v ) : time( t ), value( v ) {}

    virtual void                Serialize       ( SerializeObject &/*doc*/ ) const {}
    static ISerializablePtr     Create          ( DeserializeObject &doc )
    {
        auto time = doc.GetValue( "time" );
        auto value = doc.GetValue( "val" );
        return std::make_shared< KeyFrame >( time, value );
    }
};


// ********************************************************************************************************************

ISerializablePtr AbstractModelParameter::Create( DeserializeObject& dob ) // FIXME: rethink if is might be done cleaner
{
    auto name = dob.GetValue( "name" );
    auto type = dob.GetValue( "type" );
    //ITimeEvaluatorPtr te = dob.m_tm->GetRootTimeline();
    auto timeline = dob.GetValue( "timeline" );
    ITimeEvaluatorPtr te = dob.GetTimelineManager()->GetTimeline( timeline );
    
    auto values = dob.LoadArray< KeyFrame >( "interpolator" );

    if( type == "float" )
    {
        auto param = ParametersFactory::CreateParameterFloat( name, te );
        
        for( auto value : values )
        {
            float val = std::stof( value->value );
            float t = std::stof( value->time );
            param->SetVal( val , t );
        }

        return param;
    } else if( type == "4" || type == "vec4" )
    {
        auto param = ParametersFactory::CreateParameterVec4( name, te );
        
        for( auto value : values )
        {
            auto vals = split( value->value, ',' );
            assert( vals.size() == 4 );
            glm::vec4 val( std::stof( vals[0] ), 
                std::stof( vals[1] ), 
                std::stof( vals[2] ), 
                std::stof( vals[3] ) );
            float t = std::stof( value->time );
            param->SetVal( val , t );
        }

        return param;
    } else if( type == "6" || type == "transform_vec" ) // FIXME: this should be made a constant somewhere
    {
        auto param = ParametersFactory::CreateParameterTransformVec( name, te );

        auto transes = dob.LoadProperties< TransformF >( "composite_transform" );

        int i = 0;
        for( auto trans : transes )
        {
            param->InsertTransform( i++, *trans.get() ); // FIXME (?)
        }
        
        return param;
    }

    //assert( false ); // FIXME
    return ParametersFactory::CreateParameterBool( name, te );

    //auto values = dob.LoadProperties< KeyFrame >( "timeval" );

    //if( values.size() == 0 )
    //    values.push_back( std::make_shared< KeyFrame >( "0", dob.GetValue( "value" ) ) );

    //auto param = ParametersFactory::CreateParameterFloat( name, te ); // FIXME

    //try
    //{
    //    for( auto value : values )
    //    {
    //        float val = std::stof( value->value );
    //        float t = std::stof( value->time );
    //        param->SetVal( val , t );
    //    }
    //}catch( std::invalid_argument & )
    //{
    //    return ParametersFactory::CreateParameterBool( name, te ); // FIXME
    //}

    //return param;
}

} }