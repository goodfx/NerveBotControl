#pragma once
typedef struct {
	std::string OrderId;
	int Index;
	CListCtrl *List;
	int Status;//0成交订单，不需要再删除，1为需要删除，2为删除完成
}CANCEL_ORDER_INFO;
unsigned int __stdcall CancelOrders(void*lp);