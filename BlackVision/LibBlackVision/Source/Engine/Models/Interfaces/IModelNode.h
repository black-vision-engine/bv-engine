#pragma once

#include <iostream>
#include <memory>

#include "Engine/Models/IDGenerator.h"

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Interfaces/IModelNodeEffect.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"

#include "Engine/Models/Interfaces/INodeLogic.h"

#include <unordered_set>

namespace bv {

class SceneNode;
class Renderer;

namespace model {

class IFinalizePlugin;
class IPluginListFinalized;

class IModelNode;
DEFINE_PTR_TYPE(IModelNode)
DEFINE_WEAK_PTR_TYPE( IModelNode )
DEFINE_CONST_PTR_TYPE(IModelNode)

class IModelNode : public bv::IUpdatable
{
public:

    virtual const std::string &							GetName             () const                                                            = 0;
    virtual UniqueID                                    GetUID              () const                                                            = 0;

    virtual IPluginPtr									GetPlugin           ( const std::string & name ) const                                  = 0;
    virtual IFinalizePluginConstPtr						GetFinalizePlugin   () const                                                            = 0;

    virtual IModelNodePtr								GetNode             ( const std::string & path, const std::string & separator = "/" )   = 0;
    virtual IModelNodePtr								GetNode             ( UniqueID id, bool recursive = true ) = 0;
    virtual IModelNodePtr								GetChild            ( const std::string & name )                                        = 0;

    virtual const IPluginListFinalized *				GetPluginList       () const                                                            = 0;
    virtual std::vector< IParameterPtr >				GetParameters       () const                                                            = 0;
    virtual std::vector< ITimeEvaluatorPtr >		    GetTimelines        ( bool recursive ) const                                            = 0;

    virtual unsigned int								GetNumChildren      () const                                                            = 0;

    // FIXME: TRANSITION - BEGIN
    virtual IModelNodeEffectPtr						    GetNodeEffect       () const                                                            = 0;
    // FIXME: TRANSITION - END

    virtual INodeLogicPtr							    GetLogic            () const                                                            = 0;

    virtual bool                                        IsVisible           () const                                                            = 0;

    //pablito
    virtual void                                        SetVisible           ( bool visible )                                                   = 0;

    virtual                                             ~IModelNode         () {};

};

} //model
} //bv
