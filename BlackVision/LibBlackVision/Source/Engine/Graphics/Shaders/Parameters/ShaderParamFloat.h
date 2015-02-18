#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamFloat : public GenericShaderParam
{
private:

    const ValueFloat *   m_valModel;
    
public:

                            ShaderParamFloat    ( const std::string & name, const ValueFloat * value );
                            ~ShaderParamFloat   ();

    void                    SetModelValue       ( const ValueFloat * value );

    virtual const void *    GetValuePtr         () const override;
    virtual void *          AccessValuePtr      () override //FIXME: temporary, REMOVE
    {
        return const_cast<ValueFloat*>(m_valModel);
    }

};

} //bv
