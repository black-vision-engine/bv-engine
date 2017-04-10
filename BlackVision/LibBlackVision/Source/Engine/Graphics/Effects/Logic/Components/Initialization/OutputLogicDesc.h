#pragma once

#include "Engine/Graphics/Effects/Logic/Components/Initialization/OutputDesc.h"


namespace bv { 

class OutputLogicDesc
{
private:

    std::vector< OutputDesc >   m_outputDescriptors;

public:

    unsigned int    GetNumDescriptors   () const;

    OutputDesc &    AccessDesc          ( unsigned int i );
    void            AppendDesc          ( const OutputDesc & desc );

};


} //bv
