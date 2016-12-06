#include "BVTimeCode.h"
namespace bv {
	namespace videocards {

	
		BVBluefishTimeCode BVTimeCode::GetTimeCode(bool bIsRp188LTC)
		{
			BVBluefishTimeCode tc;

			tc.timecode_u64 = 0LL;

			unsigned short Xor16_0, Xor16_1;
			unsigned char Xor8_0, Xor8_1;
			unsigned int Xor32_0, Xor32_1;
			tc.struct_timecode.unit_frame = (frame % 10);
			tc.struct_timecode.ten_frame = (frame / 10);
			tc.struct_timecode.unit_second = (s % 10);
			tc.struct_timecode.ten_second = (s / 10);
			tc.struct_timecode.unit_minute = (m % 10);
			tc.struct_timecode.ten_minute = (m / 10);
			tc.struct_timecode.unit_hours = (h % 10);
			tc.struct_timecode.ten_hours = (h / 10);

			
			tc.struct_timecode.drop_frame_flag = 0;

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

				if (frame == 25 || frame == 50)
					tc.struct_timecode.binarygroupflag59 = parity;
				else //24, 30 and 60
					tc.struct_timecode.field_bit = parity;
			}
			return tc;
		}	

		

		void BVTimeCode::GenerateTimeCode(unsigned int nIndex,				//frame number
			unsigned int nFramesPerSecond,	//frame rate
			bool bIsDropFrame,				//drop frame mode: 23.98, 29.97 or 59.94
			bool bIsProgressive,
			bool bIsRp188LTC,				//if timecode is RP188 LTC bit 59 is treated as an odd parity bit (firmware makes it even parity due to another bit)
			struct BVBluefishTimeCode& tc,
			bool bIsField2)
		{
			{bIsProgressive;}

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
		BVTimeCode::BVTimeCode()
		{
			
		}


		BVTimeCode::~BVTimeCode()
		{
		}

	}
}
