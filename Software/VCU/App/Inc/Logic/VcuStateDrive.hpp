#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStateDrive : public VcuState
{
  public:
    VcuStateDrive(VcuController* context, ILogger& logger) : VcuState(context, logger) {}

    void onEnter(const VcuInputs& inputs) override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
    virtual VcuStateId getId() override;
};

} // namespace App::Logic