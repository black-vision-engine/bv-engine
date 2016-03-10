#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectInstance.h"

#include "Engine/Graphics/Resources/Textures/Texture.h"

namespace bv {

class RenderLogicContext;
class SceneNode;
class Texture;
class ITextureDescriptor;
DEFINE_CONST_PTR_TYPE( ITextureDescriptor )

class NodeEffectLogic
{
private:

    std::vector< IValuePtr >        m_values;

    std::vector< TextureConstPtr >  m_textures;

    PreFullscreenEffectLogic *      m_preFSELogic;
    
    FullscreenEffectInstance *      m_FSE;
    
    PostFullscreenEffectLogic *     m_postFSELogic;

public:

                    NodeEffectLogic             ();
                    ~NodeEffectLogic            ();

    void            Update                      ();
    void            Render                      ( SceneNode * node, RenderLogicContext * ctx );

private:

    void            RenderPre                   ( SceneNode * node, RenderLogicContext * ctx );
    void            RenderFSE                   ( RenderTarget * output, RenderLogicContext * ctx );
    void            RenderPost                  ( SceneNode * node, RenderLogicContext * ctx );

    bool            IsFSERequired               () const;

    void            AllocateRenderTargets       ( RenderLogicContext * ctx );
    void            FreeRenderTargets           ( RenderLogicContext * ctx );

public:

    void            SetComponent                ( PreFullscreenEffectLogic * logic );
    void            SetComponent                ( PostFullscreenEffectLogic * logic );
    void            SetComponent                ( FullscreenEffectInstance * fse );

    void            AddTexture                  ( const ITextureDescriptorConstPtr & txDesc );

    unsigned int    GetNumValues                () const;
    IValuePtr       GetValueAt                  ( unsigned int i ) const;
    IValuePtr       GetValue                    ( const std::string & name ) const;

private:

    void            RecreateValues              ( std::vector< IValuePtr > & values );

};

} //bv
