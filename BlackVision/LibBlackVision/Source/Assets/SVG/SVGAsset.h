#pragma once

#include "Assets/Asset.h"

#include "Assets/Mesh/MeshAsset.h"

namespace bv {

class SVGAsset : public MeshAsset
{
public:
                                        SVGAsset        ( const std::string & key );
};

}
