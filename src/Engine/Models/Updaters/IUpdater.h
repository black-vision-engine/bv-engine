#pragma once

#include <vector>

namespace bv
{

class IUpdater
{
public:

    virtual         ~IUpdater   () {}

    virtual void    Update      ( float t ) = 0;
};


class UpdatersManager
{
private:

    std::vector<IUpdater*> m_updaters;

public:

    ~UpdatersManager    ()
    {
        for( auto u : m_updaters )
        {
            delete u;
        }
    }

    void Update ( float t )
    {
        for( auto updater : m_updaters )
        {
            updater->Update( t );
        }
    }

    void RegisterUpdater    ( IUpdater * updater )
    {
        m_updaters.push_back( updater );
    }

    static UpdatersManager& get()
    {
        static UpdatersManager instance = UpdatersManager();
        return instance;
    }

};


} // bv
