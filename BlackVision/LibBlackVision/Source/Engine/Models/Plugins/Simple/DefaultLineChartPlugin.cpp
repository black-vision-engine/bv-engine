#include "stdafx.h"
#include "DefaultLineChartPlugin.h"

#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "Assets/DataArray/DataArrayAsset.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string       DefaultLineChartPlugin::PARAMS::WIDTH   = "width";


const std::string       DefaultLineChartPlugin::AssetName       = "ChartData";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultLineChartPluginDesc::DefaultLineChartPluginDesc                          ()
    : DefaultGeometryPluginDescBase( UID(), "line chart" )
{
}

// *******************************
//
IPluginPtr              DefaultLineChartPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultLineChartPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultLineChartPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );
    h.SetOrCreateVacModel();

    h.SetOrCreatePSModel();

    return h.GetModel();
}

// *******************************
//
std::string             DefaultLineChartPluginDesc::UID                       ()
{
    return "DEFAULT_LINE_CHART";
}


// ========================================================================= //
// Generators
// ========================================================================= //


#include "Mathematics/Defines.h"
namespace DefaultLineChartPluginGenerator
{

    bool        CompareVec2 ( glm::vec2 & first, glm::vec2 & second )
    {
        if( first.x < second.x )
            return true;
        return false;
    }


    class MainGenerator : public IGeometryOnlyGenerator
    {
    protected:

        DataArrayAssetConstPtr  m_data;

    public:
        MainGenerator( DataArrayAssetConstPtr data )
            :   m_data( data )
        {}

        virtual ~MainGenerator()
        {
            m_data = nullptr;
        }


        virtual void GenerateGeometry( Float3AttributeChannelPtr verts ) override
        {
            if( !m_data->IsEmpty() )
            {
                auto plot = m_data->GetRow( "Plot" );
                if( plot->GetType() == ModelParamType::MPT_VEC2 )
                {
                    auto typedPlot = static_cast< DataArrayRow< glm::vec2 >* >( plot );

                    // Consciously coping vector.
                    auto plotPoints = typedPlot->GetArray();
                    std::sort( plotPoints.begin(), plotPoints.end(), CompareVec2 );

                    for( UInt32 i = 0; i < ( UInt32 )plotPoints.size() - 1; i++ )
                    {
                        verts->AddAttribute( glm::vec3( plotPoints[ i ], 0.0f ) );
                        verts->AddAttribute( glm::vec3( plotPoints[ i + 1 ], 0.0f ) );
                    }

                    return;
                }
            }

            // Engine demands non empty channel.
            verts->AddAttribute( glm::vec3( 0.0f, 0.0f, 0.0f ) );
            verts->AddAttribute( glm::vec3( 0.0f, 0.0f, 0.0f ) );
        }

    };
}


// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
DefaultLineChartPlugin::DefaultLineChartPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
:   DefaultGeometryPluginBase( name, uid, prev, model )
,   m_asset( nullptr )
{
    m_pluginParamValModel->Update();
    LoadResource( DefaultAssets::Instance().GetDefaultDesc< DataArrayAssetDesc >() );

    InitGeometry( PrimitiveType::PT_LINES );

    // LoadResource sets this bool to true. Than we call InitGeometry and update isn't necessary anymore.
    m_assetUpdated = false;
}

// *************************************
// 
DefaultLineChartPlugin::~DefaultLineChartPlugin         ()
{
    m_asset = nullptr;
}


std::vector<IGeometryGeneratorPtr>	DefaultLineChartPlugin::GetGenerators()
{
    std::vector<IGeometryGeneratorPtr> gens;
    
    gens.push_back( std::make_shared< DefaultLineChartPluginGenerator::MainGenerator >( m_asset ) );
    return gens;
}


// ***********************
//
bool                                DefaultLineChartPlugin::NeedsTopologyUpdate()
{
    auto & states = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetStates();
    for( auto iter = states.begin(); iter != states.end(); ++iter )
    {
        if( iter->second->StateChanged() )
        {
            return true;
        }
    }

    if( m_assetUpdated )
    {
        m_assetUpdated = false;
        return true;
    }

    return false;
}

// *************************************
// 
bool                                DefaultLineChartPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto daAssetDescr = QueryTypedDesc< DataArrayAssetBaseDescConstPtr >( assetDescr );

    if( daAssetDescr )
    {
        m_asset = LoadTypedAsset< DataArrayAsset >( daAssetDescr );

        SetAsset( 0, LAsset( AssetName, assetDescr, nullptr ) );
        m_assetUpdated = true;

        return true;
    }

    return false;
}



} // model
} // bv

