#pragma once

namespace bv {
	namespace videocards {

		class BVTimeCode
		{
		public:
			int h;
			int m;
			int s;
			int frame;

			BVTimeCode();
			~BVTimeCode();
		};

	}
}