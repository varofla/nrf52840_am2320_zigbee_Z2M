#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>

#include "hw.h"

#include "am2320.h"

#define FOO_WORK_QUEUE_PRIORITY 6     // WorkQueue 우선순위
#define FOO_WORK_QUEUE_STACK_SIZE 512 // WorkQueue Handler 함수의 스택 크기

#define FOO_THREAD_PRIORITY 5 // 우선순위 높은 테스트 thread 우선순위
#define FOO_THREAD_STACKSIZE 512

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static void s_foo_work_hd(struct k_work *);                                // WorkQueue Handler 함수 전방선언
static struct k_work_q s_foo_work_queue = {0};                             // WorkQueue
static K_WORK_DEFINE(s_foo_work, s_foo_work_hd);                           // Work 생성 (Work와 Work Handler 함수 연결)
static K_THREAD_STACK_DEFINE(s_foo_work_stack, FOO_WORK_QUEUE_STACK_SIZE); // WorkQueue Stack 할당

/**
 * @brief 대충 시간 많이 걸리는 작업
 *
 */
static inline void s_some_work() {
  for (volatile int count_out = 0; count_out < 150000; count_out++) {
    ;
  }
}

/**
 * @brief WorkQueue Handler 함수
 *
 */
static void s_foo_work_hd(struct k_work *) {
  LOG_INF("work starts!");

  s_some_work(); // 대충 시간 많이 걸리는 작업

  LOG_INF("work finished!");
}

/**
 * @brief ISR과 같이 우선순위가 높은 Thread
 *
 */
static void s_foo_thread(void) {
  k_work_queue_start(&s_foo_work_queue, s_foo_work_stack, K_THREAD_STACK_SIZEOF(s_foo_work_stack), FOO_WORK_QUEUE_PRIORITY, NULL);

  for (;;) {
    k_work_submit_to_queue(&s_foo_work_queue, &s_foo_work);
    k_msleep(1000);
  }
}

K_THREAD_DEFINE(foo_thread_id, FOO_THREAD_STACKSIZE, s_foo_thread, NULL, NULL, NULL, FOO_THREAD_PRIORITY, 0, 0);
