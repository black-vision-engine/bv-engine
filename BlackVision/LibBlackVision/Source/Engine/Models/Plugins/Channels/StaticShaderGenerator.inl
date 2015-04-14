#pragma once

namespace bv { namespace model {

    //const unsigned int GNumLists = 19;
    //const unsigned int GMaxNumEntries = 5;

    //@see: https://docs.google.com/spreadsheets/d/1K3dblNOAijnKdJXeha9_DWlSLhOc1Wcq5F1Mwlvk7x4
    char * acceptedPluginLists[ GNumLists ][ GMaxNumEntries ] = { 
                                                                    { "DEFAULT_COLOR",  "",                 "",                     "",             "" },
                                                                    { "",               "DEFAULT_TEXTURE",  "",                     "",             "" },
                                                                    { "DEFAULT_COLOR",  "",                 "",                     "DEFAULT_TEXT", "" },
                                                                    { "",               "",                 "DEFAULT_ANIMATION",    "",             "" },
                                                                    { "",               "",                 "DEFAULT_VIDEOINPUT",    "",             "" },
                                                                    { "DEFAULT_COLOR",  "",                 "",                     "",             "DEFAULT_ALPHA_MASK" },
                                                                    { "",               "DEFAULT_TEXTURE",  "",                     "",             "DEFAULT_ALPHA_MASK" },
                                                                    { "DEFAULT_COLOR",  "",                 "",                     "DEFAULT_TEXT", "DEFAULT_ALPHA_MASK" },
                                                                    { "",               "",                 "DEFAULT_ANIMATION",    "",             "DEFAULT_ALPHA_MASK" },
                                                                    { "",               "",                 "",                     "DEFAULT_TEXT", "" },
                                                                    { "",               "",                 "",                     "DEFAULT_TEXT", "DEFAULT_ALPHA_MASK" },
                                                                    { "DEFAULT_COLOR",  "",                 "",                     "DEFAULT_TIMER", "DEFAULT_ALPHA_MASK" },
                                                                    { "",               "",                 "",                     "DEFAULT_TIMER", "" },
                                                                    { "",               "",                 "",                     "DEFAULT_TIMER", "DEFAULT_ALPHA_MASK" },
                                                                    { "",               "",                 "",                     "",              "DEFAULT_HEIGHT_MAP" },
                                                                    { "DEFAULT_LINEAR_GRADIENT",  "",                 "",                     "",             "" },
                                                                    { "",  "",                 "",                     "DEFAULT_TEXT",             "DEFAULT_LINEAR_GRADIENT" },
                                                                    { "DEFAULT_LINEAR_GRADIENT",  "",                 "",                     "DEFAULT_TIMER",             "" },
                                                                    { "",  "",                 "",                     "DEFAULT_TIMER",             "DEFAULT_LINEAR_GRADIENT" },

                                                                    { "",               "DEFAULT_NO_PERSPECTIVE_TEXTURE",  "",		"",             "" },
                                                                    { "",               "DEFAULT_NO_PERSPECTIVE_TEXTURE",  "",		"",             "DEFAULT_ALPHA_MASK" },
                                                                };

    //@see: https://docs.google.com/spreadsheets/d/1K3dblNOAijnKdJXeha9_DWlSLhOc1Wcq5F1Mwlvk7x4
    char * baseShaderFileNames[ GNumLists ] = {
                                        "col",
                                        "tx",
                                        "col_txt",
                                        "tx",
                                        "tx",
                                        "col_am",
                                        "tx_am",
                                        "col_txt_am",
                                        "tx_am",
                                        "col_txt",
                                        "col_txt_am",
                                        "col_txt_am",
                                        "col_txt",
                                        "col_txt_am",
                                        "hm",
                                        "lg",
                                        "txt_lg",
                                        "txt_lg",
                                        "txt_lg",

										"nptx",
										"nptx_am"
                                    };

} //model
} //bv
