#pragma once
#include "VcuComms.h"
#include "VcuStruct.hpp"

#include <stdint.h>
#include <stdio.h>

namespace App::Infrastructure {

class DashTransmitter
{
  public:
    DashTransmitter() = default;
    void process(App::Logic::VcuStateId currentState, const App::Logic::VcuInputs& inputs,
                 const App::Logic::VcuOutputs& outputs);

  private:
    uint32_t m_lastTxMs = 0;

    const uint32_t INTERVAL_MS = 50;
};

} // namespace App::Infrastructure