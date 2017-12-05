import os
import sys
import fnmatch
import subprocess
import shutil


def EnsureDir(file_path):
    directory = os.path.dirname(file_path)
    if not os.path.exists(directory):
        os.makedirs(directory)


def RemoveFirstLines( filePath, numlines ):
    
    with open( filePath, 'r' ) as fin:
        data = fin.read().splitlines(True)
    with open( filePath, 'w' ) as fout:
        fout.writelines( data[ numlines: ] )
        
        
def CleanDirectory( directory ):

    print "Clean directory: [" + directory + "]"
    shutil.rmtree( directory )
        
        

def ListBenchmarks( directory ):

    benchmarkFiles = []
    for root, dirnames, filenames in os.walk( directory ):
        for filename in fnmatch.filter(filenames, '*.exe'):
            benchmarkFiles.append(os.path.join(root, filename))
    
    return benchmarkFiles


def InvokeSingleBenchamark( file, resultDir ):
    
    print "\n"
    print "Execute benchmark: [" + file + "]"
    print "=================================================================\n"
    
    fileName = os.path.basename( file )
    fileWithoutExt = os.path.splitext( fileName )[0]
    
    resultFile = os.path.join( resultDir, fileWithoutExt + ".csv" )
    
    args = file + " --benchmark_out_format=csv --benchmark_out=" + resultFile
    subprocess.call( args )
    
    # Benchmarks should prepare csv file, but it leaves some processor information on the beginning.
    # We must remove it first.
    RemoveFirstLines( resultFile, 7 )
    
    print "================================================================="
    print "Report placed in " + resultFile
    print "\n"
    
def InvokeBenchmarks( directory, resultDir ):
    
    EnsureDir( resultDir )
    
    print "Looking for benchmark executables in [" + directory + "]\n"
    
    benchList = ListBenchmarks( directory )
    
    print "Benchmarks found: "
    for benchFile in benchList:
        print benchFile
    
    for bench in benchList:
        InvokeSingleBenchamark( bench, resultDir )

        
def ProgramHeader():
    
    print "\n================================================================="
    print "Automatic Performance Tests"
    print "This program will call all benchmarks in BlackVision directories."
    print "=================================================================\n"
        
    
def RunBenchmarks():
    
    ProgramHeader()
    
    if len( sys.argv ) < 5:
        print "Invalid number of arguments"
    else:
        
        arch = sys.argv[ 1 ]
        configuration = sys.argv[ 2 ]
        toolset = sys.argv[ 3 ]
        outputDir = sys.argv[ 4 ]
        
        CleanDirectory( outputDir )
        
        benchmarksExecs = "_Builds/" + arch + "-" + toolset + "-" + configuration + "/Benchmarks/"
    
        InvokeBenchmarks( benchmarksExecs, outputDir )

        
RunBenchmarks()