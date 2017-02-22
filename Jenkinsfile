#!groovy​

pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
				branch 'master'
				checkout scm
                echo 'Building..'
				
				bat "\"${tool 'MSBuild'}\" BlackVision\\Projects\\Win\\VS11\\BlackVision.sln /p:Configuration=Debug /maxcpucount:4 /p:Platform=\"x64\" /p:ProductVersion=1.0.0.${env.BUILD_NUMBER}"
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
