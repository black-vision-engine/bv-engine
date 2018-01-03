import os
import sys
import fnmatch
import subprocess
import shutil
import csv


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
        

def GenLabels():
    return [ "benchmark", "meanTime", "medianTime", "stddevTime", "meanCPU", "medianCPU", "stddevCPU", "max", "min", "Iterations" ]
    
def ToMillis( time ):
    # Engine produces values in microseconds
    return float( time ) / 1000
        
def GenSingleBenchmarkReport( rows, resultDir ):
    
    meanRow = rows[ 0 ]
    medianRow = rows[ 1 ]
    stddevRow = rows[ 2 ]
    maxRow = rows[ 3 ]
    minRow = rows[ 4 ]
    
    nameColumn = meanRow[ 0 ]
    nameParts = nameColumn.split( "/" )     # Report type consist of repeats count and mean,median,stddev string.
    
    baseName = nameParts[ 0 ]
    problemSize = nameParts[ 1 ]
    
    plotName = baseName + "_ProblemSize_" + problemSize
    fileName = os.path.join( resultDir, plotName + ".csv" )
    
    meanTime = ToMillis( meanRow[ 2 ] )
    medianTime = ToMillis( medianRow[ 2 ] )
    stddevTime = ToMillis( stddevRow[ 2 ] )
    maxTime = ToMillis( maxRow[ 2 ] )
    minTime = ToMillis( minRow[ 2 ] )
    iterations = ToMillis( meanRow[ 1 ] )
    
    meanTimeCPU = ToMillis( meanRow[ 3 ] )
    medianTimeCPU = ToMillis( medianRow[ 3 ] )
    stddevTimeCPU = ToMillis( stddevRow[ 3 ] )
    maxTimeCPU = ToMillis( maxRow[ 3 ] )
    minTimeCPU = ToMillis( minRow[ 3 ] )
        
    csvLine = [ plotName, meanTime, medianTime, stddevTime, meanTimeCPU, medianTimeCPU, stddevTimeCPU, maxTime, minTime, iterations ]
    
    with open( fileName, 'w') as file:
        csvWriter = csv.writer( file, delimiter=',' )
        
        csvWriter.writerow( GenLabels() )
        csvWriter.writerow( csvLine )
        
    print "Generated report file: [" + fileName + "]"
        
def ProcessReport( reportFile, resultDir ):
    
    print "================================================================="
    print "Generating separate report for each benchmark..."
    
    rows = []
    
    with open( reportFile, 'rb' ) as csvfile:
        reader = csv.reader( csvfile, delimiter=',', quotechar='\"' )
        for row in reader:
            rows.append( row )
    
    # Remove Labels
    del rows[ 0 ]
    
    while rows:
        
        # We want to extract mean, median and stddev, max and min.
        rowsToProcess = rows[ 0:5 ]
        
        GenSingleBenchmarkReport( rowsToProcess, resultDir )
        
        del rows[ 0:5 ]
    

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
    fileDirectory = os.path.dirname( file )
    
    tempResultDir = os.path.join( resultDir, "Temporary/" )
    resultFile = os.path.join( tempResultDir, fileWithoutExt + ".csv" )
    
    EnsureDir( resultFile )
    
    # Working directory should be the same as file directory
    scriptWorkingDir = os.getcwd()
    os.chdir( fileDirectory )
    
    args = fileName + " --benchmark_out_format=csv --benchmark_out=" + resultFile
    subprocess.call( args )
    
    os.chdir( scriptWorkingDir )
    
    # Benchmarks should prepare csv file, but it leaves some processor information on the beginning.
    # We must remove it first.
    RemoveFirstLines( resultFile, 7 )
    
    print "================================================================="
    print "Temporary report placed in [" + resultFile + "]"
    
    ProcessReport( resultFile, resultDir )
    
    print "================================================================="
    print "Removing temporary report [" + resultFile + "]"
    
    os.remove( resultFile )
    
    
def InvokeBenchmarks( directory, resultDir ):
    
    EnsureDir( resultDir )
    
    resultDir = os.path.abspath( resultDir )
    
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
        
        EnsureDir( outputDir )
        CleanDirectory( outputDir )
        
        benchmarksExecs = "_Builds/" + arch + "-" + toolset + "-" + configuration + "/Benchmarks/"
    
        InvokeBenchmarks( benchmarksExecs, outputDir )

        
RunBenchmarks()