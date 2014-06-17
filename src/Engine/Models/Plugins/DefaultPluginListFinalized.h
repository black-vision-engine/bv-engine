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

    virtual IPluginConstPtr         GetPlugin           ( const std::string & name ) const override;
    virtual IPluginConstPtr         GetPlugin           ( unsigned int idx ) const override;
    virtual IPluginConstPtr         GetLastPlugin       () const override;

    virtual unsigned int            NumPlugins          () const override;

    virtual const IFinalizePlugin * GetFinalizePlugin   () const override;

    virtual void                    Update              ( TimeType t ) override;

    IPluginPtr                      GetPlugin           ( const std::string & name );
    IPluginPtr                      GetPlugin           ( unsigned int idx );

    void                            AttachPlugin        ( IPluginPtr plugin );

};

DEFINE_PTR_TYPE(DefaultPluginListFinalized)

} // model
} // bv

