// Copyright Start Games, Inc. All Rights Reserved.

#include "ShooterDataAsset.h"

#include "ShooterUIMessage.h"
#include "ShooterUtilityLibrary.h"
#include "WeaponInfoDTA.h"
#include "WeaponLogs.h"
#include "Weapons/ShooterPickup.h"

void UShooterDataAsset::NotifyPickedUp(AShooterPickup* Pickup, const UWeaponInfoDTA* DTA,
                                       const TScriptInterface<IShooterWeaponHolder>& WeaponHolder)
{
	if (!Pickup)
	{
		UE_LOG(LogWeapon, Error, TEXT("UShooterDataAsset::NotifyPickedUp: Inputted NULL Pickup"));
		return;
	}
	if (!DTA)
	{
		UE_LOG(LogWeapon, Error, TEXT("UShooterDataAsset::NotifyPickedUp: Inputted NULL DTA. Pickup(%s)"),
			*GetNameSafe(Pickup));
		return;
	}
	if (!WeaponHolder)
	{
		UE_LOG(LogWeapon, Error, TEXT("UShooterDataAsset::NotifyPickedUp: Inputted NULL WeaponHolder. Pickup(%s)|DTA(%s)"),
			*GetNameSafe(Pickup),
			*DTA->GetUserFacingId());
		return;
	}
	PickedUp(Pickup, DTA, WeaponHolder);
}

void UShooterDataAsset::PickedUp_Implementation(AShooterPickup* Pickup, const UWeaponInfoDTA* DTA,
	const TScriptInterface<IShooterWeaponHolder>& WeaponHolder)
{
	FShooterUIMessage message;
	message.Message = FText::Format(INVTEXT("{0}: {1} picked up by {2}")
		, FText::FromString(DTA->GetUserFacingId())
		, FText::FromString(GetName())
		, FText::FromString(GetNameSafe(WeaponHolder.GetObject())));
	message.Duration = 10.0f;

	UShooterUtilityLibrary::SendShooterUIMessage(Pickup, message);
}
