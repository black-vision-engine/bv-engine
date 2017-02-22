﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CommandLine;
using CommandLine.Text;



namespace AutomaticTester
{
    public class Options
    {
        [Option( 'i', "input", Required = true, HelpText = "Input test file." )]
        public string       InputTestsDir { get; set; }

        [Option( 'e', "exec", Required = true, HelpText = "BlackVision executable file." )]
        public string       BVExecPath { get; set; }

        [Option( 'v', null, HelpText = "Print details during execution." )]
        public bool Verbose { get; set; }

    }
}
