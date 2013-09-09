#include "PluginsManager.h"

namespace bv
{
    PluginsManager::PluginsManager()
    {}

    PluginsManager& PluginsManager::get()
    {
        static PluginsManager instance = PluginsManager();
        return instance;
    }

    const PluginInfo* PluginsManager::registerPlugin(const std::string& name, PluginsGroups group)
    {
        PluginInfo* ret = nullptr;

        if(isRegistered(name) != nullptr)
        {
            PluginInfo* ret = new PluginInfo(name, group);
            m_plugins.push_back(ret);
        }

        return ret;
    }

    void PluginsManager::unregisterPlugin(const std::string& name)
    {
        int i = -1;
        if(PluginInfo* pl = isRegistered(name, &i))
        {
            delete pl;
            m_plugins.erase(m_plugins.begin() + i);
        }
    }

    void PluginsManager::allowCoex(const std::string& name0, const std::string& name1)
    {
        if(PluginInfo* pl0 = isRegistered(name0))
            if(PluginInfo* pl1 = isRegistered(name1))
            {
                m_coexMap[std::make_pair(pl0,pl1)] = true;
                m_coexMap[std::make_pair(pl1,pl0)] = true;
            }
    }

    void PluginsManager::disallowCoex(const std::string& name0, const std::string& name1)
    {
        if(PluginInfo* pl0 = isRegistered(name0))
            if(PluginInfo* pl1 = isRegistered(name1))
            {
                m_coexMap[std::make_pair(pl0,pl1)] = false;
                m_coexMap[std::make_pair(pl1,pl0)] = false;
            }
    }

    void PluginsManager::allowCoex(const std::string& name, const std::vector<std::string>& nameList)
    {
        for(auto pl : nameList)
        {
            allowCoex(name, pl);
        }
    }

    void PluginsManager::disallowCoex(const std::string& name, const std::vector<std::string>& nameList)
    {
        for(auto pl : nameList)
        {
            disallowCoex(name, pl);
        }
    }

    PluginInfo* PluginsManager::isRegistered(const std::string& name, int* index) const
    {
        for(unsigned int i = 0; i < m_plugins.size(); ++i)
            if(m_plugins[i]->getName() == name)
            {
                *index = i;
                return m_plugins[i];
            }

        return nullptr;
    }

    bool PluginsManager::isCoexAllowed(const std::string& name0, const std::string& name1) const
    {
        if(PluginInfo* pl0 = isRegistered(name0))
            if(PluginInfo* pl1 = isRegistered(name1))
                return isCoexAllowed(pl0, pl1);

        return false;
    }

    bool PluginsManager::isCoexAllowed(const PluginInfo* pl0, const PluginInfo* pl1) const
    {
        auto it = m_coexMap.find(std::pair<const PluginInfo*, const PluginInfo*>(pl0, pl1));

        if(it != m_coexMap.end())
            return (*it).second == true;
        else
            if(pl0->getGroup() != pl1->getGroup())
                return true;
            else
                return false;
    }

    bool PluginsManager::isCoexAllowed(const std::string& name, const std::vector<std::string>& nameList) const
    {
        for(auto n: nameList)
        {
            if(!isCoexAllowed(name, n))
                return false;
        }

        return true;
    }

    bool PluginsManager::isCoexAllowed(const PluginInfo* pl, const std::vector<const PluginInfo*> plList) const
    {
        for(auto p: plList)
        {
            if(!isCoexAllowed(pl, p))
                return false;
        }

        return true;
    }

} // bv
