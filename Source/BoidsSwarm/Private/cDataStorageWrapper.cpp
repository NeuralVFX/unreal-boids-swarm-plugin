// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "cDataStorageWrapper.h"
#include "Misc/Paths.h"


bool UcDataStorageWrapper::ImportDLL(FString FolderName, FString DLLName)
{
	// Init DLL from a Path
	FString FilePath = *FPaths::ProjectPluginsDir() + FolderName + "/" + DLLName;

	if (FPaths::FileExists(FilePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*FilePath);
		if (v_dllHandle != NULL)
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load %s"),*FilePath);
		}
	}
	return false;
}


bool UcDataStorageWrapper::ImportMethods()
{
	// Loop through and import all Functions from DLL   --   make sure proc_name matches name of DLL method
	if (v_dllHandle != NULL)
	{
		FString ProcName = "InitNet";
		m_funcInit = (__Init)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_funcInit == NULL)
		{
			return false;
		}
		ProcName = "CloseNet";
		m_funcClose = (__Close)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_funcClose == NULL)
		{
			return false;
		}
		ProcName = "Run";
		m_funcRun = (__Run)FPlatformProcess::GetDllExport(v_dllHandle, *ProcName);
		if (m_funcRun == NULL)
		{
			return false;
		}
	}
	return true;
}


int UcDataStorageWrapper::CallInit(AttributeData attributes)
{
	// Check if DLL function is loaded
	if (m_funcInit == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Function Not Loaded From DLL: Init"));
		return -1;
	}

	// Calls DLL function to initial LibTorch model
	int init = m_funcInit(attributes);
	return init;
}


int UcDataStorageWrapper::CallClose()
{
	// Check if DLL function is loaded
	if (m_funcClose == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Function Not Loaded From DLL: Close"));
		return -1;
	}

	// Calls DLL Function to ShutDown Camera
	m_funcClose();
	UE_LOG(LogTemp, Log, TEXT("Connection Close"));

	return 1;
}


int UcDataStorageWrapper::CallRun(float* pos, float* vel, TickData tick_attrs)
{
	// Check if DLL function is loaded
	if (m_funcRun == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Function Not Loaded From DLL: Run"));
		return -1;
	}

	// Calls DLL Function to Run LibTorch Model and Return Image
	bool result = m_funcRun(pos, vel, tick_attrs);

	if (result)
	{
		return 1;
	}

	return 0;
}


