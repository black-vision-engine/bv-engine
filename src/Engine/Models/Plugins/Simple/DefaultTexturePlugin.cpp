#include "DefaultTexturePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

#include "Engine/Models/Resources/IPluginResourceDescr.h"


namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTexturePluginDesc::DefaultTexturePluginDesc                          ()
    : BasePluginDescriptor( UID(), "texture" )
{
}

// *******************************
//
IPlugin *               DefaultTexturePluginDesc::CreatePlugin              ( const std::string & name, const IPlugin * prev ) const
{
    return CreatePluginTyped< DefaultTexturePlugin >( name, prev );
}

// *******************************
//
DefaultPluginParamValModel *    DefaultTexturePluginDesc::CreateDefaultModel() const
{
    //Create all models
    DefaultPluginParamValModel * model  = new DefaultPluginParamValModel();
    DefaultParamValModel * psModel      = new DefaultParamValModel();
    DefaultParamValModel * vsModel      = new DefaultParamValModel();
   
    //Create all parameters and evaluators
    SimpleVec4Evaluator *      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor" );
    SimpleFloatEvaluator *     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha" );
    SimpleTransformEvaluator * trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat" );

    ParamFloat *  paramWrapModeX     = ParametersFactory::CreateParameterFloat( "wrapModeX" );
    ParamFloat *  paramWrapModeY     = ParametersFactory::CreateParameterFloat( "wrapModeY" );
    ParamFloat *  paramFilteringMode = ParametersFactory::CreateParameterFloat( "filteringMode" );
    ParamFloat *  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachmentMode" );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    psModel->AddParameter( paramWrapModeX );
    psModel->AddParameter( paramWrapModeY );
    psModel->AddParameter( paramFilteringMode );
    psModel->AddParameter( paramAttachMode );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT( TimeType( 0.0 ) );

    //FIXME: integer parmeters should be used here
    paramWrapModeX->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramWrapModeY->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramFilteringMode->SetVal( (float) TextureFilteringMode::TFM_LINEAR, TimeType( 0.f ) );
    paramAttachMode->SetVal( (float) TextureAttachmentMode::MM_ATTACHED, TimeType( 0.f ) );

    return model;
}

// *******************************
//
std::string             DefaultTexturePluginDesc::UID                       ()
{
    return "DEFAULT_TEXTURE";
}

// *******************************
//
std::string             DefaultTexturePluginDesc::VertexShaderSource        ()
{
    return "../dep/media/shaders/defaulttexture.vert";
}

// *******************************
//
std::string             DefaultTexturePluginDesc::PixelShaderSource         ()
{
    return "../dep/media/shaders/defaulttexture.frag";
}


//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
DefaultTexturePlugin::DefaultTexturePlugin         ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultTexturePluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultTexturePluginDesc::VertexShaderSource(), model->GetVertexShaderChannelModel() ) );

    InitGeometryChannel( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();
    
    m_paramWrapModeX        = QueryTypedParam< ParamFloat >( psModel->GetParameter( "wrapModeX" ) );
    m_paramWrapModeY        = QueryTypedParam< ParamFloat >( psModel->GetParameter( "wrapModeY" ) );
    m_paramFilteringMode    = QueryTypedParam< ParamFloat >( psModel->GetParameter( "filteringMode" ) );
    m_paramAttachMode       = QueryTypedParam< ParamFloat >( psModel->GetParameter( "attachmentMode" ) );

    assert( m_paramWrapModeX );
    assert( m_paramWrapModeY );
    assert( m_paramFilteringMode );
    assert( m_paramAttachMode );

    m_lastTextureWrapModeX;
    m_lastTextureWrapModeY;
    m_lastTextureFilteringMode;
    m_lastTextureAttachMode;

}

// *************************************
// 
bool                            DefaultTexturePlugin::LoadResource  ( const IPluginResourceDescr * resDescr )
{
    auto txResDescr = QueryTextureResourceDescr( resDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txResDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 1 );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txResDescr->GetTextureFile(), txResDescr->GetName() );

        if( txDesc != nullptr )
        {
            if( txData->GetTextures().size() == 0 )
            {
                txData->AddTexture( txDesc );
            }
            else
            {
                txData->SetTexture( 0, txDesc );
            }
            return true;
        }
    }

    return false;
}

// *************************************
// 
const IVertexAttributesChannel *    DefaultTexturePlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel.get();
}

// *************************************
// 
const IPixelShaderChannel *         DefaultTexturePlugin::GetPixelShaderChannel       () const
{
    return m_psc.get();
}

// *************************************
// 
const IVertexShaderChannel *        DefaultTexturePlugin::GetVertexShaderChannel      () const
{
    return m_vsc.get();
}

// *************************************
// 
void                                DefaultTexturePlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update( t );

    auto attachmentMode = GetAttachementMode( t );

    if( attachmentMode == TextureAttachmentMode::MM_FREE )
    {
        if( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
        {
            for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
            {
                auto connComp = static_cast< const model::ConnectedComponent* >( m_vaChannel->GetComponents()[ i ] );
                auto compChannels = connComp->GetAttributeChannels();

                if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
                    if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, m_texCoordChannelIndex ) )
                    {
                        auto & verts  = dynamic_cast< Float3AttributeChannel* >(posChannel)->GetVertices();
                        auto & uvs    = dynamic_cast< Float2AttributeChannel* >(uvChannel)->GetVertices();

                        for( unsigned int i = 0; i < verts.size(); ++i )
                        {
                            uvs[ i ].x = verts[ i ].x;
                            uvs[ i ].y = verts[ i ].y;
                        }
                    }
            }
        }

    }

    if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
    {
        m_vaChannel->SetNeedsAttributesUpdate( true );
    }

    m_vsc->PostUpdate();
    m_psc->PostUpdate();

//    m_vaChannel->Update( t );
//    m_pixelShaderChannel->Update( t );
//    m_vertexShaderChannel->Update( t );

    //FIXME: update chanels according to parent (e.g. when position data has been changed)
//    m_alphaValue->SetValue( m_alphaParam->Evaluate( t ) );
//    m_tex0TransformValue->SetValue( m_tex0TransformParam->Evaluate( t ) );
//    m_tex1TransformValue->SetValue( m_tex1TransformParam->Evaluate( t ) );
    
}

namespace {

// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloat * param, TimeType t )
{
    int val = int( param->Evaluate( t ) );

    return TextureWrappingMode( val );
}

} //anonymous

// *************************************
// 
TextureWrappingMode                         DefaultTexturePlugin::GetWrapModeX          ( TimeType t ) const
{
    return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeX, t );
}

// *************************************
// 
TextureWrappingMode                         DefaultTexturePlugin::GetWrapModeY          ( TimeType t ) const
{
    return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeY, t );
}

// *************************************
// 
TextureFilteringMode                        DefaultTexturePlugin::GetFilteringMode      ( TimeType t ) const
{
    return EvaluateAsInt< TextureFilteringMode >( m_paramFilteringMode, t );
}

// *************************************
// 
TextureAttachmentMode                       DefaultTexturePlugin::GetAttachementMode    ( TimeType t ) const
{
    return EvaluateAsInt< TextureAttachmentMode >( m_paramAttachMode, t );
}

// *************************************
// 
bool                                        DefaultTexturePlugin::StateChanged                ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am ) const
{
    return wmX != m_lastTextureWrapModeX || wmY != m_lastTextureWrapModeY || fm != m_lastTextureFilteringMode || am != m_lastTextureAttachMode;
}

// *************************************
// 
bool                                        DefaultTexturePlugin::UpdateState                 ( TextureWrappingMode wmX, TextureWrappingMode wmY, TextureFilteringMode fm, TextureAttachmentMode am )
{
    m_lastTextureWrapModeX      = wmX;
    m_lastTextureWrapModeY      = wmY;
    m_lastTextureFilteringMode  = fm;
    m_lastTextureAttachMode     = am;
}

} // model
} // bv
