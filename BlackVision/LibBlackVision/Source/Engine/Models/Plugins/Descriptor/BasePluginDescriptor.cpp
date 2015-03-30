#include "BasePluginDescriptor.h"

namespace bv { namespace model {

// *********************************
//
BasePluginDescriptor::BasePluginDescriptor                      ( const std::string & uid, const std::string & defaultName )
    : m_uid( uid )
    , m_defaultName( defaultName )
{
}

// *********************************
//
const std::string &     BasePluginDescriptor::GetPluginTypeUID  () const
{
    return m_uid;
}

// *********************************
//
const std::string &     BasePluginDescriptor::DefaultPluginName () const
{
    return m_defaultName;
}

// *********************************
//
bool                    BasePluginDescriptor::CanBeAttachedTo   ( IPluginConstPtr plugin )  const
{
    //By default assume that plugin can be connected just to any other plugin
    return true;
}

// *********************************
//
IPluginParamValModelPtr BasePluginDescriptor::CreateModel       ( ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreateDefaultModel( timeEvaluator );
}

// *********************************
//
void BasePluginDescriptor::ModelHelper::CreateVacModel         ()
{
    auto vacModel = std::make_shared< DefaultParamValModel >();
    m_model->SetVertexAttributesChannelModel( vacModel );
    m_lastParamValModel = vacModel;
}

} //model
} //bv
