#include "PixelShader.h"

namespace bv {

// ********************************
//
PixelShader::PixelShader( const std::string& programSource )
    : Shader( programSource )
{
}

}

//
//#include <fstream>
//using std::ifstream;
//using std::ios;
//
//#include <sstream>
//using std::ostringstream;
//
//#include <sys/stat.h>
//
//namespace bv {
//
//PixelShader::PixelShader(const std::string& programName, int inputNum, int outputNum)
//    : Shader(programName, inputNum, outputNum)
//{}
//
//PixelShader* PixelShader::create(const std::string& programFile, const std::string& programName, int inputNum, int outputNum)
//{
//    if( ! fileExists(programFile) )
//    {
//        return nullptr;
//    }
//
//    ifstream inFile( programFile, ios::in );
//    if( !inFile ) {
//        return false;
//    }
//
//    ostringstream code;
//    while( inFile.good() )
//    {
//        int c = inFile.get();
//        if( ! inFile.eof() )
//            code << (char) c;
//    }
//
//    inFile.close();
//
//    PixelShader* ret = new PixelShader(programName, inputNum, outputNum);
//
//    ret->programSource = code.str();
//
//    return ret;
//}
//
//} // bv