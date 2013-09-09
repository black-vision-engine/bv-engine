#pragma once
#include "PluginTransform.h"

#include "Mathematics\Transform\MatTransform.h"

namespace bv
{
class ParamTransform;

class PluginTransformSimple : public PluginTransform
{
    ParamTransform*          m_transParam;

public:
    virtual void    Update          (float t) override;

    explicit PluginTransformSimple(const TransformF&);
    virtual ~PluginTransformSimple(){}
};

}