#pragma once
#include "VcuState.hpp"
#include "ThrottleMapper.hpp"

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

    uint32_t m_lastTickMs = 0;
    ThrottleMapper m_forwardMap{{0.04f, 1.5f, MAX_AMPERAGE, 1.5f, 4.0f}};
    ThrottleMapper m_reverseMap{{0.04f, 1.5f, MAX_AMPERAGE * 0.5f, 1.0f, 4.0f}};
};

} // namespace App::Logic