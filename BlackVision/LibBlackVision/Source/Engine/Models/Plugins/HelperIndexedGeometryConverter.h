#pragma once

#include "IndexedGeometry.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {



class IndexedGeometryConverter
{
private:


public:
	IndexedGeometryConverter(void);
	~IndexedGeometryConverter(void);

	void makeStrip( IndexedGeometry& mesh, Float3AttributeChannelPtr verts);
};


}
}
