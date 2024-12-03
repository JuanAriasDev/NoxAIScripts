// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../NoxDummy.h"
#include "BehaviorTree/BTService.h"
#include "BTSCheckDummyInGrid.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNOX_API UBTSCheckDummyInGrid : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTSCheckDummyInGrid();

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	TArray<AActor*> m_tDummies;
};
