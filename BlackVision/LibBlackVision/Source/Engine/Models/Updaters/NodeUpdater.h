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

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

//FIXME: this part suxx as hell
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Models/Interfaces/IModelNode.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"

#include "Engine/Graphics/Resources/VertexArray.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"

namespace bv {

class SceneNode;
class RenderableEntity;
class RendererStateInstance;
class ShaderParameters;
class IShaderDataSource;
class ITextureDescriptor;
class IAnimationDescriptor;
class Texture2D;


namespace model
{
    class IModelNode;
    class ITransformChannel;
    class IVertexAttributesChannel;

    class HelperVertexAttributesChannel;
}

typedef std::pair< ITexturesDataConstPtr, ShaderParameters * > TexData2ShaderParams;
typedef std::pair< IValueConstPtr, GenericShaderParam * > Value2ShaderParam;

//typedef std::pair< const ITextureDescriptor *, Texture2D * > Tex2Tex2DPair;
//typedef std::pair< const IAnimationDescriptor *, Texture2D * > Anim2Tex2DPair;

class NodeUpdater;
DEFINE_CONST_PTR_TYPE(NodeUpdater)
DEFINE_PTR_TYPE(NodeUpdater)

class NodeUpdater : public IUpdater
{
private:

    SceneNode *                                 m_sceneNode;
    model::IModelNodeConstPtr                   m_modelNode;

    RenderableEntity *                          m_renderable;
    RenderableEntity *                          m_boundingBox;
    model::ITransformChannelConstPtr            m_transformChannel;
    model::IVertexAttributesChannelConstPtr     m_vertexAttributesChannel;
    
    bool                                        m_timeInvariantVertexData;
    bool                                        m_hasEffect;

    std::vector< RendererStateInstance * >      m_redererStateInstanceVec;
    model::RendererContextConstPtr              m_rendererContext;

    //std::vector< Tex2Tex2DPair >                m_texMappingVec;
    //std::vector< Anim2Tex2DPair >               m_animMappingVec;

    std::vector< TexData2ShaderParams >         m_texDataMappingVec;
    std::vector< std::vector< UInt64 > >		m_texDataUpdateID;

    std::vector< Value2ShaderParam >			m_paramsMappingVec;

    UInt64										m_attributesUpdateID;
    UInt64										m_topologyUpdateID;


                            NodeUpdater         ( SceneNode * sceneNode, model::IModelNodeConstPtr modelNode ); 

public:

    virtual                 ~NodeUpdater        ();

    static NodeUpdaterPtr   Create              ( SceneNode * sceneNode, model::IModelNodeConstPtr modelNode );
    virtual void            DoUpdate            () override;

private:

    void            RegisterTexturesData( IShaderDataSourceConstPtr psTxData, IShaderDataSourceConstPtr vsTxData, IShaderDataSourceConstPtr gsTxData, RenderablePass * pass );
    bool            MustBeRegistered    ( IShaderDataSourceConstPtr shaderDataSrc, ShaderParameters * shaderParams );

    template< typename ShaderType >
    void RegisterTypedTexturesData( IShaderDataSourceConstPtr txData, ShaderType * shader )
    {
        if ( shader != nullptr && MustBeRegistered( txData, shader->GetParameters() ) )
        {
            RegisterTex2Params( txData->GetTexturesData(), shader->GetParameters() );
        }
    }

    void            RegisterTex2Params  ( ITexturesDataConstPtr texturesData, ShaderParameters * shaderParams );

    void            UpdateNodeEffect    ();

    inline  void    UpdateTransform     ();
    inline  void    UpdateGeometry      ();
    inline  void    UpdateRendererState ();

    inline  void    UpdatePositions     ();
    inline  void    UpdateTopology      ();

    inline	void	UpdateShaderParams  ();
    inline  void    UpdateTexturesData	();

private:

    void            RegisterShaderParams	( IValueSetConstPtr values, Shader * shader );
    
    
    void			UpdateShaderParam		( IValueConstPtr source, GenericShaderParam * dest );

    template< typename ValType, typename ShaderParamType >
    void			UpdateTypedShaderParam   ( IValueConstPtr source, GenericShaderParam * dest );

private:
    
    void			UpdateValue			( IValueConstPtr source, IValuePtr dest );

    template< typename ValType >
    void			UpdateTypedValue    ( IValueConstPtr source, IValuePtr dest );

};

} //bv

#include "NodeUpdater.inl"
