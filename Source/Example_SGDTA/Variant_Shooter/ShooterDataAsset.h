// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ExampleDataAsset.h"

#include "ShooterDataAsset.generated.h"

/**
 * Base data asset type for the Shooter FPS variant.
 */
UCLASS()
class EXAMPLE_SGDTA_API UShooterDataAsset : public UExampleDataAsset
{
	GENERATED_BODY()
public:

	/** Notifies this data asset that it was picked up. */
	UFUNCTION(BlueprintCallable, Category = "Shooter Data Asset")
	void NotifyPickedUp(AShooterPickup* Pickup, const UWeaponInfoDTA* DTA,
		const TScriptInterface<IShooterWeaponHolder>& WeaponHolder);

protected:

	/** Executes when this data asset was "picked up". */
	UFUNCTION(BlueprintNativeEvent, Category = "Shooter Data Asset")
	void PickedUp(AShooterPickup* Pickup, const UWeaponInfoDTA* DTA,
		const TScriptInterface<IShooterWeaponHolder>& WeaponHolder);
	virtual void PickedUp_Implementation(AShooterPickup* Pickup, const UWeaponInfoDTA* DTA,
		const TScriptInterface<IShooterWeaponHolder>& WeaponHolder);
};
