#pragma once
#include "VcuState.hpp"

#define MAX_AMPERAGE 95.0f

namespace App::Logic {

class VcuStateDrive : public VcuState
{
  public:
    VcuStateDrive(VcuController* context, ILogger& logger) : VcuState(context, logger) {}

    void onEnter(const VcuInputs& inputs) override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
    virtual VcuStateId getId() override;

  private:
    float getTargetAmps(const VcuInputs& inputs);
};

} // namespace App::Logic