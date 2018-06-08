#include "amd.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ADL_MAIN_CONTROL_CREATE ADL_Main_Control_Create;
ADL_MAIN_CONTROL_DESTROY ADL_Main_Control_Destroy;
ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get;
ADL_ADAPTER_ADAPTERINFO_GET ADL_Adapter_AdapterInfo_Get;

// Memory allocation function
void* __stdcall ADL_Main_Memory_Alloc(int iSize){
	void* lpBuffer = malloc(iSize);
	return lpBuffer;
}

// Optional Memory de-allocation function
void __stdcall ADL_Main_Memory_Free(void** lpBuffer){
	if (*lpBuffer != NULL){
		free (*lpBuffer);
		*lpBuffer = NULL;
	}
}

void amd_init(){
	void *hDLL;

	hDLL = dlopen("libatiadlxx.so", RTLD_LAZY|RTLD_GLOBAL);


	if(hDLL == NULL){
		printf("Cannot load libatiadl\n");
		return;
	}

	ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE) dlsym(hDLL,"ADL_Main_Control_Create");
	ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY) dlsym(hDLL, "ADL_Main_Control_Destroy");
	ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET) dlsym(hDLL, "ADL_Adapter_NumberOfAdapters_Get");
	ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET) dlsym(hDLL, "ADL_Adapter_AdapterInfo_Get");

	if(ADL_Main_Control_Create == NULL || ADL_Main_Control_Destroy == NULL ||
			ADL_Adapter_NumberOfAdapters_Get == NULL || ADL_Adapter_AdapterInfo_Get == NULL){

		printf("Cannot init ADL library\n");
		return;
		// TODO: error handling
	}

	int ADL_Err;

	ADL_Err = ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1);
	if(ADL_Err != ADL_OK){
		printf("Failed to initialize memory for the ADL library\n");
		return;
		// TODO: error handling
	}

	int noAdapters;
	if(ADL_Adapter_NumberOfAdapters_Get(&noAdapters) != ADL_OK){
		printf("Cannot get number of adapters\n");
		return;
	}

	if(noAdapters <= 0){
		printf("No AMD devices found\n");
		return;
	}

	LPAdapterInfo adapterInfo;
	adapterInfo = (LPAdapterInfo) malloc(sizeof(AdapterInfo) * noAdapters);
	memset(adapterInfo, 0, sizeof(AdapterInfo) * noAdapters);

	ADL_Adapter_AdapterInfo_Get(adapterInfo, sizeof(AdapterInfo) * noAdapters);

	for(int i = 0; i < noAdapters; i++){
		AdapterInfo adapter = adapterInfo[i];

		printf("Adapter %d:\n", i);
		printf("Index: %d\n", adapter.iAdapterIndex);
		printf("Bus no: %d\n", adapter.iBusNumber);
		printf("Device no: %d\n", adapter.iDeviceNumber);
		printf("Function no: %d\n", adapter.iFunctionNumber);
		printf("Present: %d\n", adapter.iPresent);
		printf("Size: %d\n", adapter.iSize);
		printf("Vendor ID: %d\n", adapter.iVendorID);
		printf("Adapter name: %s\n", adapter.strAdapterName);
		printf("Display name: %s\n", adapter.strDisplayName);
		printf("UDID: %s\n\n", adapter.strUDID);
	}
}