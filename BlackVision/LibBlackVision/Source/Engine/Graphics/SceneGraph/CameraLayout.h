#pragma once

#include "Engine/Graphics/Resources/UniformBuffer.h"

namespace bv
{


class CameraLayout
{
private:

    static std::string          UNIFORM_BLOCK_NAME;
    static std::string          UNIFORM_BLOCK_DEF_PATH;

private:

    UniformBlockLayoutUPtr      m_blockLayout;

public:
    CameraLayout();
    ~CameraLayout();


    const UniformBlockLayout *  GetBlockLayout          () const;

    static CameraLayout &       Instance                ();
};



}	// bv