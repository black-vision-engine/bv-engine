#pragma once

#include <memory>

#include "Engine/Interfaces/IValueSet.h"
#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"
#include "Engine/Types/Values/TypedValues.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"

namespace bv { namespace model
{

class IPlugin;

class DefaultTransformChannel : public ITransformChannel
{
private:
    ValueMat4Ptr                m_value;
    const ValueMat4Ptr *        m_prevValue;

    bool                        m_isReadOnly;

private:

    explicit                            DefaultTransformChannel ( IPluginPtr prev, const ValueMat4Ptr & value, bool isReadOnly );

public:

    static  DefaultTransformChannel *   Create                  ();
    static  DefaultTransformChannel *   Create                  ( IPluginPtr prev, const ValueMat4Ptr & value, bool isReadOnly = false );

    virtual const ValueMat4Ptr &        GetTransformValue       ()  const override;

    virtual bool                        IsReadOnly              ()  const override;  //IChannel

    virtual void                        PostUpdate              ()  override;         //Should also be IChannel

};

DEFINE_PTR_TYPE(DefaultTransformChannel)

} // model
} // bv