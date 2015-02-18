#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec2 : public GenericShaderParam
{
private:

    const ValueVec2 *   m_valModel;
    
public:

                            ShaderParamVec2     ( const std::string & name, const ValueVec2 * value );
                            ~ShaderParamVec2    ();

    void                    SetModelValue       ( const ValueVec2 * value );

    virtual const void *    GetValuePtr         () const override;
    virtual void *          AccessValuePtr      () override //FIXME: temporary, REMOVE
    {
        return const_cast<ValueVec2*>(m_valModel);
    }

};

} //bv
