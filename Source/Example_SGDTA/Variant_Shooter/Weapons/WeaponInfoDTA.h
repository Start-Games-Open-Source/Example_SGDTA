// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/SGDynamicTextAsset.h"

#include "WeaponInfoDTA.generated.h"

class UStaticMesh;
class UWeaponMetadata;

class AShooterWeapon;

/**
 * Dynamic Text Asset container for a type of weapon pickup.
 */
UCLASS(ClassGroup = "SGDTA Example")
class EXAMPLE_SGDTA_API UWeaponInfoDTA : public USGDynamicTextAsset
{
	GENERATED_BODY()
public:

	/** Mesh to display on the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	TSoftObjectPtr<UStaticMesh> StaticMesh = nullptr;

	/** Weapon class to grant on pickup */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Info", meta = (AssetBundles = "Weapon"))
	TSoftClassPtr<AShooterWeapon> WeaponToSpawn = nullptr;

	/** Metadata to execute when this weapon is picked up. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Weapon Info")
	TArray<TObjectPtr<UWeaponMetadata>> Metadata;
};
