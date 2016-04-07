#pragma once

#include "Engine/Models/Lights/HelperModelLights.h"
#include "Engine/Models/Lights/ModelBaseLight.h"


namespace bv
{


// *************************************
//
template<>
LightsLayout< UniformBlockLayoutType::STD140 >::LightsLayout      ()
{
    m_blockLayout = std::unique_ptr< UniformBlockLayout >( new UniformBlockLayout() );

    m_blockLayout->blockName = "Lights";

    UInt32 count = 0;
    for( UInt32 i = 0; i < ( UInt32 )LightType::LT_TOTAL; ++i )
    {
        //FIXME: it shouldn't use model class - data should be passed as descriptor somehow
        auto light = model::HelperModelLights::CreateModelLight( ( LightType )i, nullptr );
        
        for( UInt32 j = 0; j < MaxLightsNum; ++j )
        {
            for( auto value : light->GetValues() )
            {
                std::ostringstream ss;
                ss << light->GetTypeName() << "[" << j << "]." << value->GetName();

                UniformDesc desc;
                desc.name = ss.str();
                desc.size = ( UInt32 )value->GetSize();
                desc.align = UniformBuffer::TypeAlignMap[ ( UInt32 )value->GetType() ];
                m_blockLayout->uniformDescs.push_back( desc );
                ComputeOffset( count );
                count++;
            }
        }


        std::ostringstream ss;
        ss << light->GetTypeName() << "Num";

        UniformDesc desc;
        desc.name = ss.str();
        desc.size = sizeof( Float32 );
        desc.align = UniformBuffer::TypeAlignMap[ ( UInt32 )ParamType::PT_INT ];
        desc.offset = 16 * ( ( UInt32 )( m_blockLayout->uniformDescs[ count - 1 ].offset ) / 16 + 1 ); //array offset
        m_blockLayout->uniformDescs.push_back( desc );
        count++;

        delete light;
    }

    auto & lastDesc = m_blockLayout->uniformDescs[ count - 1 ];
    m_blockLayout->size = lastDesc.offset + lastDesc.size;
}

// *************************************
//
template<>
void    LightsLayout< UniformBlockLayoutType::STD140 >::ComputeOffset      ( UInt32 idx )
{
    auto offset = 0;

    if( idx > 0 )
    {
        assert( idx < m_blockLayout->uniformDescs.size() );

        auto & prevDesc = m_blockLayout->uniformDescs[ idx - 1 ];
        auto prev = prevDesc.offset + prevDesc.size;

        auto descAlign = m_blockLayout->uniformDescs[ idx ].align;

        if( prev % descAlign == 0 )
        {
            offset = prev;
        }
        else
        {
            offset = prevDesc.offset + descAlign;
        }
    }

    m_blockLayout->uniformDescs[ idx ].offset = offset;
}

// *************************************
//
template<>
LightsLayout< UniformBlockLayoutType::STD140 > &    LightsLayout< UniformBlockLayoutType::STD140 >::Instance                 ()
{
    static LightsLayout instance;
    return instance;
}

// *************************************
//
template<>
const UniformBlockLayout *              LightsLayout< UniformBlockLayoutType::STD140 >::GetBlockLayout            () const
{
    return m_blockLayout.get();
}

} // bv
