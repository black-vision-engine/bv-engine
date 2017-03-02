#pragma once

namespace bv {

namespace videocards {
    class VideoCardManager;
}

class BVAppLogic;
class BVProject;

class BVServiceProvider
{
private:
    videocards::VideoCardManager *      m_vcm;

#ifdef BV_TESTS

    BVProject *                         m_bvProj;

    void                                RegisterBVProject       ( BVProject * bvProj );

#endif // BV_TESTS

    explicit                            BVServiceProvider       ();

    void                                RegisterVideoCardManager( videocards::VideoCardManager * vcm );



public:

    videocards::VideoCardManager *      GetVideoCardManager();

#ifdef BV_TESTS

    BVProject *                         GetBVProject            ();

#endif // BV_TESTS

    static BVServiceProvider &          GetInstance();

friend class BVAppLogic;
};

} // bv