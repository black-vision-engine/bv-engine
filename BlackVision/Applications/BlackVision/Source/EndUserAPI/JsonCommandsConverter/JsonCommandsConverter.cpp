#include "JsonCommandsConverter.h"


namespace bv
{

// ***********************
//
JsonCommandsConverter::JsonCommandsConverter()
{}

JsonCommandsConverter::~JsonCommandsConverter()
{}

// ***********************
//
void                JsonCommandsConverter::QueueEvent          ( std::wstring&& /*eventString*/ )
{

}

// ***********************
//
std::wstring        JsonCommandsConverter::PollEvent           ()
{
    return L"";
}



} //bv
