
#pragma once

#include "BlueFish/inc/BlueVelvet4.h"
#include <string>
#pragma pack(push, blue_timecode_struct, 1)
struct BlueTimeCodeStruct
{
		unsigned __int64	unit_frame : 4,			//00-03:	frame
		binary1 : 4,				//04-07:	not used, set to 0
		ten_frame : 2,			//08-09:	frame tens
		drop_frame_flag : 1,		//10:		only 30/60 framerates: drop frame flag, otherwise 0
		color_frame_flag : 1,		//11:		not used, set to 0
		binary2 : 4,				//12-15:	not used, set to 0
		unit_second : 4,			//16-19:	second
		binary3 : 4,				//20-23:	not used, set to 0
		ten_second : 3,			//24-26:	second tens
		field_bit : 1,			//27:		24/30/60 framerates: odd parity; 25/50 framerates: not used, set to 0
		binary4 : 4,				//28-31:	not used, set to 0
		unit_minute : 4,			//32-35:	minute
		binary5 : 4,				//36-39:	not used, set to 0
		ten_minute : 3,			//40-42:	minute tens
		binarygroupflag43 : 1,	//43:		not used, set to 0
		binary6 : 4,				//44-47:	not used, set to 0
		unit_hours : 4,			//48-51:	hour
		binary7 : 4,				//52-55:	not used, set to 0
		ten_hours : 2,			//56-57:	hour tens
		binarygroupflag58 : 1,	//58:		not used, set to 0
		binarygroupflag59 : 1,	//59:		24/30/60 framerates: not used, set to 0; 25/50 framerates: odd parity
		binary8 : 4;				//60-63:	not used, set to 0
};

struct TimeCode
{
	union
	{
		struct BlueTimeCodeStruct struct_timecode;
		unsigned __int64 timecode_u64;
	};
};
#pragma pack(pop, blue_timecode_struct)

struct blue_videomode_info
{
	bool			bIs3G;
	float			fFrameAspectRatioX;
	float			fFrameAspectRatioY;
	std::string		strVideoModeFriendlyName;
	unsigned int	nVideoMode;
	unsigned int	nHeight;
	unsigned int	nWidth;
	unsigned int	nFrameRate;
	bool			bIs1001FrameRate;
	bool			bIsProgressive;
	unsigned int	nAudioFrameSequence[5];
};

struct blue_videomode_info gVideoModeInfo[] = {
	{ false,4.0,3.0,("PAL"),VID_FMT_PAL,576,720,25,false,false,{ 1920,1920,1920,1920,1920 } },
	{ false,4.0,3.0,("NTSC"),VID_FMT_NTSC,486,720,30,true,false,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("720P 59.94"),VID_FMT_720P_5994,720,1280,60,true,true,{ 801,800,801,801,801 } },
	{ false,16.0,9.0,("720P 60"),VID_FMT_720P_6000,720,1280,60,false,true,{ 800,800,800,800,800 } },
	{ false,16.0,9.0,("1080PSF 23.97"),VID_FMT_1080PSF_2397,1080,1920,24,true,false,{ 2002,2002,2002,2002,2002 } },
	{ false,16.0,9.0,("1080PSF 24.00"),VID_FMT_1080PSF_2400,1080,1920,24,false,false,{ 2000,2000,2000,2000,2000 } },
	{ false,16.0,9.0,("1080P 23.97"),VID_FMT_1080P_2397,1080,1920,24,true,true,{ 2002,2002,2002,2002,2002 } },
	{ false,16.0,9.0,("1080P 24"),VID_FMT_1080P_2400,1080,1920,24,false,true,{ 2000,2000,2000,2000,2000 } },
	{ false,16.0,9.0,("1080I 50"),VID_FMT_1080I_5000,1080,1920,25,false,false,{ 1920,1920,1920,1920,1920 } },
	{ false,16.0,9.0,("1080I 59.94"),VID_FMT_1080I_5994,1080,1920,30,true,false,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("1080I 60"),VID_FMT_1080I_6000,1080,1920,30,false,false,{ 1600,1600,1600,1600,1600 } },
	{ false,16.0,9.0,("1080P 25"),VID_FMT_1080P_2500,1080,1920,25,false,true,{ 1920,1920,1920,1920,1920 } },
	{ false,16.0,9.0,("1080P 29.97"),VID_FMT_1080P_2997,1080,1920,30,true,true,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("1080P 30"),VID_FMT_1080P_3000,1080,1920,30,false,true,{ 1600,1600,1600,1600,1600 } },
	{ false,4.0,3.0,("HSDL 14"),VID_FMT_HSDL_1498,1556,2048,15,true,false,{ 1602,1601,1602,1601,1602 } },
	{ false,4.0,3.0,("HSDL 15"),VID_FMT_HSDL_1500,1556,2048,15,false,false,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("720P 50"),VID_FMT_720P_5000,720,1280,50,false,true,{ 960,960,960,960,960 } },
	{ false,16.0,9.0,("720P 23.98"),VID_FMT_720P_2398,720,1280,24,true,true,{ 2002,2002,2002,2002,2002 } },
	{ false,16.0,9.0,("720P 24"),VID_FMT_720P_2400,720,1280,24,false,true,{ 2000,2000,2000,2000,2000 } },
	{ false,16.0,9.0,("2K PSF 23.97"),VID_FMT_2048_1080PSF_2397,1080,2048,24,true,false,{ 2002,2002,2002,2002,2002 } },
	{ false,16.0,9.0,("2K PSF 24"),VID_FMT_2048_1080PSF_2400,1080,2048,24,false,false,{ 2000,2000,2000,2000,2000 } },
	{ false,16.0,9.0,("2K P 23.97"),VID_FMT_2048_1080P_2397,1080,2048,24,true,true,{ 2002,2002,2002,2002,2002 } },
	{ false,16.0,9.0,("2K P 24"),VID_FMT_2048_1080P_2400,1080,2048,24,false,true,{ 2000,2000,2000,2000,2000 } },
	{ false,16.0,9.0,("1080PSF 25"),VID_FMT_1080PSF_2500,1080,1920,25,false,false,{ 1920,1920,1920,1920,1920 } },
	{ false,16.0,9.0,("1080PSF 29.97"),VID_FMT_1080PSF_2997,1080,1920,30,true,false,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("1080PSF 30"),VID_FMT_1080PSF_3000,1080,1920,30,false,false,{ 1600,1600,1600,1600,1600 } },
	{ true,16.0,9.0,("1080P 50"),VID_FMT_1080P_5000,1080,1920,50,false,true,{ 960,960,960,960,960 } },
	{ true,16.0,9.0,("1080P 59.94"),VID_FMT_1080P_5994,1080,1920,60,true,true,{ 801,800,801,801,801 } },
	{ true,16.0,9.0,("1080P 60"),VID_FMT_1080P_6000,1080,1920,60,false,true,{ 800,800,800,800,800 } },
	{ false,16.0,9.0,("720P 25"),VID_FMT_720P_2500,720,1280,25,false,true,{ 1920,1920,1920,1920,1920 } },
	{ false,16.0,9.0,("720P 29.97"),VID_FMT_720P_2997,720,1280,30,true,true,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("720P 30"),VID_FMT_720P_3000,720,1280,30,false,true,{ 1600,1600,1600,1600,1600 } },
	{ false,16.0,9.0,("DVB-ASI"),VID_FMT_DVB_ASI,720,1280,50,false,true,{ 960,960,960,960,960 } },
	{ false,16.0,9.0,("2K PSF 25"),VID_FMT_2048_1080PSF_2500,1080,2048,25,false,false,{ 1920,1920,1920,1920,1920 } },
	{ false,16.0,9.0,("2K PSF 29.97"),VID_FMT_2048_1080PSF_2997,1080,2048,30,true,false,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("2K PSF 30"),VID_FMT_2048_1080PSF_3000,1080,2048,30,false,false,{ 1600,1600,1600,1600,1600 } },
	{ false,16.0,9.0,("2K P 25"),VID_FMT_2048_1080P_2500,1080,2048,25,false,true,{ 1920,1920,1920,1920,1920 } },
	{ false,16.0,9.0,("2K P 29.97"),VID_FMT_2048_1080P_2997,1080,2048,30,true,true,{ 1602,1601,1602,1601,1602 } },
	{ false,16.0,9.0,("2K P 30"),VID_FMT_2048_1080P_3000,1080,2048,30,false,true,{ 1600,1600,1600,1600,1600 } },
	{ true,16.0,9.0,("2K P 50"),VID_FMT_2048_1080P_5000,1080,2048,50,false,true,{ 960,960,960,960,960 } },
	{ true,16.0,9.0,("2K P 59.94"),VID_FMT_2048_1080P_5994,1080,2048,60,true,true,{ 801,800,801,801,801 } },
	{ true,16.0,9.0,("2K P 60"),VID_FMT_2048_1080P_6000,1080,2048,60,false,true,{ 800,800,800,800,800 } },
	{ true,16.0,9.0,("1080P 48"),VID_FMT_1080P_4800,1080,1920,48,false,true,{ 1000,1000,1000,1000,1000 } },
	{ true,16.0,9.0,("2K P 48"),VID_FMT_2048_1080P_4800,1080,2048,48,false,true,{ 1000,1000,1000,1000,1000 } },
	{ false,1.0,1.0,("VID_FMT_INVALID"),VID_FMT_INVALID,0,0,0,false,true,{ 0,0,0,0,0 } }
};

unsigned int GetAudioSamplesPerFrame(unsigned int nVideoMode, unsigned int frame_no)
{

	if (nVideoMode < VID_FMT_INVALID)
	{
		return gVideoModeInfo[nVideoMode].nAudioFrameSequence[frame_no % 5];
	}

	return 0;
}

void GenerateTimeCode(unsigned int nIndex,				//frame number
	unsigned int nFramesPerSecond,	//frame rate
	BOOL bIsDropFrame,				//drop frame mode: 23.98, 29.97 or 59.94
	BOOL bIsProgressive,
	BOOL bIsRp188LTC,				//if timecode is RP188 LTC bit 59 is treated as an odd parity bit (firmware makes it even parity due to another bit)
	struct TimeCode& tc,
	BOOL bIsField2)
{
	unsigned int nFrames, second, minutes, hour, Xor32_0, Xor32_1;
	unsigned short Xor16_0, Xor16_1;
	unsigned char Xor8_0, Xor8_1;
	tc.timecode_u64 = 0LL;

	if (bIsField2 && !bIsRp188LTC)
	{
		if (nFramesPerSecond == 25 || nFramesPerSecond == 50)
			tc.struct_timecode.binarygroupflag59 = 1;
		else //24, 30 and 60 formats
			tc.struct_timecode.field_bit = 1;
	}

	if (nFramesPerSecond > 30)
	{
		nFramesPerSecond = nFramesPerSecond >> 1; //divide by 2, timecode only goes up to 30
		nIndex >>= 1;
	}

	hour = nIndex / (60 * 60 * nFramesPerSecond);
	minutes = nIndex % (60 * 60 * nFramesPerSecond);
	second = minutes % (60 * nFramesPerSecond);
	nFrames = second %nFramesPerSecond;
	second = second / (nFramesPerSecond);
	minutes = minutes / (60 * nFramesPerSecond);

	tc.struct_timecode.unit_frame = (nFrames % 10);
	tc.struct_timecode.ten_frame = (nFrames / 10);
	tc.struct_timecode.unit_second = (second % 10);
	tc.struct_timecode.ten_second = (second / 10);
	tc.struct_timecode.unit_minute = (minutes % 10);
	tc.struct_timecode.ten_minute = (minutes / 10);
	tc.struct_timecode.unit_hours = (hour % 10);
	tc.struct_timecode.ten_hours = (hour / 10);

	if (bIsDropFrame)
		tc.struct_timecode.drop_frame_flag = 1;

	if (bIsRp188LTC)
	{
		Xor32_0 = (unsigned int)(tc.timecode_u64 & 0x00000000FFFFFFFF);
		Xor32_1 = (unsigned int)(tc.timecode_u64 >> 32);
		Xor32_0 ^= Xor32_1;

		Xor16_0 = (unsigned short)(Xor32_0 & 0x0000FFFF);
		Xor16_1 = (unsigned short)(Xor32_0 >> 16);
		Xor16_0 ^= Xor16_1;

		Xor8_0 = (unsigned char)(Xor16_0 & 0x00FF);
		Xor8_1 = (unsigned char)(Xor16_0 >> 8);
		Xor8_0 ^= Xor8_1;

		int parity = 0;
		for (int i = 0; i<8; i++)
		{
			if (Xor8_0 & (1 << i))
				parity++;
		}
		if (parity & 0x1)	parity = 0;
		else				parity = 1;

		if (nFramesPerSecond == 25 || nFramesPerSecond == 50)
			tc.struct_timecode.binarygroupflag59 = parity;
		else //24, 30 and 60
			tc.struct_timecode.field_bit = parity;
	}
}
