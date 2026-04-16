#include "Rtttl.h"
#include <Arduino.h>

#include "config/Pins.h"

bool playSound = false;

#define isdigit(n) (n >= '0' && n <= '9')

const char* rtttlPtr = nullptr;
unsigned long noteEnd = 0;
bool rtttlPlaying = false;
static byte default_dur = 4;
static byte default_oct = 6;
static int bpm = 63;
static long wholenote = 0;
static bool headerParsed = false;


void start_rtttl(const char* p) {
  rtttlPtr = p;

  while (*rtttlPtr && *rtttlPtr != ':') rtttlPtr++;
  if (*rtttlPtr == ':') rtttlPtr++;

  noteEnd = 0;
  rtttlPlaying = true;
  headerParsed = false;
}
void stop_rtttl(){
  if(!rtttlPlaying)return;
  rtttlPlaying = false;
  headerParsed = false;
  rtttlPtr = nullptr;
  noteEnd = 0;
  noTone(BUZZER_PIN);
}
void update_rtttl() {
  if (!rtttlPlaying || !rtttlPtr) return;

  unsigned long now = millis();
  if (noteEnd && now < noteEnd) return;

  if (!headerParsed) {
    default_dur = 4;
    default_oct = 6;
    bpm = 63;

    while (*rtttlPtr && *rtttlPtr != ':') {
      if (*rtttlPtr == 'd' && rtttlPtr[1] == '=') {
        rtttlPtr += 2;
        int num = 0;
        while (isdigit(*rtttlPtr)) num = num * 10 + (*rtttlPtr++ - '0');
        if (num > 0) default_dur = num;
      } 
      else if (*rtttlPtr == 'o' && rtttlPtr[1] == '=') {
        rtttlPtr += 2;
        if (isdigit(*rtttlPtr)) default_oct = *rtttlPtr++ - '0';
      } 
      else if (*rtttlPtr == 'b' && rtttlPtr[1] == '=') {
        rtttlPtr += 2;
        int num = 0;
        while (isdigit(*rtttlPtr)) num = num * 10 + (*rtttlPtr++ - '0');
        if (num > 0) bpm = num;
      } 
      else {
        rtttlPtr++;
      }

      if (*rtttlPtr == ',') rtttlPtr++;
    }

    if (*rtttlPtr != ':') {
      rtttlPlaying = false;
      noTone(BUZZER_PIN);
      return;
    }

    rtttlPtr++;
    wholenote = (60 * 1000L / bpm) * 4;
    headerParsed = true;
  }

  while (*rtttlPtr == ' ' || *rtttlPtr == ',') rtttlPtr++;

  if (*rtttlPtr == 0) {
    rtttlPlaying = false;
    noTone(BUZZER_PIN);
    return;
  }

  int num = 0;
  while (isdigit(*rtttlPtr)) num = num * 10 + (*rtttlPtr++ - '0');

  long duration = (num > 0) ? (wholenote / num) : (wholenote / default_dur);

  byte note = 0;
  switch (*rtttlPtr) {
    case 'c': note = 1;  break;
    case 'd': note = 3;  break;
    case 'e': note = 5;  break;
    case 'f': note = 6;  break;
    case 'g': note = 8;  break;
    case 'a': note = 10; break;
    case 'b': note = 12; break;
    case 'p': note = 0;  break;
  }
  if (*rtttlPtr) rtttlPtr++;

  if (*rtttlPtr == '#') {
    note++;
    rtttlPtr++;
  }

  if (*rtttlPtr == '.') {
    duration += duration / 2;
    rtttlPtr++;
  }

  byte scale = default_oct;
  if (isdigit(*rtttlPtr)) {
    scale = *rtttlPtr - '0';
    rtttlPtr++;
  }
  scale += OCTAVE_OFFSET;

  if (*rtttlPtr == ',') rtttlPtr++;

  if (note) {
    int idx = (scale - 4) * 12 + note;
    if (idx >= 0 && idx < (int)(sizeof(notes) / sizeof(notes[0]))) {
      tone(BUZZER_PIN, notes[idx]);
    } else {
      noTone(BUZZER_PIN);
    }
  } else {
    noTone(BUZZER_PIN);
  }

  noteEnd = now + duration;
}
