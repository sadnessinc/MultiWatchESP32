#pragma once

#include <Arduino.h>



bool syncTime (bool needPrint = false, uint32_t timeoutMs = 5000);
void checkLastSync();