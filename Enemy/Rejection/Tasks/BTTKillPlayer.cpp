// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTKillPlayer.h"
#include "../RejectionController.h"
#include "../RejectionCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../../NoxGameInstance.h"

UBTTKillPlayer::UBTTKillPlayer()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTKillPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTKillPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARejectionController* pController = Cast<ARejectionController>(OwnerComp.GetOwner());
	ARejectionCharacter* pRejectionChar = pController->GetPawn<ARejectionCharacter>();
	UBlackboardComponent* pBB = pController->GetBlackboardComponent();

	float distToKill = pBB->GetValueAsFloat("DistToKill");

	if (pRejectionChar->GetDistanceTo(pRejectionChar->m_pPathfinder->m_pNoxCharacter) < distToKill)
	{
		UNoxGameInstance* GI = Cast<UNoxGameInstance>(GetWorld()->GetGameInstance());

		if (GI)
		{
			pRejectionChar->KillPlayer();
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
