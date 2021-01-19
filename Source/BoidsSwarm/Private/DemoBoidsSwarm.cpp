// Copyright 2020 NeuralVFX, Inc. All Rights Reserved.

#include "DemoBoidsSwarm.h"
#include "cDataStorageGameInstance.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"


ADemoBoidsSwarm::ADemoBoidsSwarm()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(GetRootComponent());

	// Setup instanced static mesh
	ISMCA = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMCA"));
	SetRootComponent(ISMCA);
	ISMCA->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	ISMCA->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ISMCA->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ISMCA->SetMobility(EComponentMobility::Movable);
	ISMCA->SetCastShadow(true);

	// Scene components for targeting and avoidance
	Avoid = CreateDefaultSubobject<USceneComponent>(TEXT("Avoid"));
	Avoid->SetupAttachment(RootComponent);

	Goal = CreateDefaultSubobject<USceneComponent>(TEXT("Goal"));
	Goal->SetupAttachment(RootComponent);

	// Defualt values
	fish_size = .3;
	WhichControl = false;

	// DLL Initiation default values
	N = 1000;
	width = 400;
	height = 400;
	depth = 400;
	min_dist = 25.0;
	max_dist = 50.0;
	vel_mult = 1.5;
	min_vel = 0.5;
	max_vel = 2.0;
	maxacc = 0.03;

	// DLL Tick defualt values
	GoalStrength = .5;
	AvoidStrength = 1.;
	AvoidDist = 400.;
	ticket = 0;
}


void ADemoBoidsSwarm::BeginPlay()
{
	Super::BeginPlay();

	// Set camera far back to view flock
	Camera->SetRelativeLocation(FVector(-500, 0, 0));

	// Fill initiation struct
	AttributeData attributes;
	attributes.count = N;
	attributes.width = width;
	attributes.height = height;
	attributes.depth = depth;
	attributes.min_dist = min_dist;
	attributes.max_dist = max_dist;
	attributes.vel_mult = vel_mult;
	attributes.min_vel = min_vel;
	attributes.max_vel = max_vel;
	attributes.maxacc = maxacc;

	// Initiate DLL
	UcDataStorageGameInstance* GameInst = (UcDataStorageGameInstance*)GetGameInstance();
	ticket = GameInst->CustomStart(attributes);

	// Allocate correct amount of memory  to recieve data from DLL
	pos = (float*)malloc(sizeof(float)*N * 3);
	vel = (float*)malloc(sizeof(float)*N * 3);

	// Build array of instances
	for (int i = 0; i < N; i++)
	{
		FTransform position(FVector(0, 0, 0));
		ISMCA->AddInstance(position);
	}

	// Hide one of the targets, so that later we ca toggle between one another
	Goal->ToggleVisibility(true);
}


void ADemoBoidsSwarm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get target locations
	FVector GoalLocation = (Goal->GetRelativeTransform().GetLocation());
	FVector AvoidLocation = (Avoid->GetRelativeTransform().GetLocation());

	// Prepare struct to pass to DLL
	TickData tick_attrs;
	tick_attrs.gx = GoalLocation.X;
	tick_attrs.gy = GoalLocation.Y;
	tick_attrs.gz = GoalLocation.Z;
	tick_attrs.ax = AvoidLocation.X;
	tick_attrs.ay = AvoidLocation.Y;
	tick_attrs.az = AvoidLocation.Z;
	tick_attrs.a_mult = AvoidStrength;
	tick_attrs.t_mult = GoalStrength;
	tick_attrs.a_dist = AvoidDist;
	tick_attrs.ticket = ticket;

	// Run single iteration of BOID
	UcDataStorageGameInstance* GameInst = (UcDataStorageGameInstance*)GetGameInstance();
	GameInst->Run(pos, vel, tick_attrs);

	// Get actor transform to multiply DLL output by
	FTransform ActorTran = GetActorTransform();

	// Loop through and set posistion and orientation of all instances
	for (int i = 0; i < N; i++)
	{
		FMatrix rot = FRotationMatrix::MakeFromX(FVector(vel[i * 3], vel[(i * 3) + 1], vel[(i * 3) + 2]));
		FTransform position(rot.ToQuat(), FVector(pos[i * 3], pos[(i * 3) + 1], pos[(i * 3) + 2]), FVector(fish_size));
		ISMCA->UpdateInstanceTransform(i, position*ActorTran, true);
		ISMCA->MarkRenderStateDirty();
	}

	// Animate the scale of the targets
	float time = GetGameTimeSinceCreation();
	Goal->SetWorldScale3D(FVector(1 - (time - int(time))));
	Avoid->SetWorldScale3D(FVector(time - int(time)));
}

void ADemoBoidsSwarm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set keyboard mapping
	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &ADemoBoidsSwarm::Switch);

	// Set mouse mapping
	PlayerInputComponent->BindAxis("Right", this, &ADemoBoidsSwarm::MouseRight);
	PlayerInputComponent->BindAxis("Up", this, &ADemoBoidsSwarm::MouseUp);
}

void  ADemoBoidsSwarm::MouseRight(float Value)
{
	// Add movment to either attraction or repulsion target
	if (WhichControl)
	{
		Goal->AddWorldOffset(FVector(0, Value * 1000 * GetWorld()->GetDeltaSeconds(), 0));
	}
	else
	{
		Avoid->AddWorldOffset(FVector(0, Value * 1000 * GetWorld()->GetDeltaSeconds(), 0));
	}
}


void  ADemoBoidsSwarm::MouseUp(float Value)
{
	// Add movment to either attraction or repulsion target
	if (WhichControl)
	{
		Goal->AddWorldOffset(FVector(0, 0, Value * 1000 * GetWorld()->GetDeltaSeconds()));
	}
	else
	{
		Avoid->AddWorldOffset(FVector(0, 0, Value * 1000 * GetWorld()->GetDeltaSeconds()));
	}
}


void  ADemoBoidsSwarm::Switch()
{
	// Toggle variable
	WhichControl = !WhichControl;

	// Toggle actual visibility
	Goal->ToggleVisibility(true);
	Avoid->ToggleVisibility(true);
}
