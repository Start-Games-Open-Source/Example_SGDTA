// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "StructUtils/InstancedStruct.h"

#include "ShooterUIMessage.generated.h"

/**
 * Container for a UI message to display on screen.
 */
USTRUCT(BlueprintType)
struct EXAMPLE_SGDTA_API FShooterUIMessage
{
	GENERATED_BODY()
public:

	FShooterUIMessage() = default;

	/** The message to display on screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;

	/** The message's duration on screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 5.0f;

	/** Optional metadata regarding this message. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInstancedStruct Metadata;
};