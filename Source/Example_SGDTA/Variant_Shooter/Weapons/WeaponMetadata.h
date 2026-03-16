// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/SGDynamicTextAssetRef.h"

#include "UObject/Object.h"
#include "WeaponMetadata.generated.h"

class IShooterWeaponHolder;

class UWeaponInfoDTA;
class UShooterDataAsset;

class AShooterPickup;

/**
 * Instanced metadata that executes when this weapon is picked up.
 */
UCLASS(Abstract, EditInlineNew, Category = "SG")
class EXAMPLE_SGDTA_API UWeaponMetadata : public UObject
{
	GENERATED_BODY()
public:

	/** Executes this metadata's functionality when picked up. */
	UFUNCTION(BlueprintCallable, Category = "Weapon|Metadata")
	void ExecuteOnPickup(AShooterPickup* PickupActor, UWeaponInfoDTA* DTA, TScriptInterface<IShooterWeaponHolder> WeaponHolder);

protected:

	/**
	 * Native C++ implementation of ::ExecuteOnPickup.
	 * Not using BlueprintNativeEvent to avoid BP overriding functionality.
	 *
	 * Executes this metadata's functionality when picked up.
	 */
	virtual void Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA, TScriptInterface<IShooterWeaponHolder>& WeaponHolder) { }

	/**
	 * Blueprint implementation of ::ExecuteOnPickup.
	 * Executes this metadata's functionality when picked up.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon|Metadata", meta = (DisplayName = "Execute on Pickup"))
	void BP_ExecuteOnPickup(AShooterPickup* Pickup, UWeaponInfoDTA* DTA, const TScriptInterface<IShooterWeaponHolder>& WeaponHolder);
};

/**
 * Simple text metadata that's printed when this weapon is picked up.
 */
UCLASS()
class EXAMPLE_SGDTA_API UTextMetadata : public UWeaponMetadata
{
	GENERATED_BODY()
public:

	/** The message to display when this weapon is picked up. */
	UPROPERTY(EditAnywhere)
	FString Message;

	/** The amount of time to display the message. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
	float DisplayTime = 3.0f;

	/** The color of the text to display. */
	UPROPERTY(EditAnywhere)
	FLinearColor Color = FLinearColor::Red;

protected:

	// UWeaponMetadata overrides
	virtual void Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA, TScriptInterface<IShooterWeaponHolder>& WeaponHolder) override;
	// ~UWeaponMetadata overrides
};

/**
 * Simple DTA Ref metadata that prints the name of the referenced DTA when this weapon is picked up.
 */
UCLASS(DisplayName = "Dynamic Text Asset Ref Metadata")
class EXAMPLE_SGDTA_API UDTARefMetadata : public UWeaponMetadata
{
	GENERATED_BODY()
public:

	/** The DTA Ref to print the name of(if valid). */
	UPROPERTY(EditAnywhere)
	FSGDynamicTextAssetRef DTARef;

protected:

	// UWeaponMetadata overrides
	virtual void Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA, TScriptInterface<IShooterWeaponHolder>& WeaponHolder) override;
	// ~UWeaponMetadata overrides
};

/**
 * Simple Asset Bundle metadata that retrieves the asset bundle'd Shooter Data Assets and then notifies them that it was picked up.
 */
UCLASS()
class EXAMPLE_SGDTA_API UShooterDataAssetMetadata : public UWeaponMetadata
{
	GENERATED_BODY()
public:

	/** Shooter Data Assets to notify that they were picked up. */
	UPROPERTY(EditAnywhere, meta = (AssetBundles = "ExampleBundles"))
	TSet<TSoftObjectPtr<UShooterDataAsset>> DataAssets;

protected:

	// UWeaponMetadata overrides
	virtual void Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA, TScriptInterface<IShooterWeaponHolder>& WeaponHolder) override;
	// ~UWeaponMetadata overrides
};
