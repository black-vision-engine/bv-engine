#pragma once

#include <map>
#include <vector>
#include <string>

namespace bv
{

enum class PluginsGroups
{
    object = 1,
    texture,
    material,
    light,
    behavior,
    util,
};

class PluginInfo
{
    std::string     m_name;
    PluginsGroups   m_group;

public:

    explicit PluginInfo(const std::string& name, PluginsGroups group)
        : m_name(name), m_group(group)
    {}
    ~PluginInfo() {}

    const std::string&  getName() const     { return m_name; }
    const PluginsGroups getGroup() const    { return m_group; }
};

class PluginsManager
{
    typedef std::map<std::pair<const PluginInfo*, const PluginInfo*>, bool> CoexMapType;

    std::vector<PluginInfo*>    m_plugins;
    CoexMapType                 m_coexMap;

    explicit PluginsManager();
    ~PluginsManager() {}

    PluginInfo* isRegistered(const std::string& name, int* index = nullptr) const;

public:
    const PluginInfo*   registerPlugin(const std::string& name, PluginsGroups group);
    void                unregisterPlugin(const std::string& name);

    void                allowCoex(const std::string& name0, const std::string& name1);
    void                disallowCoex(const std::string& name0, const std::string& name1);

    void                allowCoex(const std::string& name, const std::vector<std::string>& nameList);
    void                disallowCoex(const std::string& name, const std::vector<std::string>& nameList);

    bool                isCoexAllowed(const std::string& name0, const std::string& name1) const;
    bool                isCoexAllowed(const PluginInfo* pl0, const PluginInfo* pl1) const;

    bool                isCoexAllowed(const std::string& name, const std::vector<std::string>& nameList) const;
    bool                isCoexAllowed(const PluginInfo* pl0, const std::vector<const PluginInfo*> plList) const;

    static PluginsManager&  get();
};

} // bv
