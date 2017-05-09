#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"

namespace bv { namespace model {

class NodeVisibility : public ISerializable
{
private:

    DefaultParamValModelPtr m_paramValModel;
    ParamBoolPtr            m_visibleParam;
    bool                    m_isVisible;
    bool                    m_hasMoreThanOneKey;

    void        InitializeVisibleParam  ();

public:
                NodeVisibility  ();
    virtual     ~NodeVisibility ();

    void        SetVisible      ( bool visible );
    bool        IsVisible       () const;
    void        AddKey          ( bool val, TimeType t );
    void        RemoveKey       ( TimeType t );



    virtual void                Serialize       ( ISerializer & ser ) const override;
    static ISerializablePtr     Create          ( const IDeserializer & deser );
};

} // model
} // bv