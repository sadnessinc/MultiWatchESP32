#pragma once
#include <time.h>

enum TimeState {
    TIME_IDLE,
    TIME_WIFI_CONNECTING,
    TIME_NTP_REQUESTED,
    TIME_WAITING,
    TIME_READY,
    TIME_ERROR
};

void timeUpdate();
TimeState getTimeState();
bool getCurrentTimeHHMM(char* out, size_t outSize);
bool getCurrentTime(struct tm &info);

extern bool syncedOnce;
extern struct tm lastSyncTime;
extern unsigned long lastSync;