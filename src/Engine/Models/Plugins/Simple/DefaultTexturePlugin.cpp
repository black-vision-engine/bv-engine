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
    ParamFloat *  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachementMode" );

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
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultTexturePluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), false ) );
    m_vsc = DefaultPixelShaderChannelPtr( DefaultVertexShaderChannel::Create( DefaultTexturePluginDesc::VertexShaderSource(), model->GetPixelShaderChannelModel(), false ) );

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

//// *************************************
////
//SimpleTexturePlugin::SimpleTexturePlugin                    ( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, TextureAttachmentMode amode )
//    : BasePlugin( "dupa", "dupa", prev, nullptr )
//    , m_attachmentMode( amode )
//{
//    assert( prev != nullptr );
//
//    for(unsigned int i = 0; i < textureDescs.size(); ++i)
//    {
//        auto texInfo = LoadTexture( textureDescs[ i ], "Tex" + std::to_string( i ) );
//        m_textures.push_back( texInfo );
//    }
//
//    m_vaChannel = nullptr;
//    EvalGeometryChannel( prev );
//
//    // Set Pixel Shader Channel
//    ParamTransformVec           txMat( "TxParamTransform" );
//    std::vector<ParamFloat>     alphas;
//    std::vector<ParamVec4>      borderColors;
//
//    for( auto t : textureDescs )
//    {
//        txMat.AppendTransform( t.transform.GetTransformF() );
//        alphas.push_back( t.alpha );
//        borderColors.push_back( t.borderColor );
//    }
//
//    int texturesNum = textureDescs.size();
//
//    m_pixelShaderChannel = TexturePixelShaderChannelPtr( new TexturePixelShaderChannel( "../dep/media/shaders/simpletexture/simpletexture" + std::to_string( texturesNum ) +".frag"
//										, alphas
//										, txMat 
//                                        , borderColors
//                                        ) );
//
//    m_pixelShaderChannel->SetRendererContext( RendererContext::CreateDefault() );
//    auto rendContext = m_pixelShaderChannel->GetRendererContext();
//    rendContext->cullCtx->enabled = false;
//
//	m_vertexShaderChannel = TextureVertexShaderChannelPtr( new TextureVertexShaderChannel( "../dep/media/shaders/simpletexture/simpletexture" + std::to_string( texturesNum ) + ".vert" ) );
//}
//
//// *************************************
////
//SimpleTexturePlugin::SimpleTexturePlugin( const IPlugin * prev, const std::vector< const TextureDescriptor > & textureDescs, model::RendererContext * ctx, TextureAttachmentMode mode )
//    : BasePlugin( "dupa", "dupa", prev, nullptr )
//    , m_attachmentMode( mode )
//{
//    assert( prev != nullptr );
//
//    for( unsigned int i = 0; i < textureDescs.size(); ++i )
//    {
//        auto texInfo = LoadTexture( textureDescs[ i ], "Tex" + std::to_string( i ) );
//        m_textures.push_back( texInfo );
//    }
//
//    m_vaChannel = nullptr;
//    EvalGeometryChannel( prev );
//
//    // Set Pixel Shader Channel
//    // Set Pixel Shader Channel
//    ParamTransformVec           txMat( "TxParamTransform" );
//    std::vector<ParamFloat>     alphas;
//    std::vector<ParamVec4>      borderColors;
//
//    for( auto t : textureDescs )
//    {
//        txMat.AppendTransform( t.transform.GetTransformF() );
//        alphas.push_back( t.alpha );
//        borderColors.push_back( t.borderColor );
//    }
//
//    m_pixelShaderChannel = TexturePixelShaderChannelPtr( new TexturePixelShaderChannel( "../dep/media/shaders/simpletexture.frag"
//										, alphas
//										, txMat 
//                                        , borderColors ) );
//
//    if ( ctx )
//    {
//        m_pixelShaderChannel->SetRendererContext( ctx );
//    }
//    else
//    {
//        m_pixelShaderChannel->SetRendererContext( RendererContext::CreateDefault() );
//    }
//
//    auto rendContext = m_pixelShaderChannel->GetRendererContext();
//    rendContext->cullCtx->enabled = false;
//
//	m_vertexShaderChannel = TextureVertexShaderChannelPtr( new TextureVertexShaderChannel( "../dep/media/shaders/simpletexture.vert" ) );
//}
//
//
//// *************************************
////
//SimpleTexturePlugin::~SimpleTexturePlugin        ()
//{}
//
//// *************************************
////
//void                        SimpleTexturePlugin::SetAttachmentMode           ( TextureAttachmentMode mode )
//{
//    m_attachmentMode = mode;
//}
//
//// *************************************
////
//void SimpleTexturePlugin::EvalGeometryChannel( const IPlugin* prev )
//{
//    auto prevGeomChannel = prev->GetVertexAttributesChannel();
//    AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
//
//    for( unsigned int i = 0; i < prevGeomChannel->GetComponents().size(); ++i )
//    {
//        ConnectedComponent* connComp = new ConnectedComponent();
//        VertexAttributesChannelDescriptor vaChannelDesc;
//
//        auto prevConnComp = static_cast< const model::ConnectedComponent* >( prevGeomChannel->GetComponents()[ i ] );
//        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();
//
//        for( auto prevCompCh : prevCompChannels )
//        {
//            connComp->AddAttributeChannel( prevCompCh );
//        }
//
//        if( m_vaChannel == nullptr )
//        {
//            for( auto prevCompCh : prevCompChannels )
//            {
//                auto prevCompChDesc = prevCompCh->GetDescriptor();
//                vaChannelDesc.AddAttrChannelDesc( prevCompChDesc->GetType(), prevCompChDesc->GetSemantic(), prevCompChDesc->GetChannelRole()  );
//            }
//
//            m_texCoordChannelIndex = vaChannelDesc.GetNumVertexChannels();
//
//            for( unsigned int i = 0; i < m_textures.size(); ++i )
//            {
//                vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
//            }
//
//            auto vaChannel = VertexAttributesChannelPtr( new VertexAttributesChannel( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() ) );
//            m_vaChannel = vaChannel;
//        }
//
//        for( unsigned int i = 0; i < m_textures.size(); ++i )
//        {
//
//            float minX = 100000.0f, minY = 100000.0f;
//            float maxX = 0.0f, maxY = 0.0f;
//
//            //convex hull - make sure that prevCompChannels[ 0 ] is indeed a positional channel
//            for( unsigned int j = 0; j < prevCompChannels[ 0 ]->GetNumEntries(); ++j )
//            {
//                const glm::vec3 * pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
//
//                minX = std::min( minX, pos[ j ].x );
//                minY = std::min( minY, pos[ j ].y );
//                maxX = std::max( maxX, pos[ j ].x );
//                maxY = std::max( maxY, pos[ j ].y );
//            }
//
//            auto verTexAttrChannel = new model::Float2AttributeChannel( desc, m_textures[ 0 ]->m_texName, true );
//
//            for( unsigned int j = 0; j < prevCompChannels[0]->GetNumEntries(); ++j )
//            {
//                const glm::vec3* pos = reinterpret_cast<const glm::vec3*>( prevCompChannels[0]->GetData() );
//                verTexAttrChannel->AddAttribute( glm::vec2( ( pos[ j ].x - minX ) / ( maxX - minX ), ( pos[ j ].y - minY ) / ( maxY - minY ) ) );
//            }
//
//            connComp->AddAttributeChannel( AttributeChannelPtr( verTexAttrChannel ) );
//        }
//
//        m_vaChannel->AddConnectedComponent( connComp );
//    }
//}
//
//// *************************************
////
//TextureInfo * SimpleTexturePlugin::LoadTexture( const TextureDescriptor & texDesc, const std::string& name ) const
//{
//    TextureLoader texLoader;
//
//    Resource texture( name, texDesc.fileName );
//
//    return new TextureInfo(     texLoader.LoadResource( &texture )
//                            ,   name
//                            ,   texDesc.transform
//                            ,   texDesc.alpha
//                            ,   texDesc.borderColor
//                            ,   texDesc.wrappingModeX
//                            ,   texDesc.wrappingModeY
//                            ,   texDesc.filteringMode );
//}
//
//// *************************************
////
//void                                    SimpleTexturePlugin::Update              ( TimeType t )
//{
//    if( m_attachmentMode == TextureAttachmentMode::MM_FREE )
//    {
//        if( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
//        {
//            for( unsigned int i = 0; i < m_vaChannel->GetComponents().size(); ++i )
//            {
//                auto connComp = static_cast< const model::ConnectedComponent* >( m_vaChannel->GetComponents()[ i ] );
//                auto compChannels = connComp->GetAttributeChannels();
//
//                if( auto posChannel = AttributeChannel::GetPositionChannel( compChannels ) )
//                    if( auto uvChannel = AttributeChannel::GetUVChannel( compChannels, m_texCoordChannelIndex ) )
//                    {
//                        auto & verts  = dynamic_cast< Float3AttributeChannel* >(posChannel)->GetVertices();
//                        auto & uvs    = dynamic_cast< Float2AttributeChannel* >(uvChannel)->GetVertices();
//
//                        for( unsigned int i = 0; i < verts.size(); ++i )
//                        {
//                            uvs[ i ].x = verts[ i ].x;
//                            uvs[ i ].y = verts[ i ].y;
//                        }
//                    }
//            }
//        }
//
//    }
//
//    if ( m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
//    {
//        m_vaChannel->SetNeedsAttributesUpdate( true );
//    }
//
//    for( auto ti : m_textures )
//    {
//        ti->m_texTransformVal->SetValue( ti->m_texTransform.Evaluate( t ) );
//        ti->m_texAlphaVal->SetValue( ti->m_texAlpha.Evaluate( t ) );
//        ti->m_texBorderColorVal->SetValue( ti->m_texBorderColor.Evaluate( t ) );
//    }
//
////    m_vaChannel->Update( t );
////    m_pixelShaderChannel->Update( t );
////    m_vertexShaderChannel->Update( t );
//
//    //FIXME: update chanels according to parent (e.g. when position data has been changed)
////    m_alphaValue->SetValue( m_alphaParam->Evaluate( t ) );
////    m_tex0TransformValue->SetValue( m_tex0TransformParam->Evaluate( t ) );
////    m_tex1TransformValue->SetValue( m_tex1TransformParam->Evaluate( t ) );
//}
//
//// *************************************
////
//void                                    SimpleTexturePlugin::Print                      ( std::ostream & out, int tabs ) const
//{
//    out << GetName() << debug::EndLine( tabs );
//    for( auto t : m_textures )
//    {
//        out << "Texture: " << t->m_texName << debug::EndLine( tabs );
//    }
//}
//
//// *************************************
////
//const IVertexAttributesChannel *        SimpleTexturePlugin::GetVertexAttributesChannel () const
//{
//    return m_vaChannel.get();
//}
//
//// *************************************
////
//const IPixelShaderChannel *             SimpleTexturePlugin::GetPixelShaderChannel      () const
//{
//    return m_pixelShaderChannel.get();
//}
//
//// *************************************
////
//const IVertexShaderChannel *            SimpleTexturePlugin::GetVertexShaderChannel     () const
//{
//    return m_vertexShaderChannel.get();
//}
//
//// *************************************
////
//TextureInfoVec                          SimpleTexturePlugin::GetTextures                () const
//{
//    auto prevTextures = m_prevPlugin->GetTextures();
//    prevTextures.insert( prevTextures.end(), m_textures.begin(), m_textures.end() );
//    return prevTextures;
//}

} // model
} // bv
