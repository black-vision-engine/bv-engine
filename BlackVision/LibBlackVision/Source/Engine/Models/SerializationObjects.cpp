#include "SerializationObjects.h"

namespace bv {

void SerializeObject::Save( std::string filename )
{
    std::ofstream file( filename );
    file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
    //file << m_doc;
    file.close();
    m_doc.clear();
}

}
