#define START_MODEL( timeEval ) ModelHelper h( timeEval );

#define END_MODEL() return h.GetModel();

#define ADD_VAC_PARAM( name, defVal ) h.AddVacSimpleParam( name, defVal );
#define ADD_PS_PARAM( name, defVal ) h.AddPsSimpleParam( name, defVal );

#define ADD_VAC_STATED_PARAM( name, defVal ) h.AddVacSimpleStatedParam( name, defVal );

#define ADD_PS_EVAL_PARAM( name, defVal ) h.AddPsSimpleParam( name, defVal, true, false );
