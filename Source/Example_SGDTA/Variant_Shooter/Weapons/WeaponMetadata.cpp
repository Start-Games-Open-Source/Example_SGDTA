// Copyright Start Games, Inc. All Rights Reserved.

#include "WeaponMetadata.h"

#include "ShooterPickup.h"
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
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
		FString::Printf(TEXT("%s: %s picked up weapon and DTA Ref: %s"),
		*DTA.GetUserFacingId(),
		*GetNameSafe(WeaponHolder.GetObject()),
		*DTARef.GetUserFacingId()));
}
