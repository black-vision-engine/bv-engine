#include "stdafx.h"

#include "DefaultVideoInputPlugin.h"
//#include "DefaultVideoInput.h"
#include "Engine\Models\Plugins\ParamValModel\ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "VideoInput/DefaultVideoInputResourceDescr.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultVideoInputPluginDesc::DefaultVideoInputPluginDesc                          ()
    : DefaultTexturePluginDesc( UID(), "video input", "tx" )
{}

// *******************************
//
IPluginPtr              DefaultVideoInputPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultVideoInputPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultVideoInputPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
	auto model = DefaultTexturePluginDesc::CreateDefaultModel( timeEvaluator );

	ModelHelper h( timeEvaluator, model );

	h.SetOrCreatePSModel();
	h.AddSimpleParam( "source", 0.0f );		//FIXME: integer parmeters should be used here

    return model;
}

// *******************************
//
std::string             DefaultVideoInputPluginDesc::UID                       ()
{
    return "DEFAULT_VIDEOINPUT";
}

// *******************************
//
std::string             DefaultVideoInputPluginDesc::VertexShaderSource        ()
{
    //return "../dep/media/shaders/DefaultVideoInput.vert";
    return NULL;
}

// *******************************
//
std::string             DefaultVideoInputPluginDesc::PixelShaderSource         ()
{
    //return "../dep/media/shaders/DefaultVideoInput.frag";
    return NULL;
}

// *******************************
//
std::string             DefaultVideoInputPluginDesc::TextureName               ()
{
    return "Tex0";
}

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultVideoInputPlugin::DefaultVideoInputPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : DefaultTexturePlugin( name, uid, prev, model )
{}

// *************************************
// 
DefaultVideoInputPlugin::~DefaultVideoInputPlugin(void)
{}


// *************************************
// 
int									DefaultVideoInputPlugin::GetSourceNumber()
{
    auto param = GetParameter( "source" );
    float fSource = QueryTypedParam< ParamFloatPtr > ( param )->Evaluate();
    return (int) fSource;
}

// *************************************
// 
void                                DefaultVideoInputPlugin::Update                      ( TimeType t )
{
	DefaultTexturePlugin::Update( t );

    auto txData = m_psc->GetTexturesDataImpl();
    assert( txData->GetTextures().size() == 1 );
}

// *************************************
// 
bool                                DefaultVideoInputPlugin::LoadResource                ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultTexturePluginDesc::TextureName() );
        if( txDesc != nullptr )
        {
			txDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
			txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

			auto txData = m_psc->GetTexturesDataImpl();
            txData->SetTexture( 0, txDesc );

            m_textureWidth = txDesc->GetWidth();
            m_textureHeight = txDesc->GetHeight();

            return true;
        }
    }

    auto viResDescr = QueryTypedDesc< DefaultVideoInputResourceDescrConstPtr >( assetDescr );

    if( viResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 ); // to be safe for now
        
        if( txData->GetTextures().size() == 0 )
            txData->AddTexture( viResDescr->GetITextureDescriptor() );
        //else
        //    txData->SetTexture( viResDescr->GetITextureDescriptor() );

        //desc.AddAndSetInputAsCurrent( viResDescr );

        return true;
    }

    return false;
}

} }