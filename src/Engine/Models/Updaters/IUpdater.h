#pragma once

#include <vector>

namespace bv
{

class IUpdater
{
public:
    virtual void                Update(float t) = 0;
};


class UpdatersManager
{
private:
    std::vector<IUpdater*> m_updaters;
public:

    void Update(float t)
    {
        for(auto u : m_updaters)
        {
            u->Update(t);
        }
    }

    void RegisterUpdater(IUpdater* up)
    {
        m_updaters.push_back(up);
    }

    static UpdatersManager& get()
    {
        static UpdatersManager instance = UpdatersManager();
        return instance;
    }
};


} // bv