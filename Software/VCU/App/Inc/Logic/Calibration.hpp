#pragma once

#include "ThrottleMapper.hpp"

#include <stdint.h>

namespace App::Logic::Calibration {

// Motor/VESC
constexpr float MAX_AMPS = 95.0f;
constexpr float REVERSE_AMPS_RATIO = 0.5f;
constexpr float VESC_MAX_MOTOR_TEMP_C = 100.0f;
constexpr float VESC_MAX_MOSFET_TEMP_C = 85.0f;
constexpr float VESC_MIN_VOLTAGE_V = 42.0f;
constexpr float VESC_MAX_CURRENT_A = 250.0f;

// Throttle
constexpr float THROTTLE_RELEASE_THRESHOLD = 0.05f;
constexpr float THROTTLE_DEADZONE = 0.04f;
constexpr float THROTTLE_CURVE_EXPONENT = 1.5f;
constexpr float RISE_RATE_PER_S = 1.5f;
constexpr float REVERSE_RISE_RATE_PER_S = 1.0f;
constexpr float FALL_RATE_PER_S = 4.0f;

// Shutdown
constexpr float SHUTDOWN_CURRENT_A = 1.0f;

// Fault
constexpr uint32_t FAULT_BLINK_MS = 500;
constexpr uint32_t FAULT_BEEP_MS = 50;
constexpr uint32_t FAULT_BEEP_PAUSE_MS = 2000;

// BMS
constexpr float BMS_MIN_VOLTAGE_V = 42.0f;
constexpr float BMS_MAX_CURRENT_A = 120.0f;
constexpr float BMS_MAX_CELL_DELTA_V = 0.1f;
constexpr float BMS_MAX_TEMP_C = 55.0f;

// Power
constexpr float PRECHARGE_DONE_RATIO = 0.90f;
constexpr uint32_t PRECHARGE_TIMEOUT_MS = 3000;

constexpr ThrottleMapConfig FORWARD_THROTTLE{THROTTLE_DEADZONE, THROTTLE_CURVE_EXPONENT, MAX_AMPS,
                                             RISE_RATE_PER_S, FALL_RATE_PER_S};
constexpr ThrottleMapConfig REVERSE_THROTTLE{THROTTLE_DEADZONE, THROTTLE_CURVE_EXPONENT,
                                             MAX_AMPS * REVERSE_AMPS_RATIO, REVERSE_RISE_RATE_PER_S,
                                             FALL_RATE_PER_S};

} // namespace App::Logic::Calibration