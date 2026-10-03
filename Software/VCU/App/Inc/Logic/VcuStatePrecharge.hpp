#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStatePrecharge: public VcuState
{
  public:
    void onEnter() override;
    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) override;
  private:
    uint32_t m_prechargeStartTimeMs;
};

} // namespace App::Logic