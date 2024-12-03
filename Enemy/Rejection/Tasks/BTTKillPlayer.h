// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTKillPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNOX_API UBTTKillPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTKillPlayer();

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
