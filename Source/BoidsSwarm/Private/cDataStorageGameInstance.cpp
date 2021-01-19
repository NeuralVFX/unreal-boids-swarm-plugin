// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "cDataStorageGameInstance.h"


void UcDataStorageGameInstance::Init()
{
	// Init DLL
	Super::Init();
	if (ImportDataStorageLibrary())
	{
		UE_LOG(LogTemp, Log, TEXT("DLL Loaded"));
	}
}


bool UcDataStorageGameInstance::ImportDataStorageLibrary()
{
	// Import the DLL 
	m_refDataStorageUtil = NewObject<UcDataStorageWrapper>(this);
	if (m_refDataStorageUtil == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not Create the Data Storage Object"));
		return false;
	}
	if (!m_refDataStorageUtil->ImportDLL("unreal-boids-swarm-plugin/Binaries/Win64", "boids.dll"))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Import DLL"));
		return false;
	}
	// Import all methods of DLL
	if (!m_refDataStorageUtil->ImportMethods())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Import DLL Methods"));
		return false;
	}
	return true;
}


void UcDataStorageGameInstance::Shutdown()
{
	int Result = m_refDataStorageUtil->CallClose();
	Super::Shutdown();
	UE_LOG(LogTemp, Error, TEXT("Release Torch Models"))
}


int UcDataStorageGameInstance::CustomStart(AttributeData attributes)
{
	int Result = m_refDataStorageUtil->CallInit(attributes);
	UE_LOG(LogTemp, Log, TEXT("Creating Torch Model"));
	return Result;
}


void UcDataStorageGameInstance::Run(float* pos, float* vel, TickData tick_attrs)
{
	int Result = m_refDataStorageUtil->CallRun(pos, vel, tick_attrs);
}



