#pragma once

#include "base/CmdBase.h"
#include <string>

class CmdLoadAsset : public CmdBase
{
public:
    std::wstring        m_pluginName;
    std::wstring        m_nodeName;
    std::wstring        m_assetData;
public:
    CmdLoadAsset()
    {
        Type    =   CmdType::LOAD_ASSET;
        CmdName =   L"LOAD_ASSET";
    }
    ~CmdLoadAsset() {};

    void parse( const wObject& obj )
    {
        m_pluginName    = ParseWstring( obj, L"plugin" );
        m_nodeName      = ParseWstring( obj, L"path" );
        m_assetData     = ParseWstring( obj, L"value" );
    }

};
