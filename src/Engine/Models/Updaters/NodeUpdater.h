#pragma once

#include <vector>

#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"
#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Models/Interfaces/ITexturesData.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"

//FIXME: this part suxx as hell
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"

#include "Engine/Graphics/Resources/VertexArray.h"



namespace bv {

class SceneNode;
class RenderableEntity;
class RendererStateInstance;
class ShaderParameters;
class IShaderDataSource;
class ITextureDescriptor;
class IAnimationDescriptor;
class Texture2DImpl;
class Texture2DSequenceImpl;

namespace model
{
    class IModelNode;
    class ITransformChannel;
    class IVertexAttributesChannel;
}

typedef std::pair< ITexturesDataConstPtr, ShaderParameters * > TexData2ShaderParams;
typedef std::pair< const ITextureDescriptor *, Texture2DImpl * > Tex2Tex2DPair;
typedef std::pair< const IAnimationDescriptor *, Texture2DSequenceImpl * > Anim2Tex2DPair;

class NodeUpdater : public IUpdater
{
private:

    SceneNode *                                 m_sceneNode;
    model::IModelNodeConstPtr                   m_modelNode;

    RenderableEntity *                          m_renderable;
    const model::ITransformChannel *            m_transformChannel;
    const model::IVertexAttributesChannel *     m_vertexAttributesChannel;
    
    bool                                        m_timeInvariantVertexData;
    bool                                        m_hasEffect;

    std::vector< RendererStateInstance * >      m_redererStateInstanceVec;
    const model::RendererContext *              m_rendererContext;

    std::vector< Tex2Tex2DPair >                m_texMappingVec;
    std::vector< Anim2Tex2DPair >               m_animMappingVec;

    std::vector< TexData2ShaderParams >         m_texDataMappingVec;

public:

                    NodeUpdater         ( RenderableEntity * renderable, SceneNode * sceneNode, model::IModelNodeConstPtr modelNode ); 
                    ~NodeUpdater        ();

    virtual void    DoUpdate            () override;

private:

    void            RegisterTexturesData( const IShaderDataSource * psTxData, const IShaderDataSource * vsTxData, const IShaderDataSource * gsTxData, RenderablePass * pass );
    bool            MustBeRegistered    ( const IShaderDataSource * shaderDataSrc, ShaderParameters * shaderParams );

    template< typename ShaderType >
    void RegisterTypedTexturesData( const IShaderDataSource * txData, ShaderType * shader )
    {
        if ( shader != nullptr && MustBeRegistered( txData, shader->GetParameters() ) )
        {
            RegisterTex2Params( txData->GetTexturesData(), shader->GetParameters() );
        }
    }

    void            RegisterTex2Params  ( ITexturesDataConstPtr texturesData, ShaderParameters * shaderParams );

    inline  void    UpdateTransform     ();
    inline  void    UpdateGeometry      ();
    inline  void    UpdateRendererState ();
    inline  void    UpdateOverridenState();

    inline  void    UpdatePositions     ();
    inline  void    UpdateTopology      ();

    inline  void    UpdateTexturesData  ();

};

} //bv

#include "NodeUpdater.inl"
