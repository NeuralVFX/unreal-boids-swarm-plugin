// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "cDataStorageWrapper.generated.h"


/** Struct to hold attribute data needed to initialize simulation */
struct AttributeData
{
	/** Count and dimensions */
	int count, width, height, depth;

	/** Velocitie limites*/
	float min_dist, max_dist, vel_mult;
	float min_vel, max_vel, maxacc;
};


/** Struct to hold attribute data which can change per tick */
struct TickData
{
	/** Goal and avoid targets */
	float gx, gy, gz;
	float ax, ay, az;

	/** Multipliers and distance cut-off */
	float t_mult, a_mult, a_dist;

	/** Index of which model to lookup */
	int ticket;
};


/** DLL functions */
typedef int(*__Init)(AttributeData attributes);
typedef void(*__Close)();
typedef bool(*__Run)(float* pos, float * vel, TickData tick_attrs);


/** Wrapper for external DLL, allows the executing of BOIDs algorithm and passes the data back to Unreal */
UCLASS()
class BOIDSSWARM_API UcDataStorageWrapper : public UObject
{
	GENERATED_BODY()
private:

	/** DLL Handle */
	void *v_dllHandle;

	/** DLL Functions */
	__Init m_funcInit;
	__Close m_funcClose;
	__Run m_funcRun;

public:

	/**
	* Attempt to import DLL.
	* @param a_strFolderName -  Folder of DLL.
	* @param a_strDLLName - Name of DLL file.
	* @return Whether the operation is succesfull.
	*/
	bool ImportDLL(FString a_strFolderName, FString a_strDLLName);

	/**
	* Attempt to import all functions of the DLL.
	* @return Whether the operation is succesfull.
	*/
	bool ImportMethods();

	/**
	 * Create Libtorch model and set attributes.
	 * @param attributes - Set of attributes required for initialization.
	 * @return interger of what index in the model array the new model is assigned, or -1 if fuction not loaded.
	 */
	int CallInit(AttributeData attributes);

	/**
	 * Empty the Libtorch model array.
	 * @return -1: function not loaded - 0: error running - 1: success.
	 */
	int CallClose();

	/**
	 * Run single tick of Boids simulation through Libtorch.
	 * @param pos - Array to ouput new position values to.
	 * @param vel - Array to ouput new velocity values to.
	 * @param tick_attrs - Set of attributes required for tick.
	 * @return -1: function not loaded - 0: error running - 1: success.
	 */
	int CallRun(float* pos, float* vel, TickData tick_attrs);
};






