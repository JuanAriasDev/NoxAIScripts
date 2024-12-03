// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSCheckRejectionInFOV.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNOX_API UBTSCheckRejectionInFOV : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTSCheckRejectionInFOV();

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	
	bool InRangeFOVX(FVector& _v1, FVector& _v2, float fov);
	bool InRangeFOVY(FVector& _v1, FVector& _v2, float fov);
};
