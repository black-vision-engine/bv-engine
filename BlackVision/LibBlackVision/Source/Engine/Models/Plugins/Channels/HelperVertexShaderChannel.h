#pragma once

#include "Engine/Models/Plugins/Plugin.h"


namespace bv { namespace model
{


class HelperVertexShaderChannel
{
public:
    static void         InverseTextureMatrix        ( IPluginParamValModelPtr pluginParamValModel, const std::string & paramName );
};

}   // model
}   // bv
