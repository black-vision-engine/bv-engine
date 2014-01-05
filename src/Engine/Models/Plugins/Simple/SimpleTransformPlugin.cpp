#include "SimpleTransformPlugin.h"

#include <cassert>

#include "System/Print.h"

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Transform/SimpleTransformChannel.h"

#include "Engine/Models/Plugins/Parameters/Parameter.h"
#include "Engine/Events/Interfaces/IEventManager.h"


namespace bv { namespace model {


// ***************************** PLUGIN ********************************** 

// *************************************
//
SimpleTransformPlugin*  SimpleTransformPlugin::Create( const IPlugin * prev, const ParamTransform& trans )
{
    auto p = new SimpleTransformPlugin( prev, trans );
    return p;
}

// *************************************
//
SimpleTransformPlugin::SimpleTransformPlugin                    ( const IPlugin * prev, const ParamTransform& trans )
    : BasePlugin( prev )
{
    m_transformChannel = new model::SimpleTransformChannel();
    m_transformChannel->AddTransform( trans );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &SimpleTransformPlugin::OnSetTransform ), TransformSetEvent::Type() );
}

// *************************************
//
SimpleTransformPlugin::~SimpleTransformPlugin   ()
{
}

// *************************************
//
const ITransformChannel *       SimpleTransformPlugin::GetTransformChannel         () const
{
    if( m_prevPlugin && m_prevPlugin->GetTransformChannel() )
    {
        //TODO:

        assert(!"prev plugin transform channel is not empty");
    }

    return m_transformChannel;
}

// *************************************
//
void                SimpleTransformPlugin::Update              ( TimeType t )
{
    m_transformChannel->Update( t );
}

// *************************************
//
void                SimpleTransformPlugin::Print               ( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
}

// *************************************
//FIXME: JEDEN KURWA WIELKI HACK
void                SimpleTransformPlugin::OnSetTransform      ( IEventPtr evt )
{
    TransformSetEventPtr tevt = std::static_pointer_cast<TransformSetEvent>( evt );

    const glm::vec3 & translation   = tevt->Translation();
    const glm::vec3 & scale         = tevt->Scale();

    ParamTransform & pt = m_transformChannel->AccessFirstParamTransform(); //FIXME: kolejny kurwa mega hack
    TransformF & t = pt.TransformRef();

    for( unsigned int i = 0; i < t.Size(); ++i )
    {
        const auto st = t[ i ];
        
        if( st->KindKurwaMac() == TransformKind::scale )
        {
            st->SetValues( 0.f, scale[ 0 ], scale[ 1 ], scale[ 2 ] );
        }

        if( st->KindKurwaMac() == TransformKind::translation )
        {
            st->SetValues( 0.f, translation[ 0 ], translation[ 1 ], translation[ 2 ] );
        }
    }
}

} // model
} // bv
