#pragma once

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

class NodeVisibility;
DEFINE_PTR_TYPE( NodeVisibility )

class NodeVisibility : public ISerializable, public IUpdatable
{
private:

    DefaultParamValModelPtr         m_paramValModel;
    model::ValueParamState< bool >  m_isVisibleValParamState;
    bool                            m_isVisible;

    void        InitializeVisibleParam  ();

public:
                NodeVisibility  ();
    virtual     ~NodeVisibility ();

    void        SetVisible      ( bool visible );
    bool        IsVisible       () const;

    IParameterPtr GetVisibleParameter   ();

    virtual void                Serialize       ( ISerializer & ser ) const override;
    virtual void                Update          ( TimeType t ) override;
    static ISerializablePtr     Create          ( const IDeserializer & deser );
    static NodeVisibilityPtr    CreateTyped     ( const IDeserializer & deser );
};

} // model
} // bv