#include "Engine/Models/BasicNode.h"
#include "Engine/Models/BoundingVolume.h"
#include "UpdatersHelpers.h"

#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h" // FIXME



namespace bv {

namespace {

// *******************************
//
template< typename ContextType, typename StateTypePtr >
inline void UpdateState( const ContextType * ctxState, StateTypePtr rendererState )
{
    if( ctxState )
    {
        assert( rendererState );

        RendererStatesBuilder::Assign( rendererState, ctxState );
    }
}

} //anonymous


// *****************************
//
inline  void    NodeUpdater::UpdateTransform     ()
{
    //FIXME: rili rili
    m_renderable->ResetLocalTransform();

    const auto & transform = m_transformChannel->GetTransformValue();
    if( transform )
    {
        const glm::mat4 & mat = transform->GetValue();

        m_renderable->SetLocalTransform( m_renderable->LocalTransform().Matrix() * mat );

        auto state = Cast< model::SimpleState< glm::mat4 >* >( m_transformStatedValue.get() );
        state->Update( mat );
    }
}

// *****************************
//
inline  void    NodeUpdater::UpdateGeometry      ()
{
    if( m_vertexAttributesChannel->GetTopologyUpdateID() > m_topologyUpdateID )
    {
        UpdateTopology();

        m_topologyUpdateID = m_vertexAttributesChannel->GetTopologyUpdateID();
        m_attributesUpdateID = m_vertexAttributesChannel->GetAttributesUpdateID();

		UpdateBoundingBox();
    }
    else if( m_vertexAttributesChannel->GetAttributesUpdateID() > m_attributesUpdateID )
    {
        UpdatePositions();

        m_attributesUpdateID = m_vertexAttributesChannel->GetAttributesUpdateID();	

		UpdateBoundingBox();
    }
}

// *****************************
//
inline  void    NodeUpdater::UpdateRendererState ()
{
    if ( m_rendererContext->StateChanged() )
    {
        for( auto stateInstance : m_redererStateInstanceVec )
        {
            UpdateState( m_rendererContext->alphaCtx, RenderStateAccessor::AccessAlphaState( stateInstance ) );
            UpdateState( m_rendererContext->cullCtx, RenderStateAccessor::AccessCullState( stateInstance ) );
            UpdateState( m_rendererContext->depthCtx, RenderStateAccessor::AccessDepthState( stateInstance ) );
            UpdateState( m_rendererContext->fillCtx, RenderStateAccessor::AccessFillState( stateInstance ) );
        }

        m_rendererContext->SetStateChanged( false );
    }
}

namespace {

// *****************************
//
inline  void    UpdatePositionsImpl     ( RenderableEntity * m_renderable, model::IVertexAttributesChannelConstPtr vaChannel )
{
    //FIXME: implement for other types of geometry as well
    assert( m_renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP || 
        m_renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLES );

    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessary
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_renderable->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = rad->VAO                  (); 
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
    // const VertexDescriptor * vd         = vao->GetVertexDescriptor  ();

    auto components = vaChannel->GetComponents();
    auto geomDesc   = vaChannel->GetDescriptor();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P  TYM RAZEM KURWA PODWOJNA, BO TU NAPRAWDE ZACZYNA SIE ROBIC BURDEL
    unsigned int currentOffset = 0;

    unsigned int k = 0;

    for( auto cc : components )
    {
        //This is update only, so the number of vertices must match
        assert( cc->GetNumVertices() == vao->GetNumVertices( k ) );
 
        UpdatersHelpers::WriteVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();

        ++k;
    }

    vao->SetNeedsUpdateMemUpload( true );
}
//
//// *****************************
////
//inline  void    UpdateBoxPositions     ( RenderableEntity * renderable, model::IConnectedComponentPtr cc )
//{
//    assert( renderable->GetType() == RenderableEntity::RenderableType::RT_LINES );
//
//    RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( renderable->GetRenderableArrayData() );
//
//    VertexArraySingleVertexBuffer * vao = rad->VAO                  (); 
//    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
//
//    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P  TYM RAZEM KURWA PODWOJNA, BO TU NAPRAWDE ZACZYNA SIE ROBIC BURDEL
//
//    //This is update only, so the number of vertices must match
//    assert( cc->GetNumVertices() == vao->GetNumVertices( 0 ) );
// 
//    UpdatersHelpers::WriteVertexDataToVBO( vbData, cc );
//
//    vao->SetNeedsUpdateMemUpload( true );
//}

inline void UpdateTopologyImpl( RenderableEntity * renderable, model::IVertexAttributesChannelConstPtr vaChannel )
{
    //FIXME: implement for other types of geometry as well
    assert( renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP || 
            renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLES ||
            renderable->GetType() == RenderableEntity::RenderableType::RT_LINES );

    //FIXME: if this is the last update then STATIC semantic should be used but right now it's irrelevant
    DataBuffer::Semantic vbSemantic = DataBuffer::Semantic::S_DYNAMIC;

    auto components = vaChannel->GetComponents();
    auto geomDesc   = vaChannel->GetDescriptor();

    if( components.empty() )
    {
        //assert( false ); //FIXME: at this point empty geometry is not allowed
        return;
    }

    unsigned int totalNumVertivces = vaChannel->TotalNumVertices();
    
	if( totalNumVertivces == 0 )
	{
		//assert( false ); //FIXME: at this point empty geometry is not allowed
		return;
	}

    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessary
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * radasvb = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( renderable->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = radasvb->VAO              ();
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();

    vb->Reinitialize( totalNumVertivces, geomDesc->SingleVertexEntrySize(), vbSemantic );
    vao->ResetState();

    // recreate vertex descriptor
    VertexDescriptor * vd = new VertexDescriptor( geomDesc->GetNumVertexChannels() );
    unsigned int attributeOffset = 0;

    for( unsigned int i = 0; i < geomDesc->GetNumVertexChannels(); ++i )
    {
        auto channelDesc = geomDesc->GetAttrChannelDescriptor( i );
        vd->SetAttribute( i, i, attributeOffset, channelDesc->GetType(), channelDesc->GetSemantic() );
        attributeOffset += channelDesc->GetEntrySize();     
    }

    vd->SetStride( attributeOffset );
    vao->SetVertexDescriptor( vd );


    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P
    unsigned int currentOffset = 0;

    for( auto cc : components )
    {
        assert( !cc->GetAttributeChannels().empty() );

        vao->AddCCEntry( cc->GetNumVertices() );

        UpdatersHelpers::WriteVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();
    }
    
    vao->SetNeedsUpdateRecreation( true );
}

} //anonymous

// *****************************
//
inline  void    NodeUpdater::UpdatePositions     ()
{
    UpdatePositionsImpl( m_renderable, m_vertexAttributesChannel );
}

// *****************************
//
inline  void    NodeUpdater::UpdateTopology      ()
{
    UpdateTopologyImpl( m_renderable, m_vertexAttributesChannel );
}

// *****************************
//
inline void     NodeUpdater::UpdateShaderParams				()
{
    for( auto & pair : m_paramsMappingVec )
    {
        UpdateGenericShaderParam( pair.first.get(), pair.second );
    }
}

// *****************************
//
inline void		NodeUpdater::UpdateTexturesData				()
{
    for( unsigned int txIdx = 0; txIdx < ( unsigned int )m_texDataMappingVec.size(); ++txIdx )
    {
        auto txDataPair		= m_texDataMappingVec [ txIdx ];
        auto texData        = txDataPair.first;
        auto shaderParams   = txDataPair.second;
    
        auto textures   = texData->GetTextures();
        auto animations = texData->GetAnimations();

        //FIXME: make sure that textures and animations from model are passed in this exact order (textures first and animations afterwards)
        unsigned int j = 0;
        for( unsigned int i = 0; i < textures.size(); ++i, ++j )
        {
            auto texDesc    = textures[ i ];

            if( m_texDataUpdateID[ txIdx ][ j ] < texDesc->GetUpdateID() )
            {
                auto tex2D  = std::static_pointer_cast< Texture2D >( shaderParams->GetTexture( j ) );
                if( GTexture2DCache.IsStored( tex2D ) && tex2D != GTexture2DCache.GetTexture( texDesc.get() ) )
                {
                    auto newTex2D = GTexture2DCache.GetTexture( texDesc.get() );
                    shaderParams->SetTexture( j, newTex2D );
                }
                else //Some other texture type which just requires contents to be swapped
                {
                    tex2D->SetData( texDesc->GetBits(), texDesc->GetFormat(), texDesc->GetWidth(), texDesc->GetHeight(), texDesc->GetNumLevels() );
                }

                m_texDataUpdateID[ txIdx ][ j ] = texDesc->GetUpdateID();
            }

            auto samplerState = texDesc->GetSamplerState();
            auto shaderSamplerParams = shaderParams->GetSamplerParameters( j );

            //update sampler values
            shaderSamplerParams->SetWrappingModeX( ( SamplerWrappingMode )samplerState->GetWrappingModeX() );
            shaderSamplerParams->SetWrappingModeY( ( SamplerWrappingMode )samplerState->GetWrappingModeY() );
            shaderSamplerParams->SetWrappingModeZ( ( SamplerWrappingMode )samplerState->GetWrappingModeZ() );
            shaderSamplerParams->SetFilteringMode( ( SamplerFilteringMode )samplerState->GetFilteringMode() );
            shaderSamplerParams->SetBorderColor( samplerState->GetBorderColor() );
        }


        for( unsigned int i = 0; i < animations.size(); ++i, ++j )
        {
            auto tex2D   = std::static_pointer_cast< Texture2D >( shaderParams->GetTexture( j ) );
            auto animDesc   = animations[ i ];

            auto currFrame = animDesc->CurrentFrame();
            auto numTextures = animDesc->NumTextures();

            assert( currFrame <= numTextures );

            if( m_texDataUpdateID[ txIdx ][ j ] < animDesc->GetUpdateID() )
            {
                if( currFrame < numTextures )
                {
                    tex2D->SetData( animDesc->GetBits( currFrame ), animDesc->GetFormat(), animDesc->GetWidth(), animDesc->GetHeight() );
                }
                else if ( currFrame == numTextures )
                {
                    tex2D->ForceUpdate();
                }
                m_texDataUpdateID[ txIdx ][ j ] = animDesc->GetUpdateID();
            }

            auto samplerState = animDesc->GetSamplerState();
            auto shaderSamplerParams = shaderParams->GetSamplerParameters( j );

            //update sampler values
            shaderSamplerParams->SetWrappingModeX( ( SamplerWrappingMode )samplerState->GetWrappingModeX() );
            shaderSamplerParams->SetWrappingModeY( ( SamplerWrappingMode )samplerState->GetWrappingModeY() );
            shaderSamplerParams->SetWrappingModeZ( ( SamplerWrappingMode )samplerState->GetWrappingModeZ() );
            shaderSamplerParams->SetFilteringMode( ( SamplerFilteringMode )samplerState->GetFilteringMode() );
            shaderSamplerParams->SetBorderColor( samplerState->GetBorderColor() );
        }
    }
}

// *****************************
//
inline void		NodeUpdater::UpdateAudio				()
{
    if( m_audio && m_audioChannel )
    {
        
        
        if( m_audioChannel->GetFormat() != m_audio->GetFormat() ||
            m_audioChannel->GetFrequency() != m_audio->GetFrequency() )
        {
            m_audio->Reinitialize( m_audioChannel->GetFrequency(), m_audioChannel->GetFormat() );
        }

		while( auto packet = m_audioChannel->PopPacket() )
		{
			m_audio->PushData( packet );
		}
    }
}

//// *******************************
////
//template< typename ValType, typename ShaderParamType >
//void	NodeUpdater::UpdateTypedShaderParam   ( IValueConstPtr source, GenericShaderParam * dest )
//{
//    static_cast< ShaderParamType * >( dest )->SetValue( QueryTypedValue< ValType >( source )->GetValue() );
//}

// *******************************
//
template< typename ValType >
void	NodeUpdater::UpdateTypedValue   ( IValueConstPtr source, IValuePtr dest )
{
    QueryTypedValue< ValType >( dest )->SetValue( QueryTypedValue< ValType >( source )->GetValue() );
}

} //bv
