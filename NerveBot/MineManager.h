#pragma once
typedef struct {
	float base_asset_added;
	float quote_asset_added;
	float quote_base_asset_added;
	float TotalAssetAdded;
}ASSET_ADDED;
unsigned int __stdcall IniMineManager(void* lp);
unsigned int __stdcall GetRewardingList(void* lp);
unsigned int __stdcall TransfeThread(void* lp);