#include "PluginTransformSimple.h"

#include "Engine\Models\Parameter.h"

namespace bv
{

// *********************************
//
PluginTransformSimple::PluginTransformSimple(const TransformF& transInterpolator)
    : PluginTransform("transform.simple")
{
    m_transParam = new ParamTransform("localTransform", transInterpolator);
    m_value = new ValueMat4("MVP");
    RegisterValue(m_value);
}
// *********************************
//
void PluginTransformSimple::Update(float t)
{
    if(m_transParam->IsEvaluationNeeded(t))
    {
        m_value->SetValue( m_transParam->Evaluate(t) );
    }
}

} // bv