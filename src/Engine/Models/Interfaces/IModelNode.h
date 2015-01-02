#pragma once

#include <iostream>
#include <memory>

#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Models/Plugins/Interfaces/IFinalizePlugin.h"

namespace bv {

class SceneNode;

namespace model {

class IFinalizePlugin;
class IPluginListFinalized;
class IOverrideState;

class IModelNode;
DEFINE_PTR_TYPE(IModelNode)
DEFINE_CONST_PTR_TYPE(IModelNode)

class IModelNode : public bv::IUpdatable
{
public:

    virtual const std::string &             GetName             () const                                                            = 0;

    virtual IPluginPtr                      GetPlugin           ( const std::string & name ) const                                  = 0;
    virtual IFinalizePluginConstPtr         GetFinalizePlugin   () const                                                            = 0;

    virtual IModelNodePtr                   GetNode             ( const std::string & path, const std::string & separator = "/" )   = 0;
    virtual IModelNodePtr                   GetChild            ( const std::string & name )                                        = 0;
    virtual IModelNodePtr                   GetLayer            ( const std::string & name )                                        = 0;

    virtual const IPluginListFinalized *    GetPluginList       () const                                                            = 0;

    virtual void                            EnableOverrideStateAM   ()                                                              = 0;
    virtual void                            EnableOverrideStateNM   ()                                                              = 0;
    virtual void                            DisableOverrideStateAM  ()                                                              = 0;
    virtual void                            DisableOverrideStateNM  ()                                                              = 0;

    virtual bool                            OverrideStateChangedAM  () const                                                        = 0;
    virtual bool                            OverrideStateChangedNM  () const                                                        = 0;
    virtual void                            SetOverrideStateChgAM   ( bool changed )                                                = 0;
    virtual void                            SetOverrideStateChgNM   ( bool changed )                                                = 0;

    virtual bool                            IsStateOverridenNM  () const                                                            = 0;
    virtual bool                            IsStateOverridenAM  () const                                                            = 0;

    virtual IOverrideState *                GetOverrideState    ()                                                                  = 0;

    virtual SceneNode *                     BuildScene          ()                                                                  = 0;
    virtual void                            Print               ( std::ostream & out, int tabs = 0 ) const                          = 0;

    virtual bool                            IsVisible           () const                                                            = 0;

    virtual                                 ~IModelNode         () {};

};

} //model
} //bv
