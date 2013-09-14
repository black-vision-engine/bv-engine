#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "Engine/Models/Parameter.h"

#include "System/Print.h"
#include "Engine/Models/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv {

class ValueMat4;

//FIXME: can you feel it, that SHIT???
extern void PrintMatrixDebugOnlyLocal( const glm::mat4 mat, std::ostream & out, int tabs );

template< typename TransformPluginParametersDescriptor >
class PluginTransform : public BasePlugin<ITransformPlugin, TransformPluginParametersDescriptor>
{
protected:

    ValueMat4 *                 m_value;

public:

    const glm::mat4 &           TransformMatrix     ()                                    const;
    virtual void                Print               ( std::ostream& out, int tabs = 0 )   const;

    explicit PluginTransform                        ();
    virtual ~PluginTransform                        ()
    {
        delete m_value;
    };

};

// *********************************
//
template< typename TransformPluginParametersDescriptor >
PluginTransform< TransformPluginParametersDescriptor >::PluginTransform()
    : m_value( nullptr )
{
}

// *********************************
//
template< typename TransformPluginParametersDescriptor >
const glm::mat4 & PluginTransform< TransformPluginParametersDescriptor >::TransformMatrix     () const
{
    return m_value->GetValue();
}

// *********************************
//
template< typename TransformPluginParametersDescriptor >
void PluginTransform< TransformPluginParametersDescriptor >::Print( std::ostream & out, int tabs ) const
{
    out << GetName() << debug::EndLine( tabs );
    out << "PARAMS:" << debug::EndLine( tabs );
    out << "Transform:" << debug::EndLine( tabs );
    PrintMatrixDebugOnlyLocal( m_value->GetValue(), out, tabs );
}

}
