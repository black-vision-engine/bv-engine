#pragma once

#include "CoreDEF.h"


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

    explicit                            BVServiceProvider           ();

    void                                RegisterVideoCardManager    ( videocards::VideoCardManager * vcm );
    void                                UnregisterVideoCardManager  ();



public:

    videocards::VideoCardManager *      GetVideoCardManager();

#ifdef BV_TESTS

    BVProject *                         GetBVProject            ();

#endif // BV_TESTS

    static BVServiceProvider &          GetInstance();

private:
    friend class BVAppLogic;
    FRIEND_TEST_ACCESSOR( BVServiceProvider )
};

} // bv