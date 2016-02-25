
import os
result = [os.path.join(dp, f) for dp, dn, filenames in os.walk(".") for f in filenames if os.path.splitext(f)[1] == '.cpp']


for p in result:

    with open(p, "r") as f:
        data = f.read()
    
    with open( p, "w" ) as f:
        f.write( "#include \"stdafx.h\"\n\n" + data );
        