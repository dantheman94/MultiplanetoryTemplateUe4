// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceFighterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLANETTEMPLATE_API ASpaceFighterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Hud")
		float GetObjectScreenRadius(AActor* InActor);

};
