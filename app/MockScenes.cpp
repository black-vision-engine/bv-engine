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
    position->SetWrapMethod(WrapMethod::repeat, WrapMethod::repeat);

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
    position->SetWrapMethod(WrapMethod::repeat, WrapMethod::pingPong);

    float t = 0.f;
    double endTime = 2. * M_PI;
    double dt = endTime / 10.f;
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

void RenderMockScene(bv::model::BasicNode* tree, std::ostream& out)
{
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
    assert( false );
    bv::model::BasicNodePtr mockTree = std::make_shared< bv::model::BasicNode >( "MockRoot", nullptr );

    // SOLID
    bv::Vec4Interpolator color;

    float rs = 10.f;

    color.AddKey(rs * .25f, glm::vec4(0,0,1,0.7f));
    //color.AddKey(rs *.5f, glm::vec4(0.3,0.4,0.5,1));
    //color.AddKey(rs *.75f, glm::vec4(0.1,1.0,0.8,0));
    //color.AddKey(rs *.95f, glm::vec4(0.7,0.2,0.4,1));
    //color.AddKey(rs *1.15f, glm::vec4(0.4,0.1,1.,0));
    //color.AddKey(rs *1.33f, glm::vec4(1,1.0,0.1,1));
    //color.AddKey(rs *1.6f, glm::vec4(1,0.4,0.2,0));
    //color.AddKey(rs *1.9f, glm::vec4(0,0.4,1.0,1));

    color.SetWrapMethod( bv::WrapMethod::clamp, bv::WrapMethod::pingPong );

//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

  //  mockTree->setPixelShaderPlugin(solid1);

    //bv::FloatInterpolator si;

    //si.SetWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    //float s = 5.0f;
    ////si.AddKey(0.f, 0.5f);si.AddKey(0.4f, 0.5f);si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
    //si.AddKey(0.f, 0.1f);
    //si.AddKey(2.4f, 2.5f);//si.AddKey(0.5f, s * 0.6f);si.AddKey(0.65f, 0.4f);si.AddKey(0.8f, s * 0.9f);si.AddKey(0.95f, 0.35f);si.AddKey(1.05f, 0.5f);
    
    //mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));

//    mockTree->addGeometryPlugin(new bv::PluginGeometryRect(4.f, 4.f));

    // TRANSFORMATION
    bv::FloatInterpolator angle; angle.SetWrapPostMethod ( bv::WrapMethod::pingPong );
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    x.AddKey( 0.f, 0.f );
    y.AddKey( 0.f, 0.f );
    z.AddKey( 0.f, 1.f );
    angle.AddKey( 0.f, 0.f );
    angle.AddKey( 5.f, 180.f );

    bv::TransformF trans;
    trans.AddTransform( new bv::RotationF( angle, x,y,z ) );
//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    //FIXME:
    //mockTree->addTransformPlugin(transPlugin);

    if( parent != nullptr )
    {
        parent->AddChild( mockTree );
    }

    return mockTree;
}

model::BasicNodePtr SceneExamples::BuildMockScene2( bv::model::BasicNodePtr parent )
{
    assert( false );
    bv::model::BasicNodePtr mockTree = std::make_shared< bv::model::BasicNode >( "Mock2Root", nullptr );

    // SOLID
    bv::Vec4Interpolator color;

    color.AddKey(.0f, glm::vec4(1,0,0,0.5f));
    //color.AddKey(3.f, glm::vec4(0.3,0.4,0.5,1.f));
    //color.AddKey(1.f, glm::vec4(0.1,0.2,0.8,1.f));

    //color.AddKey(2.f, glm::vec4(1,0.4,0.5,1.f));

    color.SetWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

//    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

//    mockTree->setPixelShaderPlugin(solid1);

    //bv::FloatInterpolator scaleInt;

    //scaleInt.SetWrapMethod(bv::WrapMethod::pingPong, bv::WrapMethod::pingPong);

    //scaleInt.AddKey(0.f, -3.5f);
    //scaleInt.AddKey(-1.30f, 3.5f);

    //mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(scaleInt));

//    mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    float s = 2.0f;
    float l = 1.4f;
    x.AddKey(0.f, 0.5f);x.AddKey(0.4f, 0.5f);x.AddKey(0.5f, s * 0.6f);x.AddKey(0.65f, 0.4f);x.AddKey(0.8f, s * 0.9f);x.AddKey(0.95f, 0.35f);x.AddKey(1.05f, 0.5f);
    y.AddKey(0.f, 0.5f);y.AddKey(0.4f, 0.5f);y.AddKey(0.5f, l * 0.6f);y.AddKey(0.65f, 0.4f);y.AddKey(0.8f, l * 0.9f);y.AddKey(0.95f, 0.35f);y.AddKey(1.05f, 0.5f);
    z.AddKey(0.f, 1.f);

    x.SetWrapPostMethod(bv::WrapMethod::repeat);
    y.SetWrapPostMethod(bv::WrapMethod::repeat);
    
    bv::TransformF * trans = new bv::TransformF();

    bv::FloatInterpolator mx;
    bv::FloatInterpolator my;
    bv::FloatInterpolator mz;

    mx.AddKey(0.f, 0.0f);
    my.AddKey(0.f, 0.0f);
    mz.AddKey(0.f, 0.0f);

    mx.AddKey(1.f, 1.0f);
    my.AddKey(2.f, -2.0f);
    mz.AddKey(3.f, 0.f);

    trans->AddTranslation(mx, my, mz);
    trans->AddScale(x,y,z);

//    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    //FIXME:
    //mockTree->addTransformPlugin(transPlugin);

    if(parent != nullptr)
    {
        parent->AddChild(mockTree);
    }

    return mockTree;
}
} //bv
