// Copyright Start Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"

#include "ExampleDataAsset.generated.h"

/**
 * This is a simple data asset for this example project to help showcase different features.
 * Intended as the base class for all UDataAsset types within the project.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "Example SGDTA")
class EXAMPLE_SGDTA_API UExampleDataAsset : public UDataAsset
{
	GENERATED_BODY()
};
