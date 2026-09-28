#pragma once
#include "VcuState.hpp"

namespace App::Logic {

class VcuStateFault : public VcuState {
public:
  void onEnter() override;
  virtual VcuStateId update(const VcuInputs &inputs,
                            VcuOutputs &outputs) override;

private:
  int m_blinkTimer;
  bool m_isBoardLedOn;
};

} // namespace App::Logic