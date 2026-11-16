#pragma once
#include "Calibration.hpp"
#include "ThrottleMapper.hpp"
#include "VcuState.hpp"

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
    ThrottleMapper m_forwardMap{Calibration::FORWARD_THROTTLE};
    ThrottleMapper m_reverseMap{Calibration::REVERSE_THROTTLE};
};

} // namespace App::Logic