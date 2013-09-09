#include "MockScenes.h"

#include "Engine/Models/Plugins/Geometry/PluginGeometryRing.h"
#include "Engine/Models/Plugins/Geometry/PluginGeometryRect.h"
#include "Engine\Models\Plugins\Extrude.h"
#include "Engine/Models/Plugins/Solid.h"
#include "Engine\Models\BasicNode.h"
#include "Mathematics\Transform\MatTransform.h"
#include "Engine\Models\ModelFactory.h"
#include "Engine\Models\Plugins\Transform\PluginTransformSimple.h"

namespace bv
{

bv::BasicNode* CreateRect(bv::BasicNode* parent)
{
	bv::BasicNode* rectNode = new bv::BasicNode();
	bv::Vec4Interpolator color;
	color.addKey(0, glm::vec4(1,0,0,1));
	color.addKey(10, glm::vec4(0,1,0,1));
	color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);
	bv::SolidColorPlugin* solidRect = new bv::SolidColorPlugin(color);
	rectNode->setPixelShaderPlugin(solidRect);

	//extrude
	bv::FloatInterpolator si;
    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    si.addKey(0.f, 0.1f);
    si.addKey(1.f, 2.0f);
    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
	rectNode->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));
	
	rectNode->addGeometryPlugin(new bv::PluginGeometryRect(2.f,1.f));

	// b edzie sie rozszerzal

	bv::FloatInterpolator identity; 
	identity.addKey(0.f, 1.f);

	bv::FloatInterpolator scaleX; scaleX.setWrapPostMethod(bv::WrapMethod::pingPong);
    scaleX.addKey(0.f, 1.f);
    scaleX.addKey(5.f, 1.2f);
    
	bv::TransformF* trans = new bv::TransformF();
	trans->addScale(scaleX,identity,identity);
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

	rectNode->addTransformPlugin(transPlugin);
	if(parent != nullptr)
    {
        parent->addChild(rectNode);
    }
	return rectNode;
}

void createMyRing(bv::BasicNode* parent,float offsetX,float offsetY,float startAngle,float endAngle)
{
	bv::BasicNode* mockTree = new bv::BasicNode();

	


    // SOLID
    bv::Vec4Interpolator color;

    float rs = 10.f;

    color.addKey(rs * .25f, glm::vec4(1,0.4,0.5,1));
    color.addKey(rs *.5f, glm::vec4(0.3,0.4,0.5,1));
    color.addKey(rs *.75f, glm::vec4(0.1,1.0,0.8,1));
    color.addKey(rs *.95f, glm::vec4(0.7,0.2,0.4,1));
    color.addKey(rs *1.15f, glm::vec4(0.4,0.1,1.,1));
    color.addKey(rs *1.33f, glm::vec4(1,1.0,0.1,1));
    color.addKey(rs *1.6f, glm::vec4(1,0.4,0.2,1));
    color.addKey(rs *1.9f, glm::vec4(0,0.4,1.0,1));

    color.setWrapMethod(bv::WrapMethod::clamp, bv::WrapMethod::pingPong);

    bv::SolidColorPlugin* solid1 = new bv::SolidColorPlugin(color);

    mockTree->setPixelShaderPlugin(solid1);

    bv::FloatInterpolator si;

    si.setWrapMethod(bv::WrapMethod::repeat, bv::WrapMethod::pingPong);
    float s = 5.0f;
    si.addKey(0.f, 0.5f);si.addKey(0.4f, 0.5f);si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);
    si.addKey(0.f, 0.1f);
    //si.addKey(2.4f, 2.5f);//si.addKey(0.5f, s * 0.6f);si.addKey(0.65f, 0.4f);si.addKey(0.8f, s * 0.9f);si.addKey(0.95f, 0.35f);si.addKey(1.05f, 0.5f);

	mockTree->setGeometryShaderPlugin(new bv::ExtrudePlugin(si));

	FloatInterpolator radIn;
	FloatInterpolator radOut;
	FloatInterpolator start;
	FloatInterpolator end;

	radIn.addKey(0.f, 1.f);
	radOut.addKey(0.f, 2.f);
	start.addKey(0.f, startAngle);
	end.addKey(0.f, endAngle);
	   mockTree->addGeometryPlugin(new bv::PluginGeometryRing(radIn, radOut, start, end, 3));
	//mockTree->addGeometryPlugin(new bv::PluginGeometryRect());

    // TRANSFORMATION
    bv::FloatInterpolator angle; angle.setWrapPostMethod(bv::WrapMethod::pingPong);
    bv::FloatInterpolator x;
    bv::FloatInterpolator y;
    bv::FloatInterpolator z;

    x.addKey(0.f, 0.f);
    y.addKey(0.f, 0.f);
    z.addKey(0.f, -1.1f);
    angle.addKey(0.f, 0.f);
    //angle.addKey(5.f, 180.f);

    bv::TransformF* trans = new bv::TransformF();
    
	bv::FloatInterpolator transX;
	transX.addKey(0.f,offsetX);

	bv::FloatInterpolator transY;
	transY.addKey(0.f,offsetY);
	
	bv::FloatInterpolator transZ;
	transZ.addKey(0.f,0.1f);

	trans->addTranslation(transX,transY,transZ);

	

	trans->addTransform(new bv::RotationF(angle, x,y,z));
    bv::PluginTransformSimple* transPlugin = bv::ModelFactory::CreatePluginTransformSimple(*trans);

    mockTree->addTransformPlugin(transPlugin);


	

    if(parent != nullptr)
    {
        parent->addChild(mockTree);
    }
}

bv::BasicNode* SceneExamples::CreateSceneMock3(bv::BasicNode* parent)
{
	bv::BasicNode* rect=CreateRect(parent);

	float Angle1=3.1415f;
	float Angle2=3.1415f/2.0;
	float Angle3=0;
	float Angle4=-3.1415f/2;
	float offset=-3.1415f/2;

	createMyRing(rect,-1,0.5,Angle1,Angle1+offset);	
	createMyRing(rect,1,0.5,Angle2,Angle2+offset);
	
	createMyRing(rect,1,-0.5,Angle3, Angle3+offset);	
	createMyRing(rect,-1,-0.5,Angle4,Angle4+offset);
	

    return rect;


}

}