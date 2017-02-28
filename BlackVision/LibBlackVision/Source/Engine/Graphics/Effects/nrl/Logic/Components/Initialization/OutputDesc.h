#pragma once

#include <hash_map>
#include <vector>
#include <string>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputStaticData.h"


namespace bv { namespace nrl {

enum class OutputChannelMapping : unsigned int
{
    OCM_R = 0,
    OCM_G,
    OCM_B,
    OCM_A,
    OCM_RGBA,

    OCM_TOTAL
};

class OutputDesc
{
public:

    typedef     std::hash_map< std::string, std::string >   TOutputPropertyMap;
    typedef     std::vector< TOutputPropertyMap >           TOutputPropertiesVector;

private:

    OutputStaticData        m_staticData;

    TOutputPropertiesVector m_outputProperties;

    OutputChannelMapping    m_outputChannelMapping;

    CustomOutputType        m_outputType;

    bool                    m_isEnabled;

public:

                                    OutputDesc                  ();
                                        
    TOutputPropertiesVector &       AccessOutputProperties      ();
    const TOutputPropertiesVector & GetOutputProperties         () const;

    unsigned int                    GetWidth                    () const;
    unsigned int                    GetHeight                   () const;

    RenderChannelType               GetSelectedRenderedChannel  () const;
    
    OutputChannelMapping            GetOutputChannelMapping     () const;

    CustomOutputType                GetRepresentedOutputType    () const;

    bool                            IsEnabled                   () const;

    void                            SetWidth                    ( unsigned int width );
    void                            SetHeight                   ( unsigned int height );
    
    void                            SetSelectedRenderedChannel  ( RenderChannelType rct );
    
    void                            SetOutputChannelMapping     ( OutputChannelMapping ocm );

    void                            SetRepresentedOutputType    ( CustomOutputType cot );

    void                            SetEnabled                  ( bool isEnabled );

};

} //nrl
} //bv
