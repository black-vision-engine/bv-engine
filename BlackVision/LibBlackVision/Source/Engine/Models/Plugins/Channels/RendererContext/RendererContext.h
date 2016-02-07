#pragma once

#include "CoreDEF.h"
#include "Engine/Models/Plugins/Channels/RendererContext/AlphaContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/CullContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/DepthContext.h"
#include "Engine/Models/Plugins/Channels/RendererContext/FillContext.h"

#include "Serialization/ISerializable.h"

namespace bv { namespace model {

class RendererContext;
DEFINE_CONST_PTR_TYPE(RendererContext)
DEFINE_PTR_TYPE(RendererContext)

class RendererContext : public ISerializable
{
private:

    mutable bool    m_stateChanged;

public:

    AlphaContext *  alphaCtx;
    CullContext  *  cullCtx;
    DepthContext *  depthCtx;
    FillContext  *  fillCtx;

public:

            RendererContext     ();
public:
            ~RendererContext    ();

    bool                        StateChanged        () const;
    void                        SetStateChanged     ( bool stateChanged ) const;

    static RendererContextPtr   Create              ();
    static RendererContextPtr   CreateDefault       ();

    RendererContextPtr		    Clone				() const;

    void		                SetContext		    ( RendererContextConstPtr & ctx );
    
    virtual void                Serialize       ( ISerializer& ser ) const;
    void                        Deserialize     ( const IDeserializer& deser );
    static RendererContextPtr   Create          ( const IDeserializer& deser );
};

} //model
} //bv
