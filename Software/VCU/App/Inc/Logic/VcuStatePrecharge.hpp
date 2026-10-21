#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStatePrecharge : public VcuState
{
  public:
    VcuStatePrecharge(VcuController* context, ILogger& logger) : VcuState(context, logger) {}
    void onEnter(const VcuInputs& inputs) override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
    virtual VcuStateId getId() override;

  private:
    uint32_t m_prechargeStartTimeMs;
};

} // namespace App::Logic