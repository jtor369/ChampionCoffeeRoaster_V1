#pragma once

#include "Seeed_MCP9600.h"

err_t sensor_basic_config(MCP9600 *_sensor);
err_t get_temperature(MCP9600 *_sensor, float *value);
