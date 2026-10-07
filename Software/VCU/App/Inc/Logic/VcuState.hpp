#pragma once
#include "ILogger.hpp"
#include "VcuStruct.hpp"

namespace App::Logic {

class VcuState
{
  public:
    VcuState(ILogger& logger) : m_logger(logger) {}
    virtual ~VcuState() {}

    virtual void onEnter(const VcuInputs& inputs) {}
    virtual void onExit() {}

    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) = 0;

  protected:
    ILogger& m_logger;
};

} // namespace App::Logic