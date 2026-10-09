#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStateFault : public VcuState
{
  public:
    VcuStateFault(ILogger& logger) : VcuState(logger) {}
    void onEnter(const VcuInputs& inputs) override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
    virtual VcuStateId getId() override;

  private:
    int m_blinkTimer;
    bool m_isBoardLedOn;
};

} // namespace App::Logic