#pragma once

#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"


namespace bv {

class ShaderParamInt : public GenericShaderParam
{
private:

    const ValueInt *		m_valModel;
    
public:

							ShaderParamInt		( const std::string & name, const ValueInt * value );
                            ~ShaderParamInt		();

    void                    SetModelValue       ( const ValueInt * value );

    virtual const void *    GetValuePtr         () const override;
    virtual void *          AccessValuePtr      () override //FIXME: temporary, REMOVE
    {
        return const_cast< ValueInt * >(m_valModel);
    }

};

} //bv
