#include "stdafx.h"
#include "DefaultExtrudePlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {


const std::string        DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR           = "extrude vector";
const std::string        DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE   = "smooth threshold angle";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultExtrudePluginDesc::DefaultExtrudePluginDesc                          ()
    : DefaultGeometryProcessorDescBase( UID(), "extrude" )
{}

// *******************************
//
IPluginPtr              DefaultExtrudePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultExtrudePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultExtrudePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model = helper.GetModel();
    
    helper.CreateVacModel();
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR, glm::vec3( 0.0, 0.0, -0.3 ), true, true );
    helper.AddSimpleParam( DefaultExtrudePlugin::PARAMS::SMOOTH_THRESHOLD_ANGLE, 160.0f, true, true );

    return model;
}

// *******************************
//
std::string             DefaultExtrudePluginDesc::UID                       ()
{
    return "DEFAULT_EXTRUDE_PLUGIN";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultExtrudePlugin::DefaultExtrudePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultGeometryProcessorBase( name, uid, prev, model )
{
    SetPrevPlugin( prev );
}

// *************************************
// 
DefaultExtrudePlugin::~DefaultExtrudePlugin         ()
{}

// ***********************
//
void        DefaultExtrudePlugin::ProcessConnectedComponent       ( model::ConnectedComponentPtr & currComponent,
                                                                    std::vector< IConnectedComponentPtr > & /*allComponents*/,
                                                                    PrimitiveType topology )
{
    glm::vec3 translate = QueryTypedValue< ValueVec3Ptr >( GetValue( DefaultExtrudePlugin::PARAMS::EXTRUDE_VECTOR ) )->GetValue();

    auto positions = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );
    auto normals = std::static_pointer_cast< Float3AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_NORMAL ) );
    auto uvs = std::static_pointer_cast< Float2AttributeChannel >( currComponent->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) );

    assert( positions );    if( !positions ) return;
    
    auto posChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    auto newPositions = std::make_shared< model::Float3AttributeChannel >( posChannelDesc, positions->GetName(), true );

    auto connComp = ConnectedComponent::Create();
    connComp->AddAttributeChannel( newPositions );


    if( topology == PrimitiveType::PT_TRIANGLE_STRIP )
    {
        int numVerticies = positions->GetNumEntries();

        newPositions->GetVertices().reserve( 2 * numVerticies + 2 );

        for( int i = 0; i < numVerticies; ++i )
        {
            newPositions->AddAttribute( positions->GetVertices()[ i ] );
        }

        newPositions->AddAttribute( positions->GetVertices()[ numVerticies - 1 ] );
        newPositions->AddAttribute( translate + positions->GetVertices()[ numVerticies - 1 ] );

        for( int i = numVerticies - 1; i >= 0; --i )
        {
            newPositions->AddAttribute( translate + positions->GetVertices()[ i ] );
        }

        if( normals )
        {
            auto normChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
            auto newNormal = std::make_shared< model::Float3AttributeChannel >( normChannelDesc, normals->GetName(), true );
            newNormal->GetVertices().reserve( 2 * numVerticies + 2 );

            connComp->AddAttributeChannel( newNormal );

            for( int i = 0; i < numVerticies; ++i )
            {
                newNormal->AddAttribute( normals->GetVertices()[ i ] );
            }

            newNormal->AddAttribute( normals->GetVertices()[ numVerticies - 1 ] );
            newNormal->AddAttribute( -normals->GetVertices()[ numVerticies - 1 ] );

            for( int i = numVerticies - 1; i >= 0; --i )
            {
                newNormal->AddAttribute( -normals->GetVertices()[ i] );
            }
        }

        if( uvs )
        {
            auto uvChannelDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
            auto newUVs = std::make_shared< model::Float2AttributeChannel >( uvChannelDesc, uvs->GetName(), true );
            newUVs->GetVertices().reserve( 2 * numVerticies + 2 );

            connComp->AddAttributeChannel( newUVs );

            for( int i = 0; i < numVerticies; ++i )
            {
                newUVs->AddAttribute( uvs->GetVertices()[ i ] );
            }

            newUVs->AddAttribute( uvs->GetVertices()[ numVerticies - 1 ] );
            newUVs->AddAttribute( uvs->GetVertices()[ numVerticies - 1 ] );

            for( int i = numVerticies - 1; i >= 0; --i )
            {
                newUVs->AddAttribute( uvs->GetVertices()[ i ] );
            }
        }

    }
    else
    {
        assert( !"This primitive topology is not supported yet" );
    }

    m_vaChannel->AddConnectedComponent( connComp );
}


} // model
} // bv

