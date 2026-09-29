#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStatePark : public VcuState
{
  public:
    void onEnter() override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
};

} // namespace App::Logic