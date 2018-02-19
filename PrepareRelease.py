import os
import sys
import shutil


def ProgramHeader():
    
    print "\n================================================================="
    print "Release Preparing Script"
    print "This program will copy build artifacts, remove not necessary files"
    print "and prepare build for sending to server."
    print "=================================================================\n"
       


def EnsureDir(file_path):
    directory = os.path.dirname(file_path)
    if not os.path.exists(directory):
        print "Create directory: [" + directory + "]"
        os.makedirs(directory)

        
def CleanDirectory( directory ):

    if os.path.exists(directory):
        print "Clean directory: [" + directory + "]"
        shutil.rmtree( directory )
        
        
def CopyBuild( src, out ):

    # Copy build to output directory
    CleanDirectory( out )

    print "Copy build from: [" + src + "] to [" + out + "]."
    
    shutil.copytree( src, out )
    

def ListNotNecessaryFiles():

    list = [ "TestAssets", "TestConfigs", "cache", "Logs" ]
    return list
    
def CleanNotNecessaryFiles( appOutDir ):
    
    toRemoveList = ListNotNecessaryFiles()
    
    print "\n"
    print "## ------------------------------------------------------------ ##"
    print "## Removing not necessary files                                 ##"
    print "## ------------------------------------------------------------ ##"
    
    for element in toRemoveList:
        
        toRemovePath = os.path.join( appOutDir, element )
        
        print "Removing: " + toRemovePath
        
        shutil.rmtree( toRemovePath, True )
    
    
def CopyConfig( appOutDir ):
    
    defaultReleaseConfig = "TestConfigs/ReleaseConfig.xml"
    defaultConfigPath = os.path.join( appOutDir, defaultReleaseConfig )
    
    print "Copy default config file [" + defaultReleaseConfig + "]"
    
    shutil.copy2( defaultConfigPath, appOutDir )
    
    
def PrepareRelease():
    
    ProgramHeader()
    
    if len( sys.argv ) < 3:
        print "Invalid number of arguments"
    else:
        
        buildsDir = sys.argv[ 1 ]
        outputsDir = sys.argv[ 2 ]
        
        # Copy build to output directory
        appDir = os.path.join( buildsDir, "Applications/BlackVision/" )
        
        CopyBuild( appDir, outputsDir )
        CopyConfig( outputsDir )
        CleanNotNecessaryFiles( outputsDir )
        
        
        
    
    
    
PrepareRelease()