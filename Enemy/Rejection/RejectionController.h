// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NoxAIController.h"
#include "../PathfindingGrid.h"
#include "RejectionController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum State
{
	Moving UMETA(DisplayName = "Player Not looking"),
	NotMoving UMETA(DisplayName = "Player Looking"),
	Num
};

UCLASS()
class PROJECTNOX_API ARejectionController : public ANoxAIController
{
	GENERATED_BODY()
	
public:

	ARejectionController();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Init();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	APathfindingGrid* m_pGrid;
};
