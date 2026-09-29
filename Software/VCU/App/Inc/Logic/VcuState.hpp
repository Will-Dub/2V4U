#pragma once
#include "VcuStruct.hpp"

namespace App::Logic {

class VcuState
{
  public:
    virtual ~VcuState() {}

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) = 0;
};

} // namespace App::Logic