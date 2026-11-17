#pragma once
#include "VcuComms.h"
#include "VcuStruct.hpp"

namespace App::Infrastructure {

class DashTransmitter
{
  public:
    DashTransmitter() = default;
    void process(const App::Logic::VcuStatus& status, const App::Logic::VcuInputs& inputs,
                 const App::Logic::VcuOutputs& outputs);

  private:
    uint32_t m_lastVcuTxMs = 0;
    uint32_t m_lastBmsTxMs = 0;

    void sendVcu(const App::Logic::VcuStatus& status, const App::Logic::VcuInputs& inputs,
                 const App::Logic::VcuOutputs& outputs);
    void sendBms(const App::Logic::VcuInputs& inputs);

    const uint32_t INTERVAL_BMS_MS = 200;
    const uint32_t INTERVAL_VCU_MS = 50;
};

} // namespace App::Infrastructure