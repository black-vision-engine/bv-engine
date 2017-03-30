#pragma once

namespace bv {

namespace videocards {
    class VideoCardManager;
}

class BVAppLogic;

class BVServiceProvider
{
private:
    videocards::VideoCardManager *      m_vcm;

    explicit                            BVServiceProvider       ();

    void                                RegisterVideoCardManager( videocards::VideoCardManager * vcm );

public:

    videocards::VideoCardManager *      GetVideoCardManager();


    static BVServiceProvider &          GetInstance();

friend class BVAppLogic;
};

} // bv