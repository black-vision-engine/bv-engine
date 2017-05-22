#include "stdafxBVApp.h"

#include "MockScenes.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/GeometryMultiRectPlugin.h"
#include "Engine/Types/Enums.h"


#define _USE_MATH_DEFINES
#include <math.h>
//#include <cmath>


// *********************************
// FIXME: move it to a valid BV windowed version of engine and wrap with a macro
namespace bv
{

class TexturePixelShaderChannelPD
{
public:

    static const std::string            alphaParamName;
    static const std::string            txMatrix0ParamName;
    static const std::string            txMatrix1ParamName;

};

// ***************************** DESCRIPTOR ********************************** 

const std::string fontFile = "../dep/Media/fonts/ARIALUNI.TTF";

// ***************************************
//
FloatInterpolator    TestParamFactory::ConstantValue( float val )
{
    FloatInterpolator ret;
    ret.AddKey( 0.0f, val );

    return ret;
}

namespace
{
    FloatInterpolator ConstValue( float val )
    {
        return TestParamFactory::ConstantValue( val );
    }
}

namespace {
    float AddRotKeys( bv::Vec3Interpolator * it, float startAngle, double endAngle, float speed, float & t )
    {
        float alpha = startAngle;
        for(; alpha <= (float) endAngle ; alpha += speed )
        {
            float z = 2.5f * cosf(alpha);
            float x = 2.5f * sinf(alpha);
            int dev = rand();
            it->AddKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
            t += 0.1f;
        }

        it->AddKey(t, glm::vec3(1.f, 1.f, 1.f));

        return alpha;
    }
}

// ***************************************
//    
void TestParametersFactory::CameraAnimation_ver1( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up )
{
    //position->SetWrapMethod(WrapMethod::repeat, WrapMethod::repeat);
    position->SetWrapPreMethod( WrapMethod::repeat );
    position->SetWrapPostMethod( WrapMethod::repeat );

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;

    float alpha = AddRotKeys( position, 0.f, dt, 0.05f, t );
    alpha = AddRotKeys( position, alpha, 3. * dt, 0.5f, t );
    alpha = AddRotKeys( position, alpha, 5. * dt, 0.05f, t );
    alpha = AddRotKeys( position, alpha, 7. * dt, 0.5f, t );
    alpha = AddRotKeys( position, alpha, 8. * dt, 0.01f, t );
    alpha = AddRotKeys( position, alpha, endTime, 0.2f, t );
    alpha = AddRotKeys( position, 0.f, 2. * M_PI, 0.05f, t );
 
    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->AddKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}

    direction->AddKey(0.f, glm::vec3(0.f, 0.f, 0.f));
    up->AddKey(0.f, glm::vec3(0.f, 1.f, 0.f));

    //m_modelScene->AddCameraInterpolators(direction, position, up);
}

// ***************************************
//    
void TestParametersFactory::CameraAnimation_ver2( Vec3Interpolator * direction, Vec3Interpolator * position, Vec3Interpolator * up )
{
    //position->SetWrapMethod(WrapMethod::repeat, WrapMethod::pingPong);
    position->SetWrapPreMethod( WrapMethod::repeat );
    position->SetWrapPostMethod( WrapMethod::pingPong );

    /*
    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;
    */
    position->AddKey(0.f, glm::vec3(-2.f, 0.1f, 3.f));
    position->AddKey(20.f, glm::vec3(2.f, 0.1f, 3.f));
    

    //float alpha = 0.f;
    //for(float alpha = 0.f; alpha <= 2 * M_PI ; alpha += 0.05f)
    //{
    //    float z = 2 * cosf(alpha);
    //    float x = 2 * sinf(alpha);
    //    int dev = rand();
    //    position->AddKey(t, glm::vec3(x, float(dev) / float(RAND_MAX), z));
    //    t += 0.1f;
    //}

    direction->AddKey(0.f, glm::vec3(0.f, 0.f, 0.f));

    up->AddKey(0.f, glm::vec3(0.f, 1.f, 0.f));
    up->AddKey(3.f, glm::vec3(0.f, 1.f, 0.f));

    //m_modelScene->AddCameraInterpolators(direction, position, up);
}

void RenderMockScene( bv::model::BasicNode * tree, std::ostream & out )
{
    { tree; } // FIXME: suppress unused warning
    { out; } // FIXME: suppress unused warning
    //static float t = 0.f;

    //tree->Update(t);

    //out << "---------------------------------TIME : " << t << " ---------------------------";

    //out << std::endl;

    //tree->Print(out);

    //out << "----------END TREE------------" << std::endl << std::endl;

    //t += 0.1f;
}

model::BasicNodePtr SceneExamples::BuildMockScene( bv::model::BasicNodePtr parent )
{
    return nullptr;
}

model::BasicNodePtr SceneExamples::BuildMockScene2( bv::model::BasicNodePtr parent )
{
    return nullptr;
}
} //bv
