#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStateInit : public VcuState
{
  public:
    void onEnter(const VcuInputs& inputs) override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
};

} // namespace App::Logic