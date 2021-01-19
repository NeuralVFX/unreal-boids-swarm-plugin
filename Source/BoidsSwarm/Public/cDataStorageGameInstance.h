// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "cDataStorageWrapper.h"

#include "cDataStorageGameInstance.generated.h"

/** Game Instance which is responsible for loading and calling DLL wrapper */
UCLASS()
class BOIDSSWARM_API UcDataStorageGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	/** Storage for DLL object */
	UPROPERTY()
		class UcDataStorageWrapper* m_refDataStorageUtil;

	/**
	* Attempt to import DLL and all of its functions.
	* @return Whether the operation is succesfull.
	*/
	bool ImportDataStorageLibrary();

public:

	/**
	* Overridden to import DLL and DLL functions on Init.
	*/
	virtual void Init() override;

	/**
	 * Empty the Libtorch model array,
	 */
	virtual void Shutdown() override;

	/**
	 * Create Libtorch model and set attributes.
	 * @param attributes - Set of attributes required for initialization.
	 * @return interger of what index in the model array the new model is assigned.
	 */
	int CustomStart(AttributeData attributes);

	/**
	 * Run single tick of Boids simulation.
	 * @param pos - Array to ouput new position values to.
	 * @param vel - Array to ouput new velocity values to.
	 * @param tick_attrs - Set of attributes required for tick.
	 * @return Whether the operatio was successful.
	 */
	void Run(float* pos, float* vel, TickData tick_attrs);

};
