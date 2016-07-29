import os
result = [os.path.join(dp, f) for dp, dn, filenames in os.walk(".") for f in filenames if os.path.splitext(f)[1] == '.cpp']


for p in result:

    with open(p, "r") as f:
        data = f.read()
        
        idx = data.find( "namespace bv" )
        preNamespace = data[ :idx ]
        postNamespace = data[ idx: ]
        
    with open( p, "w" ) as f:
        f.write( preNamespace + "\n\n#include \"Memory/MemoryLeaks.h\"\n\n\n\n" + postNamespace );
        