// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSCheckMovement.h"
#include "../RejectionCharacter.h"
#include "../RejectionController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTSCheckMovement::UBTSCheckMovement()
{
	bNotifyTick = true;
}

void UBTSCheckMovement::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARejectionController* pController = Cast<ARejectionController>(OwnerComp.GetOwner());
	ARejectionCharacter* pRejectionChar = pController->GetPawn<ARejectionCharacter>();
	UBlackboardComponent* pBB = pController->GetBlackboardComponent();

	bool inFOVLeft = pBB->GetValueAsBool("InRangeLeftMove");
	bool inFOVRight = pBB->GetValueAsBool("InRangeRightMove");

	if (pRejectionChar && pRejectionChar->m_pPathfinder && pRejectionChar->m_pPathfinder->m_pGrid && pRejectionChar->m_pPathfinder->m_pGrid->m_bPlayerInGrid)
	{
		FHitResult CenterHit;
		FHitResult TopHit;
		FHitResult BotHit;
		FHitResult RightHit;
		FHitResult LeftHit;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(pRejectionChar->m_pPathfinder->m_pNoxCharacter);

		// Center
		GetWorld()->LineTraceSingleByChannel(CenterHit, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation(),
			pRejectionChar->m_pVisionCapsule->GetComponentLocation(), ECC_Pawn, CollisionParams);

		// Top
		FVector topEnd = pRejectionChar->m_pVisionCapsule->GetComponentLocation() +
			pRejectionChar->GetActorUpVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleHalfHeight();

		GetWorld()->LineTraceSingleByChannel(TopHit, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation(),
			topEnd, ECollisionChannel::ECC_GameTraceChannel7, CollisionParams);

		// Bot
		FVector botEnd = pRejectionChar->m_pVisionCapsule->GetComponentLocation() -
			pRejectionChar->GetActorUpVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleHalfHeight();

		GetWorld()->LineTraceSingleByChannel(BotHit, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation(),
			botEnd, ECollisionChannel::ECC_GameTraceChannel7, CollisionParams);

		// Left
		FVector leftEnd = pRejectionChar->m_pVisionCapsule->GetComponentLocation() -
			pRejectionChar->GetActorRightVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleRadius();

		GetWorld()->LineTraceSingleByChannel(LeftHit, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation(),
			leftEnd, ECollisionChannel::ECC_GameTraceChannel7, CollisionParams);

		// Right
		FVector rightEnd = pRejectionChar->m_pVisionCapsule->GetComponentLocation() +
			pRejectionChar->GetActorRightVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleRadius();

		GetWorld()->LineTraceSingleByChannel(RightHit, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation(),
			rightEnd, ECollisionChannel::ECC_GameTraceChannel7, CollisionParams);

		if (Cast<ARejectionCharacter>(CenterHit.GetActor()))
		{
			pBB->SetValueAsBool("TriggerAnxiety", true);
		}
		else
		{
			pBB->SetValueAsBool("TriggerAnxiety", false);
		}

		if (inFOVLeft || inFOVRight)
		{
			pBB->SetValueAsBool("ShouldMove", false);

			if (Cast<ARejectionCharacter>(TopHit.GetActor()) || Cast<ARejectionCharacter>(BotHit.GetActor()) ||
				Cast<ARejectionCharacter>(LeftHit.GetActor()) || Cast<ARejectionCharacter>(RightHit.GetActor())
				|| Cast<ARejectionCharacter>(CenterHit.GetActor()))
			{
				pBB->SetValueAsBool("ShouldMove", false);
				
				if (pRejectionChar->m_bIsMoving)
				{
					pRejectionChar->IsNotMoving();
					pRejectionChar->m_bIsMoving = false;
				}
			}
			else
			{
				pBB->SetValueAsBool("ShouldMove", true);

				FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(pRejectionChar->GetActorLocation(),
					pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation());

				FRotator newRot = FRotator(pRejectionChar->GetActorRotation().Pitch, lookAtRot.Yaw, pRejectionChar->GetActorRotation().Roll);

				pRejectionChar->SetActorRotation(newRot);

				if (!pRejectionChar->m_bIsMoving)
				{
					pRejectionChar->IsMoving();
					pRejectionChar->m_bIsMoving = true;
				}
			}
		}
		else
		{
			pBB->SetValueAsBool("ShouldMove", true);

			if (!pRejectionChar->m_bIsMoving)
			{
				pRejectionChar->IsMoving();
				pRejectionChar->m_bIsMoving = true;
			}
		}
	}
	else
	{
		pBB->SetValueAsBool("ShouldMove", false);
		pBB->SetValueAsBool("TriggerAnxiety", false);

		if (pRejectionChar->m_bIsMoving)
		{
			pRejectionChar->IsNotMoving();
			pRejectionChar->m_bIsMoving = false;
		}

		if (pRejectionChar->m_pPathfinder->m_pNoxCharacter)
		{
			pRejectionChar->m_pPathfinder->m_pNoxCharacter->AnxietyComp->bLooksToRejection = false;
		}
	}
}