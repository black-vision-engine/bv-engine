#pragma once

#include "CoreDEF.h"

#include "Engine/Interfaces/IValue.h"


namespace bv {

class GenericShaderParam;

void            UpdateGenericShaderParam    ( IValueConstPtr source, GenericShaderParam * dest );

} // bv
