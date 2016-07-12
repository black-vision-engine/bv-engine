using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections.ObjectModel;
using System.Threading.Tasks;

namespace ProfilerEditor.Tester
{
    public class TestsManager
    {
        private ObservableCollection< TestFile >    m_testFiles;
        private TestFile                            m_selectedFile;
        private string                              m_testsPath;

        public TestsManager()
        {
            SelectedFile = null;
            TestFiles = new ObservableCollection<TestFile>();
        }


        public void    UpdateTestPath( string newPath )
        {
            TestFiles.Clear();
            m_testsPath = newPath;

            var files = Directory.EnumerateFiles( m_testsPath, "*.bvtest" );
            foreach( var file in files )
            {
                TestFile newFile = new TestFile();
                newFile.FileName = Path.GetFileName( file );

                TestFiles.Add( newFile );
            }

        }


        public ObservableCollection<TestFile> TestFiles
        {
            get
            {
                return m_testFiles;
            }

            set
            {
                m_testFiles = value;
            }
        }

        public TestFile SelectedFile
        {
            get
            {
                return m_selectedFile;
            }

            set
            {
                m_selectedFile = value;
            }
        }

    }
}
