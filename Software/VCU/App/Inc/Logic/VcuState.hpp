#pragma once
#include "ILogger.hpp"
#include "VcuStruct.hpp"

namespace App::Logic {

class VcuController;

class VcuState
{
  public:
    VcuState(VcuController* context, ILogger& logger) : m_logger(logger), m_context(context) {}
    virtual ~VcuState() {}

    virtual void onEnter(const VcuInputs& inputs) {}
    virtual void onExit() {}

    virtual VcuStateId update(const VcuInputs& inputs, VcuOutputs& outputs) = 0;
    virtual VcuStateId getId() = 0;

  protected:
    ILogger& m_logger;
    VcuController* m_context;
};

} // namespace App::Logic