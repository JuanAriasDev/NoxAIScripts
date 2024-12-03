// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTKillDummy.h"
#include "../RejectionController.h"
#include "../RejectionCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTKillDummy::UBTTKillDummy()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTKillDummy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTKillDummy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARejectionController* pController = Cast<ARejectionController>(OwnerComp.GetOwner());
	ARejectionCharacter* pRejectionChar = pController->GetPawn<ARejectionCharacter>();
	UBlackboardComponent* pBB = pController->GetBlackboardComponent();

	UObject* oDummy = pBB->GetValueAsObject("Dummy");
	AActor* pDummy = Cast<AActor>(oDummy);

	float distToKill = pBB->GetValueAsFloat("DistToKill");

	if (pDummy && pRejectionChar->GetDistanceTo(pDummy) < distToKill)
	{
		pRejectionChar->KillDummy();
		pDummy->SetActorHiddenInGame(true);
		pDummy->SetActorLocation({ 100000.f, 100000.f, 100000.f });
		pBB->SetValueAsObject("Dummy", pDummy);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}