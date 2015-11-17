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

// *********************************
//FIXME: this should be implemented via VBOAcessor in one place only (VBO utils or some more generic utils) - right now it is copied here and in BasicNode (as AddVertexDataToVBO)
inline void    WriteVertexDataToVBO( char * data, model::IConnectedComponentPtr cc )
{
    unsigned int numVertices = cc->GetNumVertices();
    unsigned int offset = 0;

    for( unsigned int i = 0; i < numVertices; ++i )
    {
        for( auto vach : cc->GetAttributeChannels() )
        {
            assert( vach->GetNumEntries() == numVertices );

            auto eltSize = vach->GetDescriptor()->GetEntrySize(); //FIXME: most probably not required here (can be safely read from other location)
            const char * eltData = vach->GetData();

            memcpy( &data[ offset ], &eltData[ i * eltSize ], eltSize );

            offset += eltSize;
        }
    }
}

} //anonymous


// *****************************
//
inline  void    NodeUpdater::UpdateTransform     ()
{
    //FIXME: rili rili
    m_renderable->ResetLocalTransform();

    for( auto t : m_transformChannel->GetTransformValues() )
    {
        const glm::mat4 & mat = t->GetValue();

        m_renderable->SetLocalTransform( m_renderable->LocalTransform() * Transform( mat, glm::inverse( mat ) ) );
    }
}

// *****************************
//
inline  void    NodeUpdater::UpdateGeometry      ()
{
    if ( m_vertexAttributesChannel->NeedsAttributesUpdate() )
    {
        assert( !m_vertexAttributesChannel->NeedsTopologyUpdate() );
        UpdatePositions();
    }
    else if ( m_vertexAttributesChannel->NeedsTopologyUpdate() )
    {
        UpdateTopology();
    }
    else
    {
        RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_renderable->GetRenderableArrayData() );
        VertexArraySingleVertexBuffer * vao = rad->VAO();

        vao->SetNeedsUpdateMemUpload( false );
        vao->SetNeedsUpdateRecreation( false );
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

// *****************************
//
inline  void    NodeUpdater::UpdatePositions     ()
{
    //FIXME: implement for other types of geometry as well
    assert( m_renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessary
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * rad = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_renderable->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = rad->VAO                  (); 
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
    // const VertexDescriptor * vd         = vao->GetVertexDescriptor  ();

    auto vaChannel  = m_vertexAttributesChannel;
    auto components = vaChannel->GetComponents();
    auto geomDesc   = vaChannel->GetDescriptor();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P  TYM RAZEM KURWA PODWOJNA, BO TU NAPRAWDE ZACZYNA SIE ROBIC BURDEL
    unsigned int currentOffset = 0;

    unsigned int k = 0;

    for( auto cc : components )
    {
        //This is update only, so the number of vertices must match
        assert( cc->GetNumVertices() == vao->GetNumVertices( k ) );
 
        WriteVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();

        ++k;
    }

    vao->SetNeedsUpdateMemUpload( true );
}

// *****************************
//
inline  void    NodeUpdater::UpdateTopology      ()
{
    //FIXME: implement for other types of geometry as well
    assert( m_renderable->GetType() == RenderableEntity::RenderableType::RT_TRIANGLE_STRIP );

    //FIXME: if this is the last update then STATIC semantic should be used but right now it's irrelevant
    DataBuffer::Semantic vbSemantic = DataBuffer::Semantic::S_DYNAMIC;

    auto vaChannel  = m_vertexAttributesChannel;
    auto components = vaChannel->GetComponents();
    auto geomDesc   = vaChannel->GetDescriptor();

    if( components.empty() )
    {
        assert( false ); //FIXME: at this point empty geometry is not allowed
        return;
    }

    unsigned int totalNumVertivces = vaChannel->TotalNumVertices();
    
    //FIXME: works because we allow only triangle strips here
    //FIXME: this code used to update vertex bufer and vao from model should be written in some utility function/class and used where necessary
    //FIXME: putting it here is not a good idea (especially when other primitive types are added)
    RenderableArrayDataArraysSingleVertexBuffer * radasvb = static_cast< RenderableArrayDataArraysSingleVertexBuffer * >( m_renderable->GetRenderableArrayData() );

    VertexArraySingleVertexBuffer * vao = radasvb->VAO              ();
    VertexBuffer * vb                   = vao->GetVertexBuffer      ();
    // const VertexDescriptor * vd         = vao->GetVertexDescriptor  ();

    vb->Reinitialize( totalNumVertivces, geomDesc->SingleVertexEntrySize(), vbSemantic );
    vao->ResetState();

    char * vbData = vb->Data(); //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P
    unsigned int currentOffset = 0;

    for( auto cc : components )
    {
        assert( !cc->GetAttributeChannels().empty() );

        vao->AddCCEntry( cc->GetNumVertices() );

        WriteVertexDataToVBO( &vbData[ currentOffset ], cc );

        currentOffset += cc->GetNumVertices() * geomDesc->SingleVertexEntrySize();
    }

    vao->SetNeedsUpdateRecreation( true );
}

// *****************************
//
inline void    NodeUpdater::UpdateTexturesData  ()
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

			if ( m_texDataUpdateID[ txIdx ] < texDesc->GetUpdateID() )
            {
                auto tex2D  = std::static_pointer_cast< Texture2D >( shaderParams->GetTexture( j ) );

                 //Stored in cache which means that proper 2D texture has to be created for current texDesc (possibly alread stored in the cache)
                if( GTexture2DCache.IsStored( tex2D ) && tex2D != GTexture2DCache.GetTexture( texDesc ) )
                {
                    auto newTex2D = GTexture2DCache.GetTexture( texDesc );

                    shaderParams->SetTexture( j, newTex2D );
                }
                else //Some other texture type which just requires contents to be swapped
                {
					tex2D->SetData( texDesc->GetBits() );
                }

                m_texDataUpdateID[ txIdx ] = texDesc->GetUpdateID();
            }
        }


        for( unsigned int i = 0; i < animations.size(); ++i, ++j )
        {
            auto tex2D   = std::static_pointer_cast< Texture2D >( shaderParams->GetTexture( j ) );
            auto animDesc   = animations[ i ];

			auto currFrame = animDesc->CurrentFrame();
			auto numTextures = animDesc->NumTextures();

			assert( currFrame <= numTextures );

            if ( currFrame != animDesc->PreviousFrame() )
            {
				if( currFrame < numTextures )
				{
					tex2D->SetData( animDesc->GetBits( currFrame ) );
				}
				else if ( currFrame == numTextures )
				{
					tex2D->ForceUpdate();
				}
            }
        }

    }
}

} //bv
