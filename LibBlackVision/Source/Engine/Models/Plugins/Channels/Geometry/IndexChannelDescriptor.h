#pragma once

#include <string>

#include "Engine/Models/Plugins/Interfaces/IIndexChannelDescriptor.h"


namespace bv { namespace model
{

class IndexChannelDescriptor : public IIndexChannelDescriptor
{
private:

    static const int    m_sIdxSizes[ (int) IndexType::IT_TOTAL ];

    IndexType           m_indexType;

public:

                            IndexChannelDescriptor  ( IndexType type );
    virtual                 ~IndexChannelDescriptor ();

    virtual IndexType       GetType                 ()  const;
    virtual int             GetIndexSize            ()  const;

    std::string             SuggestedDefaultName    () const;

    static  int             IndexSize               ( IndexType type );

    static  std::string     DefaultName             ( IndexType type );

};

} //model
} //bv
