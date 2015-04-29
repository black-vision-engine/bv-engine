#define START_MODEL( timeEval ) ModelHelper h( timeEval ); h.CreateVacModel(); // FIXME!!!

#define END_MODEL() return h.GetModel();

#define ADD_VAC_PARAM( name, defVal ) h.AddSimpleParam( name, defVal );

#define ADD_VAC_STATED_PARAM( name, defVal ) h.AddSimpleStatedParam( name, defVal );
