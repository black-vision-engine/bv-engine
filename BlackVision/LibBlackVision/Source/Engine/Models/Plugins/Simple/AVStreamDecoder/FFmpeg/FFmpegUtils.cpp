#include "stdafx.h"

#include "FFmpegUtils.h"


namespace bv {


// *********************************
//
AVPixelFormat			FFmpegUtils::ToFFmpegPixelFormat		( VideoPixelFormat format )
{
	//FIXME
	switch( format )
	{
	case VideoPixelFormat::VPF_RGBA:
		return AV_PIX_FMT_RGBA;
	case VideoPixelFormat::VPF_RGB24:
		return AV_PIX_FMT_RGB24;
	case VideoPixelFormat::VPF_RGB8:
		return AV_PIX_FMT_RGB8;
	case VideoPixelFormat::VPF_RGB4:
		return AV_PIX_FMT_RGB4;
	case VideoPixelFormat::VPF_BGRA:
		return AV_PIX_FMT_RGBA;
	case VideoPixelFormat::VPF_BGR24:
		return AV_PIX_FMT_RGB24;
	case VideoPixelFormat::VPF_BGR8:
		return AV_PIX_FMT_RGB8;
	case VideoPixelFormat::VPF_BGR4:
		return AV_PIX_FMT_RGB4;

	case VideoPixelFormat::VPF_YUV420P:
		return AV_PIX_FMT_YUV420P;
	case VideoPixelFormat::VPF_YUV422P:
		return AV_PIX_FMT_YUV422P;
	case VideoPixelFormat::VPF_YUV444P:
		return AV_PIX_FMT_YUV444P;
	case VideoPixelFormat::VPF_YUV410P:
		return AV_PIX_FMT_YUV410P;
	case VideoPixelFormat::VPF_YUV411P:
		return AV_PIX_FMT_YUV411P;
	case VideoPixelFormat::VPF_YUVA420P:
		return AV_PIX_FMT_YUVA420P;
	case VideoPixelFormat::VPF_YUVA422P:
		return AV_PIX_FMT_YUVA422P;
	case VideoPixelFormat::VPF_YUVA444P:
		return AV_PIX_FMT_YUVA444P;

	case VideoPixelFormat::VPF_YVYU422:
		return AV_PIX_FMT_YVYU422;
	case VideoPixelFormat::VPF_YUYV422:
		return AV_PIX_FMT_YUYV422;
	case VideoPixelFormat::VPF_UYVY422:
		return AV_PIX_FMT_UYVY422;
	default:
		return AV_PIX_FMT_NONE;
	}
}

// *********************************
//
AVPixelFormat				FFmpegUtils::ToFFmpegPixelFormat		( TextureFormat format ) 
{
	//FIXME
	switch( format )
	{
	//case TextureFormat::F_A32FR32FG32FB32F:
	//case TextureFormat::F_R32FG32FB32F:
	//case TextureFormat::F_A32F:
	case TextureFormat::F_A8R8G8B8:
		return AVPixelFormat::AV_PIX_FMT_BGRA;
	case TextureFormat::F_R8G8B8:
		return AVPixelFormat::AV_PIX_FMT_BGR24;
	//case TextureFormat::F_A8:
	//case TextureFormat::F_L8:
	default:
		return AVPixelFormat::AV_PIX_FMT_NONE;
	}
}

// *********************************
//
Int64				        FFmpegUtils::ConvertToMiliseconds       ( Float64 seconds )
{
    return ( Int64 )( 1000 * seconds );
}

// *********************************
//
std::string                 FFmpegUtils::AVErrorToString            ( Int32 errCode )
{
    std::string errorStr;
    errorStr.resize( 128 );

    av_strerror( errCode, &errorStr[ 0 ], 128 );

    return errorStr;
}

// *********************************
//
void                        FFmpegUtils::AVCustomLog                ( void * ptr, int level, const char * fmt, va_list vl )
{
    static std::mutex mutex;

    if( level >= 0 || level <= 32 )
    {
        std::unique_lock< std::mutex > lock( mutex );
        static int print_prefix = 1;
        static std::string message;
        std::string buf;
        buf.resize( 1024 );

        av_log_format_line( ptr, level, fmt, vl, &buf[ 0 ], ( int ) buf.size(), &print_prefix );
        message.append( buf.substr( 0, strlen( &buf[ 0 ] ) ) );

        if( print_prefix > 0 && message.size() > 0 )
        {
            if( message[ message.size() - 1 ] == '\n' )
                message = message.substr( 0, message.size() - 1 );

            if( level == 32 )
                LOG_MESSAGE( SeverityLevel::info ) << message;
            else if ( level == 24 )
                LOG_MESSAGE( SeverityLevel::warning ) << message;
            else if( level == 16 )
                LOG_MESSAGE( SeverityLevel::error ) << message;
            else if ( level == 0 || level == 8 )
                LOG_MESSAGE( SeverityLevel::critical ) << message;

            message.clear();
        }
    }
}

} //bv
