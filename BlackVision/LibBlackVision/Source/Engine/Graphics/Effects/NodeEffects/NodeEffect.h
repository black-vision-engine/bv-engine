#pragma once

#include <vector>
#include <string>

#include "Engine/Interfaces/IValue.h"


namespace bv {

class NodeEffect
{
public:

    enum class Type : unsigned int
    {
        T_DEFAULT = 0,
        T_ALPHA_MASK,
        T_NODE_MASK,
        T_WIREFRAME,
        T_MIX_CHANNELS,

        T_TOTAL
    };

private:

    std::vector< IValuePtr >    m_modelValues;
    Type                        m_type;

public:

                    NodeEffect  ( Type type );
    virtual         ~NodeEffect ();

    SizeType        GetNumValues() const;

    IValuePtr       GetValue    ( unsigned int i );
    IValuePtr       GetValue    ( const std::string & name );

    Type            GetType     () const;

protected:

    std::vector< IValuePtr > &  AccessValues();

};

DEFINE_PTR_TYPE(NodeEffect)
DEFINE_CONST_PTR_TYPE(NodeEffect)

} // bv
