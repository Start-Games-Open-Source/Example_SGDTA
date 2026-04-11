// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Variant_Shooter/UI/ShooterUIMessage.h"

#include "ShooterUtilityLibrary.generated.h"

/**
 * Utility library for Shooter variant.
 */
UCLASS(ClassGroup = "Example SGDTA")
class EXAMPLE_SGDTA_API UShooterUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/** Sends a UI message intended for the shooter variant. */
	UFUNCTION(BlueprintCallable, Category = "Shooter Utility|UI", meta = (WorldContext = "WorldContextObject"))
	static void SendShooterUIMessage(const UObject* WorldContextObject, FShooterUIMessage Message);
};
