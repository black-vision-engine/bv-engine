#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamVec3 : public GenericShaderParam
{
private:

    const ValueVec3 *   m_valModel;
    
public:

                            ShaderParamVec3     ( const std::string & name, const ValueVec3 * value );
                            ~ShaderParamVec3    ();

    void                    SetModelValue       ( const ValueVec3 * value );

    virtual const void *    GetValuePtr         () const override;
    virtual void *          AccessValuePtr      () override //FIXME: temporary, REMOVE
    {
        return const_cast<ValueVec3*>(m_valModel);
    }

};

} //bv
