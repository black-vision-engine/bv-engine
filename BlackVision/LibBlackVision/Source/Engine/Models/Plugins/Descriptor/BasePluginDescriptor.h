#pragma once

#include "Engine/Models/Plugins/Interfaces/IPluginDescriptor.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleStateUpdater.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"


#include "Engine/Models/Gizmos/GizmoFunMapper.h"


namespace bv { namespace model {

class BasePluginDescriptor : public IPluginDescriptor
{
private:

    std::string     m_uid;
    std::string     m_defaultName;
    std::string     m_abbrv;

    GizmoFunMapper  m_gizmosMapping;

protected:

    BasePluginDescriptor    ( const std::string & uid, const std::string & defaultName, const std::string & abbrv );
    BasePluginDescriptor    ( const std::string & uid, const std::string & defaultName ); // FIXME remove this for God's sake

public:

    virtual const std::string &             GetPluginTypeUID    () const override;
    virtual const std::string &             DefaultPluginName   () const override;
    virtual const std::string &             GetPluginTypeAbbrv  () const override;

    virtual std::string                     QueryGizmoLogicName ( const std::string & functionalityName ) const override;
    virtual std::vector< std::string >      ListGizmoFunctions  () const override;

    virtual IPluginParamValModelPtr         CreateModel         ( ITimeEvaluatorPtr timeEvaluator ) const override;

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const = 0;

    template< typename PluginType >
    std::shared_ptr< PluginType >           CreatePluginTyped    ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator  ) const
    {
        return std::make_shared< PluginType >( name, GetPluginTypeUID(), prev, DefaultPluginParamValModelPtr( CreateDefaultModel( timeEvaluator ) ) );
    }

    virtual ~BasePluginDescriptor () {}

protected:

    // Internal use for plugin creator.


};

} //model
} //bv

// wget --recursive --no-clobber --page-requisites --html-extension --convert-links --restrict-file-names=windows --domains blender-mirror.kino3d.org --no-parent http://blender-mirror.kino3d.org/peach/