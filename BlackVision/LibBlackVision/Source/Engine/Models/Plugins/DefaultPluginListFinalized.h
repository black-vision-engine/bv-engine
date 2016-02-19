#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginListFinalized.h"
#include "Engine/Models/Plugins/DefaultFinalizePlugin.h"


namespace bv { namespace model {

class DefaultPluginListFinalized : public IPluginListFinalized
{
private:

    std::vector< IPluginPtr >   m_plugins;
    DefaultFinalizePluginPtr    m_finalizePlugin;

public:

                DefaultPluginListFinalized      ();
                DefaultPluginListFinalized      ( const std::vector< IPluginPtr > & plugins );
                                                                
    virtual     ~DefaultPluginListFinalized     ();

    virtual IPluginPtr              GetPlugin           ( const std::string & name ) const override;
    virtual IPluginPtr              GetPlugin           ( UInt32 idx ) const override;
    virtual IPluginPtr              GetLastPlugin       () const override;

    virtual unsigned int            NumPlugins          () const override;

    virtual IFinalizePluginPtr      GetFinalizePlugin   () const override;

    virtual void                    Update              ( TimeType t ) override;

    IPluginPtr                      GetPlugin           ( const std::string & name );
    IPluginPtr                      GetPlugin           ( UInt32 idx );

    void                            AttachPlugin        ( IPluginPtr plugin );
    bool                            AttachPlugin        ( IPluginPtr plugin, UInt32 idx );
    IPluginPtr                      DetachPlugin        ( UInt32 idx );
    IPluginPtr                      DetachPlugin        ( const std::string & name );

};

DEFINE_PTR_TYPE(DefaultPluginListFinalized)

} // model
} // bv
