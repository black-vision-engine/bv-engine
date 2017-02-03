#pragma once


namespace bv { namespace nrl {

class OutputLogicDesc
{
private:
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
    
    <videocards>
        <videocard name="BlueFish">
            <deviceID value="1" />
            <channels>
                <channel name="A" renderer="1" >
                    <output type="FILL_KEY" resolution="1080" refresh="5000" interlaced="true" flipped="true" referenceMode="FREERUN" referenceH="0" referenceV="0" />
                </channel>
                <channel name="C" renderer="1" >
                    <output type="KEY" resolution="1080" refresh="5000" interlaced="true" flipped="true" referenceMode="FREERUN" referenceH="0" referenceV="0" />
                </channel>
            </channels>
        </videocard>
    </videocards>
    */
    // FIXME: implement either a map or something based on currend vcm initialization code
public:


};

} //nrl
} //bv
