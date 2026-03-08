// Copyright Epic Games, Inc. All Rights Reserved.


#include "ShooterPickup.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShooterWeaponHolder.h"
#include "ShooterWeapon.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "WeaponInfoDTA.h"

#include "Engine/AssetManager.h"

AShooterPickup::AShooterPickup()
{
 	PrimaryActorTick.bCanEverTick = true;

	// create the root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// create the collision sphere
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(RootComponent);

	SphereCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 84.0f));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldStatic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->bFillCollisionUnderneathForNavmesh = true;

	// subscribe to the collision overlap on the sphere
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AShooterPickup::OnOverlap);

	// create the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SphereCollision);

	Mesh->SetCollisionProfileName(FName("NoCollision"));
}

void AShooterPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// New favored approach
	if (WeaponType.IsValid())
	{
		if (!WeaponType.IsNull())
		{
			TWeakObjectPtr<AShooterPickup> self = this;
			WeaponType.LoadAsync(this, [self](TScriptInterface<ISGDynamicTextAssetProvider> Provider, bool bSuccess)
			{
				// If we're no longer valid/failed to load it
				if (!self.IsValid() || !bSuccess)
				{
					return;
				}
				// Now that the ref is loaded, retrieve the static mesh and load it
				if (UWeaponInfoDTA* info = Cast<UWeaponInfoDTA>(Provider.GetObject()))
				{
					// Set the mesh
					self->Mesh->SetStaticMesh(UAssetManager::Get().GetStreamableManager().LoadSynchronous(info->StaticMesh));
				}
			});
		}
	}
}

void AShooterPickup::BeginPlay()
{
	Super::BeginPlay();

	SG_LOAD_REF_SIMPLE(WeaponType, this,
		{
			if (!self.IsValid() || !bSuccess)
			{
				return;
			}
			if (UWeaponInfoDTA* info = Cast<UWeaponInfoDTA>(Provider.GetObject()))
			{
				// Copy the weapon class
				self->WeaponClass = UAssetManager::Get().GetStreamableManager().LoadSynchronous(info->WeaponToSpawn);
			}
		});
}

void AShooterPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void AShooterPickup::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// have we collided against a weapon holder?
	if (IShooterWeaponHolder* WeaponHolder = Cast<IShooterWeaponHolder>(OtherActor))
	{
		WeaponHolder->AddWeaponClass(WeaponClass);

		// hide this mesh
		SetActorHiddenInGame(true);

		// disable collision
		SetActorEnableCollision(false);

		// disable ticking
		SetActorTickEnabled(false);

		// schedule the respawn
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AShooterPickup::RespawnPickup, RespawnTime, false);
	}
}

void AShooterPickup::RespawnPickup()
{
	// unhide this pickup
	SetActorHiddenInGame(false);

	// call the BP handler
	BP_OnRespawn();
}

void AShooterPickup::FinishRespawn()
{
	// enable collision
	SetActorEnableCollision(true);

	// enable tick
	SetActorTickEnabled(true);
}
