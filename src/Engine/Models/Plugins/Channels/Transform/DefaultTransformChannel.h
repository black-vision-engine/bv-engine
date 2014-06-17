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

    ValueMat4PtrVec             m_values;
    const ValueMat4PtrVec *     m_prevValues;

    bool                        m_isReadOnly;

private:

    explicit                            DefaultTransformChannel ( IPluginConstPtr prev, const ValueMat4PtrVec & values, bool isReadOnly );

public:

    static  DefaultTransformChannel *   Create                  ( IPluginConstPtr prev, const ValueMat4PtrVec & values, bool isReadOnly = false );

    virtual const ValueMat4PtrVec &     GetTransformValues      ()  const override;

    virtual bool                        IsReadOnly              ()  const override;  //IChannel

    virtual void                        PostUpdate              ()  override;         //Should also be IChannel

};

DEFINE_PTR_TYPE(DefaultTransformChannel)

} // model
} // bv