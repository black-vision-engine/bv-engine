#pragma once

namespace bv
{

// NOTE: used by TextureBase::m_sPixelSize (TextureBase.cpp)
// NOTE: used by GLConstantTextureFormat and GLConstantTextureType (PdrConstants.cpp)
enum class TextureFormat : int
{
	F_A32FR32FG32FB32F = 0,
	F_R32FG32FB32F,
	F_A32F,
	F_A8R8G8B8,
	F_R8G8B8,
	F_A8,
	F_L8,
	F_TOTAL
};

enum class VideoPixelFormat : int
{
	//RGB
	VPF_RGBA = 0,
	VPF_RGB24,
	VPF_RGB8,
	VPF_RGB4,
	VPF_BGRA,
	VPF_BGR24,
	VPF_BGR8,
	VPF_BGR4,

	//planar YUV
	VPF_YUV420P,
	VPF_YUV422P,
	VPF_YUV444P,
	VPF_YUV410P,
	VPF_YUV411P,
	VPF_YUVA420P,
	VPF_YUVA422P,
	VPF_YUVA444P,

	//packed YUV
	VPF_YVYU422,
	VPF_YUYV422,
	VPF_UYVY422,
};

} // bv