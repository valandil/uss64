#include "uss64.h"

void uss64_count_to_timer(int64_t count, struct uss64_timer* timer)
{
  // Initial values.
  timer->count = count;
  timer->hundreths = count * 100 / OS_CPU_COUNTER;
  timer->seconds = timer->hundreths / 100;
  timer->minutes = timer->seconds / 60;
  timer->hours = timer->minutes / 60;

  // Modulo.
  timer->hundreths %= 100;
  timer->seconds %= 60;
  timer->minutes %= 60;
}