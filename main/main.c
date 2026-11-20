#include <stdio.h>

#define CAT_(a, b, c) a##b##c
#define CAT(a, b, c)  CAT_(a, b, c)

#if CONFIG_COURSE_DAY == 1
  #define RUN_DAY day1and2_run
#elif CONFIG_COURSE_DAY == 2
  #define RUN_DAY day1and2_run
#else
  #define RUN_DAY CAT(day, CONFIG_COURSE_DAY, _run)   // 25 -> day25_run
#endif

void RUN_DAY(void);

void app_main() {
    RUN_DAY();
}
