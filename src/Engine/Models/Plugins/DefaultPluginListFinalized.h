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

    virtual const IPlugin *         GetPlugin           ( const std::string & name ) const override;
    virtual const IPlugin *         GetPlugin           ( unsigned int idx ) const override;
    virtual const IPlugin *         GetLastPlugin       () const override;

    virtual unsigned int            NumPlugins          () const override;

    virtual const IFinalizePlugin * GetFinalizePlugin   () const override;

    virtual void                    Update              ( TimeType t ) override;

    IPlugin *                       GetPlugin           ( const std::string & name );
    IPlugin *                       GetPlugin           ( unsigned int idx );

    void                            AttachPlugin        ( IPlugin * plugin );

};

typedef std::shared_ptr< DefaultPluginListFinalized > DefaultPluginListFinalizedPtr;

} // model
} // bv

