// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DemoBoidsSwarm.generated.h"


/**
* Runs BIODs simulation, and applies the result to a StaticInstancedMesh.
* Allows mouse control over the target point and avoidance point.
* Control between the two can be swapped by hitting "F".
*/
UCLASS()
class BOIDSSWARM_API ADemoBoidsSwarm : public APawn
	{
		GENERATED_BODY()

	public:
		ADemoBoidsSwarm();

		/** Camera to attach to boom */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
			class UCameraComponent* Camera;

		/** Swarm/Flock instances */
		UPROPERTY(EditAnywhere, Category = "Mesh")
			class UInstancedStaticMeshComponent* ISMCA;

		/** Scale of Swarm/Flock instances */
		UPROPERTY(EditAnywhere, Category = "Mesh")
			float fish_size;

		/**
		*Attributes required to initiate LibTorch model
		*/
		UPROPERTY(EditAnywhere, Category = "Particles")
			int N;

		UPROPERTY(EditAnywhere, Category = "Particles")
			int width;

		UPROPERTY(EditAnywhere, Category = "Particles")
			int height;

		UPROPERTY(EditAnywhere, Category = "Particles")
			int depth;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float min_dist;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float max_dist;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float vel_mult;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float min_vel;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float  max_vel;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float maxacc;

		/**
		* Attributes required to tick LibTorch model
		*/
		UPROPERTY(EditAnywhere, Category = "Particles")
			float GoalStrength;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float AvoidDist;

		UPROPERTY(EditAnywhere, Category = "Particles")
			float AvoidStrength;

		/**
		*Scene components for attraction and repultion, controlled by mouse
		*/
		UPROPERTY(EditAnywhere, Category = "Particles")
			class USceneComponent* Goal;

		UPROPERTY(EditAnywhere, Category = "Particles")
			class USceneComponent* Avoid;

		/** Index of which model we are assigned by DLL */
		UPROPERTY(VisibleAnywhere, Category = "Particles")
			int ticket;

		/** Pointer to fetch position and velocity data from DLL */
		float* pos;
		float* vel;

		/** Whether the mouse controls the attraction or repulsion target */
		bool WhichControl;

	protected:

		virtual void BeginPlay() override;

	public:

		virtual void Tick(float DeltaTime) override;

		/**
		 * Setup mouse and keyboard control.
		 * @param PlayerInputComponent - InputComponent supplied on run.
		 */
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		/**
		 * Called to move target left/right.
		 * @param Value - Amount to move.
		 */
		void MouseRight(float Value);

		/**
		 * Called to move target up/down.
		 * @param Value - Amount to move.
		 */
		void MouseUp(float Value);

		/**
		*Switch Target and Avoid
		*/
		void Switch();
	};
