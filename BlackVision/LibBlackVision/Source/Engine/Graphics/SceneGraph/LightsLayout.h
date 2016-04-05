#pragma once

#include "Mathematics/glm_inc.h"
#include "Engine/Graphics/Resources/UniformBuffer.h"


namespace bv
{


// *****************************
//
enum class UniformBlockLayoutType : int
{
    STD140 = 0
};

// *****************************
//
template< UniformBlockLayoutType L >
class LightsLayout
{
public:

    static const UInt32         MaxLightsNum        = 8;

private:

    UniformBlockLayoutUPtr      m_blockLayout;

private:

                                LightsLayout        ();
                                
    void                        ComputeOffset       ( UInt32 idx );

public:

    const UniformBlockLayout *  GetBlockLayout      () const;

    static LightsLayout &       Instance            ();

};

} // bv

#include "LightsLayout.inl"

