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
        T_ALPHA_MASK = 0,
        T_NODE_MASK,

        T_TOTAL
    };

private:

    std::vector< IValuePtr >    m_modelValues;

public:

                    NodeEffect  ();
                    ~NodeEffect ();

    SizeType        GetNumValues() const;

    IValuePtr       GetValue    ( unsigned int i );
    IValuePtr       GetValue    ( const std::string & name );

protected:

    std::vector< IValuePtr > &  AccessValues();

};


DEFINE_PTR_TYPE(NodeEffect)
DEFINE_CONST_PTR_TYPE(NodeEffect)

} // bv
