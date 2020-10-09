#pragma once
typedef struct {
	CListCtrl *List;
	int Index;
}MONITOR_ORDER_INFO;
//监控订单状态
unsigned int __stdcall MonitorOrderState(void* lp);
//监控指定订单
unsigned int __stdcall MonitorAssignOrder(void *lp);
//初始化订单状态
unsigned int __stdcall IniOrderStatus(void* lp);
//根据振幅变动，自动处理挂单逻辑
unsigned int __stdcall AutoMakeOrder(void* lp);