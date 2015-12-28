#pragma once

#include "FFmpegDef.h"
#include "Engine/Types/Enums.h"

//#include  "Mathematics/glm_inc.h"

namespace bv
{

//class FFmpegPixelFormat
//{
//private:
//	DisplayPixelLayout		m_layout;
//	glm::uvec4				m_format;
//	UInt16					m_bpp;
//};

class FFmpegUtils
{

public:
	static AVPixelFormat		ToFFmpegPixelFormat		( TextureFormat format );
	static AVPixelFormat		ToFFmpegPixelFormat		( VideoPixelFormat format );

};

} //bv
