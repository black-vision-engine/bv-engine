#pragma once

#include <vector>

#include "Engine/Interfaces/IUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

//FIXME: this part suxx as hell
#include "Engine/Models/Plugins/Interfaces/IAttributeChannel.h"
#include "Engine/Models/Plugins/Interfaces/IAttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"

#include "Engine/Graphics/Resources/VertexArray.h"


namespace bv {

class SceneNode;
class RenderableEntity;
class RendererStateInstance;
class ShaderParameters;
class IShaderDataSource;

namespace model
{
    class IModelNode;
    class ITransformChannel;
    class IVertexAttributesChannel;
    class ITexturesData;
}

typedef std::pair< const model::ITexturesData *, ShaderParameters * > Tex2ParamsPair;

class NodeUpdater : public IUpdater
{
private:

    SceneNode *                                 m_sceneNode;
    const model::IModelNode *                   m_modelNode;

    RenderableEntity *                          m_renderable;
    const model::ITransformChannel *            m_transformChannel;
    const model::IVertexAttributesChannel *     m_vertexAttributesChannel;
    
    bool                                        m_timeInvariantVertexData;
    bool                                        m_hasEffect;

    std::vector< RendererStateInstance * >      m_redererStateInstanceVec;
    const model::RendererContext *              m_rendererContext;

    std::vector< Tex2ParamsPair >               m_tex2ParamsVec;

public:

                    NodeUpdater         ( RenderableEntity * renderable, SceneNode * sceneNode, const model::IModelNode * modelNode ); 
                    ~NodeUpdater        ();

    virtual void    DoUpdate            () override;

private:

    void            RegisterTexturesData( const IShaderDataSource * psTxData, const IShaderDataSource * vsTxData, const IShaderDataSource * gsTxData, RenderablePass * pass );
    bool            CanBeRegistered     ( const IShaderDataSource * shaderDataSrc, ShaderParameters * shaderParams );
    Tex2ParamsPair  RegisterTex2Params  ( const model::ITexturesData * texturesData, ShaderParameters * shaderParams );

    inline  void    UpdateTransform     ();
    inline  void    UpdateGeometry      ();
    inline  void    UpdateRendererState ();

    inline  void    UpdatePositions     ();
    inline  void    UpdateTopology      ();

    inline  void    UpdateTexturesData  ();

};

} //bv

#include "NodeUpdater.inl"
