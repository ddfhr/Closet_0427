#ifndef __TIME_HANDLE_H__ // 防止头文件的多重包含
#define __TIME_HANDLE_H__

// 引入tim.h头文件，这个头文件可能定义了一些定时器相关的数据类型或函数
#include "tim.h"

// 定义一个结构体来表示定时器
typedef struct Timer
{
    unsigned int timeout;     // 定时时间
    unsigned int repeat;      // 是否重复，0表示不重复，其他值表示重复的间隔时间
    void (*timeout_cb)(void); // 定时器回调函数，当定时时间到达时会被调用
    struct Timer *next;       // 指向下一个定时器的指针
} Timer;

// 初始化定时器外设，启动定时器4的中断模式
void timer_peripheral_init(void);

// 初始化一个定时器结构体，并设置回调函数、超时时间和重复间隔（如果需要）
void timer_init(struct Timer *handle, void (*timeout_cb)(), unsigned int timeout, unsigned int repeat);

// 将新创建的定时器添加到链表中（如果添加成功，返回1；如果已经存在相同的定时器，返回0）
int timer_start(struct Timer *handle);

// 停止一个定时器
void timer_stop(struct Timer *handle);

// 处理定时器滴答（tick）事件，每次调用都会增加计时器的计数
void timer_ticks(void);

// 循环处理定时器事件，可能会持续调用回调函数等操作
void timer_loop(void);

#endif // 结束防止头文件的多重包含的预处理指令块
