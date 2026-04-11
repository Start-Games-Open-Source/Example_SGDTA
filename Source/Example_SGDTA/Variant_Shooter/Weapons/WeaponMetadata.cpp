// Copyright Start Games, Inc. All Rights Reserved.

#include "WeaponMetadata.h"

#include "ShooterDataAsset.h"
#include "ShooterPickup.h"
#include "ShooterUtilityLibrary.h"
#include "UI/ShooterUIMessage.h"
#include "WeaponInfoDTA.h"
#include "Weapons/WeaponLogs.h"

void UWeaponMetadata::ExecuteOnPickup(AShooterPickup* Pickup, UWeaponInfoDTA* DTA, TScriptInterface<IShooterWeaponHolder> WeaponHolder)
{
	if (!DTA)
	{
		UE_LOG(LogWeapon, Error, TEXT("UWeaponMetadata::ExecuteOnPickup: Inputted NULL DTA to UWeaponMetadata(%s)"), *GetName());
		return;
	}
	if (!Pickup)
	{
		UE_LOG(LogWeapon, Error, TEXT("UWeaponMetadata::ExecuteOnPickup: Inputted NULL Pickup to UWeaponMetadata(%s)|DTA(%s)"), *GetName(), *DTA->GetUserFacingId());
		return;
	}
	if (!WeaponHolder)
	{
		UE_LOG(LogWeapon, Error, TEXT("UWeaponMetadata::ExecuteOnPickup: Inputted NULL WeaponHolder to UWeaponMetadata(%s)|DTA(%s)"), *GetName(), *DTA->GetUserFacingId());
		return;
	}
	Native_ExecuteOnPickup(*Pickup, *DTA, WeaponHolder);
	BP_ExecuteOnPickup(Pickup, DTA, WeaponHolder);
}

void UTextMetadata::Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA, TScriptInterface<IShooterWeaponHolder>& WeaponHolder)
{
	GEngine->AddOnScreenDebugMessage(-1, DisplayTime, Color.ToFColorSRGB(),
		FString::Printf(TEXT("%s: %s"),
			*DTA.GetUserFacingId(), *Message));
}

void UDTARefMetadata::Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA, TScriptInterface<IShooterWeaponHolder>& WeaponHolder)
{
	if (!DTARef.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			FString::Printf(TEXT("%s: No DTA Reference Set."), *DTA.GetUserFacingId()));
		return;
	}
	FShooterUIMessage message;
	message.Message = FText::Format(INVTEXT("{0}: {1} picked up weapon and DTA Ref: {2}")
		, FText::FromString(DTA.GetUserFacingId())
		, FText::FromString(GetNameSafe(WeaponHolder.GetObject()))
		, FText::FromString(DTARef.GetUserFacingId()));

	UShooterUtilityLibrary::SendShooterUIMessage(&Pickup, message);
}

void UShooterDataAssetMetadata::Native_ExecuteOnPickup(AShooterPickup& Pickup, const UWeaponInfoDTA& DTA,
	TScriptInterface<IShooterWeaponHolder>& WeaponHolder)
{
	AShooterPickup* pickupPtr = &Pickup;
	const UWeaponInfoDTA* dtaPtr = &DTA;
	for (const TSoftObjectPtr<UShooterDataAsset>& asset : DataAssets)
	{
		UE_LOG(LogWeapon, Log, TEXT("NOTE: %s: valid data asset(%s)"),
			*DTA.GetUserFacingId(),
			*asset.GetAssetName());
		if (!asset.IsValid())
		{
			UE_LOG(LogWeapon, Error, TEXT("%s: %s failed to load via asset bundles."),
				*DTA.GetUserFacingId(),
				*asset.GetAssetName());
			continue;
		}
		asset.Get()->NotifyPickedUp(pickupPtr, dtaPtr, WeaponHolder);
	}
}
