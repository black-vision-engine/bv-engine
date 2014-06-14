#pragma once

#include <iostream>
#include <memory>

#include "Engine/Interfaces/IUpdatable.h"


namespace bv {

class SceneNode;

namespace model {

class IPlugin;
class IFinalizePlugin;
class IPluginListFinalized;
class IOverrideState;

class IModelNode : public bv::IUpdatable
{
public:

    virtual const std::string &             GetName             () const                                                            = 0;

    virtual IPlugin *                       GetPlugin           ( const std::string & name ) const                                  = 0;
    virtual const IFinalizePlugin *         GetFinalizePlugin   () const                                                            = 0;

    virtual IModelNode *                    GetNode             ( const std::string & path, const std::string & separator = "/" )   = 0;
    virtual IModelNode *                    GetChild            ( const std::string & name )                                        = 0;
    virtual IModelNode *                    GetLayer            ( const std::string & name )                                        = 0;

    virtual const IPluginListFinalized *    GetPluginList       () const                                                            = 0;

    virtual void                            EnableOverrideState ()                                                                  = 0;
    virtual void                            DisableOverrideState()                                                                  = 0;

    virtual bool                            OverrideStateChanged() const                                                            = 0;
    virtual void                            SetOverrideStateChg ( bool changed )                                                    = 0;

    virtual bool                            IsStateOverriden    () const                                                            = 0;
    virtual IOverrideState *                GetOverrideState    ()                                                                  = 0;

    virtual SceneNode *                     BuildScene          ()                                                                  = 0;
    virtual void                            Print               ( std::ostream & out, int tabs = 0 ) const                          = 0;

    virtual bool                            IsVisible           () const                                                            = 0;

    virtual                                 ~IModelNode         () {};

};

typedef std::shared_ptr< IModelNode >       IModelNodePtr;

} //model
} //bv
