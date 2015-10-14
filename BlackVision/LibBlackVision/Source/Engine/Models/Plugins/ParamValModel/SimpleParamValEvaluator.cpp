#include "SimpleParamValEvaluator.h"


namespace bv { namespace model {

#include "SimpleParamValEvaluator.inl"
    
} // model
} // bv


//Explicit instantiation - this way class' implementation can be stored in cpp file (like here)
template class bv::model::SimpleParamValEvaluator<bv::model::ParamIntPtr, bv::ValueIntPtr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamFloatPtr, bv::ValueFloatPtr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamVec4Ptr, bv::ValueVec4Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamVec3Ptr, bv::ValueVec3Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamVec2Ptr, bv::ValueVec2Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamMat2Ptr, bv::ValueMat2Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamTransformPtr, bv::ValueMat4Ptr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamStringPtr, bv::ValueStringPtr>;
template class bv::model::SimpleParamValEvaluator<bv::model::ParamWStringPtr, bv::ValueWStringPtr>;
