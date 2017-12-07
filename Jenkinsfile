#!groovy

def notifyBuild(String buildStatus = 'STARTED', stageName = "") {
  // build status of null means successfulnotifySuccessful
  buildStatus =  buildStatus ?: 'SUCCESSFUL'

  // Default values
  def colorName = 'RED'
  def colorCode = '#FF0000'
  def subject = "${stageName}:   ${buildStatus}: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]'"
  def summary = "${subject} (${env.BUILD_URL})"
  def details = """<p>STARTED: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]':</p>
    <p>Check console output at &QUOT;<a href='${env.BUILD_URL}'>${env.JOB_NAME} [${env.BUILD_NUMBER}]</a>&QUOT;</p>"""

  // Override default values based on build status
  if (buildStatus == 'STARTED') {
    color = 'YELLOW'
    colorCode = '#FFFF00'
  } else if (buildStatus == 'SUCCESSFUL') {
    color = 'GREEN'
    colorCode = '#00FF00'
  } else {
    color = 'RED'
    colorCode = '#FF0000'
  }

  // Send notifications
  if( env.JOB_NAME == "BlackVision-master" )
  {
	slackSend (color: colorCode, message: summary)
  }
  
  step([$class: 'Mailer', notifyEveryUnstableBuild: true, recipients: emailextrecipients([[$class: 'CulpritsRecipientProvider'], [$class: 'RequesterRecipientProvider']])])
}

def get_build_dir( buildDir, conf, platform )
{
    return buildDir + platform + '-v140-' + conf
}

def get_tests_dir( buildDir, conf, platform )
{
     return get_build_dir( buildDir, conf, platform ) + '\\Tests\\'
}

def get_app_dir( buildDir, conf, platform )
{
     return get_build_dir( buildDir, conf, platform ) + '\\Applications\\'
}


def make_build( conf, platform ) {
    def info = conf + '|' + platform
    echo 'Building ' + info
	bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS14\\BlackVision.sln /p:Configuration=\"" + conf + "\" /maxcpucount:4 /p:Platform=\"" + platform + "\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
	echo 'Building ' + info + ' FINISHED'
}

def list_test_execs( buildDir, conf, platform ) {

	def testDir = get_tests_dir( buildDir, conf, platform )
	
	return [	testDir + "TestTestFramework\\TestTestFramework.exe"	]
}

def make_archive( buildDir, conf, platform, fEnabled ) {
    def d = buildDir + conf + '-v140-' + platform
    def includes_app = get_app_dir( buildDir, conf, platform ) + '/**'
    def includes_tests = get_tests_dir( buildDir, conf, platform ) + '/**'
    archiveArtifacts artifacts: includes_app, fingerprint: fEnabled
    archiveArtifacts artifacts: includes_tests, fingerprint: fEnabled
}

def generate_tests_report( testResPath ) {
    step([$class: 'XUnitBuilder',
    thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
    tools: [[$class: 'GoogleTestType', pattern: testResPath + '/*.xml']]])
}

def copyFile( src, dest ) {
    bat 'copy /Y ' + src + ' ' + dest
}

def createDir( path ) {
    bat 'mkdir ' + path
}

def removeDir( path ) {
    if( fileExists( path ) ) {
        dir( path ) {
            deleteDir()
        }
    }
}

def generateBuildNumber()
{
	bat 'BlackVision/GenBuildVersion.bat ' + "${env.BUILD_NUMBER}"
}

def generateDoxygenDocs( buildDir, conf, platform )
{
    withCredentials([[$class: 'UsernamePasswordMultiBinding', credentialsId: '7d30c872-4901-4bfd-bddc-50de3a25dd94',
                    usernameVariable: 'USERNAME', passwordVariable: 'PASSWORD']])
    {
        bat "\"${tool 'TortoiseHg'}\\hg.exe\" --debug --config auth.jenkins.prefix=* --config auth.jenkins.username=${env.USERNAME} --config auth.jenkins.password=${env.PASSWORD} --config \"auth.jenkins.schemes=https\" clone https://nieznanysprawiciel@bitbucket.org/blackvision/bv_engine/wiki"
    }
    
    dir( "wiki" )
    {
        bat "\"${tool 'doxygen'}\" ../Doxyfile"
    }
    
    publishHTML([allowMissing: true, alwaysLinkToLastBuild: true, keepAll: true, reportDir: 'BlackVision/Doc/html/', reportFiles: 'index.html', reportName: 'BlackVision Documentation', reportTitles: ''])
}

def generatePerformancePlots( buildDir, conf, platform )
{
    def plotFiles = findFiles( glob: 'BlackVision/Reports/Benchmarks/*.csv' )

    for (i = 0; i < plotFiles.size(); i++)
    {
        def file = plotFiles[ i ]
        plot csvFileName: "${file.name}", csvSeries: [[displayTableFlag: true, exclusionValues: 'meanTime,medianTime,meanCPU,medianCPU', file: "${file}", inclusionFlag: 'INCLUDE_BY_STRING', url: '']], exclZero: false, group: 'Performance', exclZero: true, keepRecords: true, logarithmic: false, numBuilds: '', style: 'line', title: "${file.name}", useDescr: true, yaxis: '', yaxisMaximum: '', yaxisMinimum: ''
    }
}




node {
    checkout scm
    
    def buildDir = 'BlackVision\\_Builds\\'
    def tempDir = 'BlackVision\\_Temp\\'
    
    def testResPath = 'Reports/Test'
    def bechmarksResPath = 'Reports/Benchmarks'
    
    def configurations = ['Debug', 'Release']
    def platforms = ['Win32', 'x64']
    
    def currentConfiguration = configurations[1]
    def currentPlatform = platforms[1]
	

    stage('Clean')
    {
        removeDir( 'wiki' )
        removeDir( buildDir )
        removeDir( tempDir )
        removeDir( testResPath )
        removeDir( bechmarksResPath )
        removeDir( 'generatedJUnitFiles' )
        removeDir( 'DefaultPMDir' )
        generateBuildNumber()
    }

	//stage('Build Debug')
	//{
	//	try {
	//		//notifyBuild('STARTED', 'Build')
	//		make_build( configurations[0], currentPlatform )
	//	} catch( e ){
	//		currentBuild.result = "FAILED"
	//		throw e
	//	}
	//	finally {
	//		notifyBuild(currentBuild.result, 'Build')
	//	}
	//}
	
	stage('Build Release')
	{
		try
        {
			notifyBuild('STARTED', 'Build')
			make_build( configurations[1], currentPlatform )
		} catch( e ){
			currentBuild.result = "FAILED"
			throw e
		}
		finally {
			notifyBuild(currentBuild.result, 'Build')
		}
	}
	
  	stage('Archive')
	{
  	    
  	    try
        {
            notifyBuild('STARTED', 'Archive')
 	        make_archive( buildDir, currentConfiguration, currentPlatform, true )
        } catch( e )
        {
            currentBuild.result = "FAILED"
            throw e
        }
        finally {
            notifyBuild(currentBuild.result, 'Archive')
        }
  	}

	
    stage('Test')
	{
  	    try
		{
		
            notifyBuild('STARTED', 'Test')
			bat 'BlackVision/RunAllTests.bat ' + currentPlatform + ' ' + currentConfiguration + ' v140 ' + testResPath + '/'

     	    generate_tests_report( "BlackVision\\" + testResPath )
			
        } catch( e )
        {
            currentBuild.result = "FAILED"
            throw e
        }
        finally
        {
            notifyBuild(currentBuild.result, 'Test')
        }
    }
   
    
    stage( 'Generate Docs' )
    {
  	    try
		{
            generateDoxygenDocs( buildDir, currentConfiguration, currentPlatform )
			
        } catch( e )
        {
            currentBuild.result = "FAILED"
            throw e
        }
        finally
        {
            notifyBuild(currentBuild.result, 'Generate Docs')
        }
    }
    
    stage( 'Performance Tests' )
    {
      	try
		{
            bat 'BlackVision/RunBenchmarks.bat ' + currentPlatform + ' ' + currentConfiguration + ' v140 ' + bechmarksResPath + '/'
            generatePerformancePlots( buildDir, currentConfiguration, currentPlatform )
        }
        catch( e )
        {
            currentBuild.result = "FAILED"
            throw e
        }
        finally
        {
            notifyBuild(currentBuild.result, 'Performance Tests')
        }
    }
	
}