#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/ITransformChannel.h"
#include "Engine/Models/Plugins/Parameters/TypedValues.h"


namespace bv { namespace model
{

class IValueSet;
class IPlugin;

class DefaultTransformChannel : public IDefaultTransformChannel
{
private:

    ValueMat4PtrVec             m_values;
    const ValueMat4PtrVec *     m_prevValues;

    bool                        m_isReadOnly;

private:

    explicit                            DefaultTransformChannel ( const IPlugin * prev, const ValueMat4PtrVec & values, bool isReadOnly );

public:

    static  DefaultTransformChannel *   Create                  ( const IPlugin * prev, IValueSet * values, bool isReadOnly = false );

    virtual const ValueMat4PtrVec &     GetTransformValues      ()  const override;

    virtual bool                        IsReadOnly              ()  const override;  //IChannel

    virtual void                        PostUpdate              ()  override;         //Should also be IChannel

};

typedef std::shared_ptr< DefaultTransformChannel > DefaultTransformChannelPtr;

} // model
} // bv