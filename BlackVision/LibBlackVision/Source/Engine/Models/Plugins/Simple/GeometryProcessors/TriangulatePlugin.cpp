#include "stdafx.h"

#include "UseLoggerLibBlackVision.h"

#include "TriangulatePlugin.h"
#include "Triangulator.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



namespace bv { namespace model {

// ***********************
//
TriangulatePluginDesc::TriangulatePluginDesc()
    : BasePluginDescriptor( "TRIANGULATE", "triangulate" )
{
}

// ***********************
//
DefaultPluginParamValModelPtr   TriangulatePluginDesc::CreateDefaultModel      ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper model( timeEvaluator );

    return model.GetModel();
}

// ***********************
//
IPluginPtr                      TriangulatePluginDesc::CreatePlugin            ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< TriangulatePlugin >( name, prev, timeEvaluator );
}


// ***********************
//
TriangulatePlugin::TriangulatePlugin	        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultGeometryProcessorBase( name, uid, prev, model )
{
}

// ***********************
//
void TriangulatePlugin::InitializeVertexAttributesChannel()
{
    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();

	VertexAttributesChannelDescriptor vaChannelDesc;
    vaChannelDesc.AddAttrChannelDesc( std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_PROCESSOR ) );
    vaChannelDesc.AddAttrChannelDesc( std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_PROCESSOR ) );
    m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_TRIANGLES, vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
}


// ***********************
//
void        TriangulatePlugin::ProcessVertexAttributesChannel()
{
    if( !( m_prevPlugin
           && m_prevPlugin->GetVertexAttributesChannel()
           && m_prevPlugin->GetVertexAttributesChannel()->GetPrimitiveType() == PrimitiveType::PT_LINES ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();

    if( !m_vaChannel )
    {
        InitializeVertexAttributesChannel();        
    }
    else
    {
        m_vaChannel->ClearAll();
    }

    auto prevComponents = prevGeomChannel->GetComponents();

    if( prevComponents.size() )
    {
        for( int j = 0; j < prevComponents.size(); ++j )
        {
            ContoursList contours = ExtractContours( prevComponents[ j ] );

            try
            {
                auto connComp = ConnectedComponent::Create();
                auto vertsDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_PROCESSOR );
                auto normDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_PROCESSOR );

                auto vertChannel = std::make_shared< Float3AttributeChannel >( vertsDesc, "vert", false );
                auto normChannel = std::make_shared< Float3AttributeChannel >( normDesc, "norm", false );

                Triangulator triangulator( std::move( contours ) );
                auto mesh = triangulator.MakeMesh();

                vertChannel->ReplaceAttributes( std::move( mesh.GetMeshSegments()[ 0 ] ) );
                normChannel->GetVertices().resize( vertChannel->GetNumEntries(), glm::vec3( 0.0, 0.0, 1.0 ) );

                connComp->AddAttributeChannel( vertChannel );
                connComp->AddAttributeChannel( normChannel );
                m_vaChannel->AddConnectedComponent( connComp );
            }
            catch( const std::runtime_error & error )
            {
                LOG_MESSAGE( SeverityLevel::error ) << error.what();
            }
        }
    }
}

// ***********************
//
ContoursList    TriangulatePlugin::ExtractContours             ( IConnectedComponentPtr& component )
{
    auto currComponent = std::static_pointer_cast<ConnectedComponent>( component );
    auto chan = std::dynamic_pointer_cast<Float3AttributeChannel>( currComponent->GetAttrChannel( AttributeSemantic::AS_POSITION ) );

    auto & data = chan->GetVertices();

	if( data.size() == 1 )
	{
		return ContoursList();
	}

    assert( data.size() % 2 == 0 );

    ContoursList contours;

    if( !data.empty() )
    {
        FTContourPtr contour = std::make_shared< FTContour >( true );
        for( int i = 0; i <= data.size(); i += 2 )
        {
            if( i == 0 || ( i < data.size() && data[ i - 1 ] == data[ i ] ) )
            {
                contour->AddPoint( FTPoint( data[ i ].x, data[ i ].y ) );
            }
            else
            {
                contours.push_back( std::move( contour ) );

                // One contour ended. We make new contour.
                if( i < data.size() )
                {
                    contour = std::make_shared< FTContour >( true );

                    // Remember about first point.
                    contour->AddPoint( FTPoint( data[ i ].x, data[ i ].y ) );
                }
            }
        }
    }

    return contours;
}

} }
