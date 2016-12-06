#pragma once
//#include "BlueFish/inc/BlueVelvet4.h"
#include <string>

namespace bv {
	namespace videocards {


		
		struct BVBlueTimeCodeStruct
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

		struct BVBluefishTimeCode
		{
			union
			{
				struct BVBlueTimeCodeStruct struct_timecode;
				unsigned __int64 timecode_u64;
			};
		};
		



		class BVTimeCode
		{
		public:
			int h;
			int m;
			int s;
			int frame;
			
			
			BVBluefishTimeCode GetTimeCode(bool bIsRp188LTC);


			void GenerateTimeCode(unsigned int nIndex,				//frame number
				unsigned int nFramesPerSecond,	//frame rate
				bool bIsDropFrame,				//drop frame mode: 23.98, 29.97 or 59.94
				bool bIsProgressive,
				bool bIsRp188LTC,				//if timecode is RP188 LTC bit 59 is treated as an odd parity bit (firmware makes it even parity due to another bit)
				struct BVBluefishTimeCode& tc,
				bool bIsField2);
			

			BVTimeCode();
			~BVTimeCode();
		};

	}
}