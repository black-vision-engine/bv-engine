#pragma once

#include <hash_map>
#include <vector>
#include <string>

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"


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

    TOutputPropertiesVector m_outputProperties;

    unsigned int            m_width;
    unsigned int            m_height;

    RenderChannelType       m_selectedRenderedChannel;

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
    /*
    <property name="RenderChannels">
        <property name="RenderChannel_1" value="enabled" />
        <property name="RenderChannel_2" value="enabled" />
        <property name="RenderChannel_3" value="disabled" />
        <property name="RenderChannel_4" value="disabled" />
    </property>

    <PhysicalOutputs>
        <Preview width="640" height="360" renderChannel="RenderChannel_1" windowType="FULLSCREEN" top="0" left="0" /> <!-- WINDOWED | FRAMELESS | FULLSCREEN -->
        <Videocard name="BlueFish">
            <DeviceID value="1" />
            <Channels>
                <Channel name="A" renderer="1" renderChannel="RenderChannel_1" >
                    <Output type="FILL_KEY" resolution="HD" refresh="5000" interlaced="true" flipped="true" referenceMode="FREERUN" referenceH="0" referenceV="0" />
                </Channel>
                <Channel name="C" renderer="1" renderChannel="RenderChannel_2" >
                    <Output type="KEY" resolution="SD_16_9" refresh="5000" interlaced="true" flipped="true" referenceMode="FREERUN" referenceH="0" referenceV="0" />
                </Channel>
            </Channels>
        </Videocard>
        <Stream type="SharedMemory" name="bv_1" namespace="bv" width="640" height="360" renderChannel="RenderChannel_1" />
    </PhysicalOutputs>
    */

    // FIXME: implement either a map or something based on currend vcm initialization code
public:

};

} //nrl
} //bv
