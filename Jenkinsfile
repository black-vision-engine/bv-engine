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
  // slackSend (color: colorCode, message: summary)
}

def get_tests_dir( buildDir, conf, platform ) {
     return buildDir + platform + '-v110-' + conf + '\\Tests\\'
}

def get_app_dir( buildDir, conf, platform ) {
     return buildDir + platform + '-v110-' + conf + '\\Applications\\'
}

def get_auto_tester_path( buildDir, conf, platform ) {
     return buildDir + platform + '-v110-' + conf + '\\DevTools\\AutomaticTester\\AutomaticTester.exe'
}

def make_auto_tests( buildDir, conf, platform, outputDir ) {
    def autoTesterExec = get_auto_tester_path( buildDir, conf, platform )
    
    createDir( outputDir )
    
    def HARDCODED_TEST_PATH = 'F:\\GoogleDrive\\bv_data\\Regression'
    def HARDCODED_CONFIG_PATH = 'F:\\GoogleDrive\\bv_data\\bv_media\\configs\\auto_test_config.xml'
    
    copyFile( HARDCODED_CONFIG_PATH, get_app_dir( buildDir, conf, platform ) + 'BlackVision\\config.xml' )
    
    bat autoTesterExec + ' -i ' + HARDCODED_TEST_PATH + ' -e ' + get_app_dir(  buildDir, conf, platform ) + 'BlackVision\\' + 'BlackVision.exe' + ' -v ' + '--output=' + outputDir
}

def make_build( conf, platform ) {
    def info = conf + '|' + platform
    echo 'Building ' + info
	bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=\"" + conf + "\" /maxcpucount:4 /p:Platform=\"" + platform + "\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
	echo 'Building ' + info + ' FINISHED'
}

def list_test_execs( buildDir, conf, platform ) {
    //def g =  get_tests_dir( buildDir, conf, platform ) + '**/*.exe'
	//def testExecs = findFiles(glob: g )
	//return "list of exec paths"
	
	def testDir = get_tests_dir( buildDir, conf, platform )
	
	return [	testDir + "TestTestFramework\\TestTestFramework.exe"	]
}

def make_archive( buildDir, conf, platform, fEnabled ) {
    def d = buildDir + conf + '-v110-' + platform
    def includes_app = get_app_dir( buildDir, conf, platform ) + '/**'
    def includes_tests = get_tests_dir( buildDir, conf, platform ) + '/**'
    archiveArtifacts artifacts: includes_app, fingerprint: fEnabled
    archiveArtifacts artifacts: includes_tests, fingerprint: fEnabled
}

def generate_tests_report( testResPath ) {
    step([$class: 'XUnitBuilder',
    thresholds: [[$class: 'FailedThreshold', unstableThreshold: '1']],
    tools: [[$class: 'GoogleTestType', pattern: testResPath + '/**']]])
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

node {
    checkout scm
    
    def buildDir = 'BlackVision\\_Builds\\'
    def tempDir = 'BlackVision\\_Temp\\'
    
    def testResPath = 'test_reports'
    
    def configurations = ['Debug', 'Release']
    def platforms = ['Win32', 'x64']
    
    def currentConfiguration = configurations[0]
    def currentPlatform = platforms[1]
    
    //stage('Clean') {
    //    removeDir( buildDir )
    //    removeDir( tempDir )
    //    removeDir( testResPath )
    //   removeDir( 'generatedJUnitFiles' )
    //    removeDir( 'DefaultPMDir' )
    //}
     stage('Build') {
        try {
            notifyBuild('STARTED', 'Build')
 	        make_build( currentConfiguration, currentPlatform )
        } catch( e ){
            currentBuild.result = "FAILED"
            throw e
        }
        finally {
            notifyBuild(currentBuild.result, 'Build')
        }
     }
  	stage('Archive') {
  	    
  	    try {
            notifyBuild('STARTED', 'Archive')
 	        make_archive( buildDir, currentConfiguration, currentPlatform, true )
        } catch( e ){
            currentBuild.result = "FAILED"
            throw e
        }
        finally {
            notifyBuild(currentBuild.result, 'Archive')
        }
  	}
	stage('Open BV') {
		def bvExecutablePath = buildDir + currentPlatform + "-v110-" + currentConfiguration + '\\Applications\\BlackVision\\BlackVision.exe'
		bat bvExecutablePath
	}
	
    stage('Test') {


  	    try {
            notifyBuild('STARTED', 'Test')
 	        //def testExecsList = list_test_execs( buildDir, currentConfiguration, currentPlatform )
		
     		//echo testExecsList.size() + ' tests found.'
    		
     		//for( int i = 0; i < testExecsList.size(); ++i ) {
     		//    try {
     		//        bat testExecsList.get( i ) + ' -o ' + testResPath + '/TestFrameworkTest.xml -FileLog Logi/DebugLog.txt debug - DisableDefaultLog'
     		//    }
     		//    catch(err) {
     		//        echo "test fail."
     		//    }
     		//}
			
			bat 'BlackVision/RunAllTests.bat ' + currentPlatform + ' ' + currentConfiguration + ' v110 ' + testResPath + '/'
    		
    		//make_auto_tests( buildDir, currentConfiguration, currentPlatform, testResPath + '\\auto_tests' )
    		
     	    generate_tests_report( testResPath	)
        } catch( e ){
            currentBuild.result = "FAILED"
            throw e
        }
        finally {
            notifyBuild(currentBuild.result, 'Test')
        }

    }
}