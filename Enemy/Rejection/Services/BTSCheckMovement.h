// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSCheckMovement.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNOX_API UBTSCheckMovement : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTSCheckMovement();

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
