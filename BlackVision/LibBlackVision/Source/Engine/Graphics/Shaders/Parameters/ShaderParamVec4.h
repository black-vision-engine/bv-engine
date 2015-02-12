#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec4 : public GenericShaderParam
{
private:

    const ValueVec4 *   m_valModel;
    
public:

                            ShaderParamVec4     ( const std::string & name, const ValueVec4 * value );
                            ~ShaderParamVec4    ();

    void                    SetModelValue       ( const ValueVec4 * value );

    virtual const void *    GetValuePtr         () const override;
    virtual void *          AccessValuePtr      () override //FIXME: temporary, REMOVE
    {
        return const_cast<ValueVec4*>(m_valModel);
    }

};

} //bv
