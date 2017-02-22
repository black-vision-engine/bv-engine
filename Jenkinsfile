#!groovy​

pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
				checkout scm
                echo 'Building..'
				
				bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=Debug /maxcpucount:4 /p:Platform=\"x64\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
            }
        }
		stage('Archive'){
			steps {
				archive 'BlackVision/_Builds/x64-v110-Debug/Applications/BlackVision/**'
				archive 'BlackVision/_Builds/x64-v110-Debug/Applications/EnginePrototyper/**'
			}
		}
        stage('Test') {
            steps {
				branch 'jenkins'
                echo 'Testing..'
            }
        }
		
		
        stage('Deploy') {
            steps {
				branch 'jenkins'
                echo 'Deploying....'
            }
        }
    }
}
