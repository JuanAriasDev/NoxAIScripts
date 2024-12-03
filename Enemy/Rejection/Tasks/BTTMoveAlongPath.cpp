// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTMoveAlongPath.h"
#include "../RejectionController.h"
#include "../RejectionCharacter.h"
#include "../../../NoxCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

UBTTMoveAlongPath::UBTTMoveAlongPath()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTMoveAlongPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTMoveAlongPath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARejectionController* pController = Cast<ARejectionController>(OwnerComp.GetOwner());
	ARejectionCharacter* pChar = pController->GetPawn<ARejectionCharacter>();
	UBlackboardComponent* pBB = pController->GetBlackboardComponent();

	bool bIsSet = pBB->GetValueAsBool("IsSet");
	int iCurrentPoint = pBB->GetValueAsInt("CurrentPointIndex");
	float fCurrentDistance = pBB->GetValueAsFloat("CurrentDistance");
	float fTotalDistance = pBB->GetValueAsFloat("TotalDistance");
	float fSpeed = pBB->GetValueAsFloat("Speed");
	bool bTriggerAnxiety = pBB->GetValueAsBool("TriggerAnxiety");
	bool bShouldMove = pBB->GetValueAsBool("ShouldMove");
	FVector vStartLocation = pBB->GetValueAsVector("StartLocation");
	bool inFovForAnxiety = pBB->GetValueAsBool("InRangeForAnxiety");
	UObject* oDummy = pBB->GetValueAsObject("Dummy");
	AActor* pDummy = Cast<AActor>(oDummy);

	FVector newLoc = pChar->GetActorLocation();

	if (pChar->m_pPathfinder->m_pNoxCharacter)
	{
		if (pDummy && pChar->GetDistanceTo(pDummy) < pChar->GetDistanceTo(pChar->m_pPathfinder->m_pNoxCharacter))
		{
			pChar->m_pPathfinder->m_vEndLocation = pDummy->GetActorLocation();
		}
		else
		{
			pChar->m_pPathfinder->m_vEndLocation = pChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation();
		}
	}
	
	if (pChar->m_pPathfinder->m_tPath.Num() != 0)
	{
		if (!bIsSet)
		{
			iCurrentPoint = pChar->m_pPathfinder->m_tPath.Num() - 2;
			pBB->SetValueAsInt("CurrentPointIndex", iCurrentPoint);
			pBB->SetValueAsBool("IsSet", true);
		} 
	
		if (iCurrentPoint >= 0)
		{
			FVector vDirection = pChar->m_pPathfinder->m_tPath[iCurrentPoint]->m_vWorldLocation - vStartLocation;
			fTotalDistance = vDirection.Size();
			pBB->SetValueAsFloat("TotalDistance", fTotalDistance);
			vDirection = vDirection.GetSafeNormal();
			pBB->SetValueAsVector("Direction", vDirection);

			if (fCurrentDistance < fTotalDistance)
			{
				if (bShouldMove)
				{
					newLoc += vDirection * fSpeed * DeltaSeconds;
					pChar->SetActorLocation(newLoc);
					fCurrentDistance = (newLoc - vStartLocation).Size();
					pBB->SetValueAsFloat("CurrentDistance", fCurrentDistance);
				}
			}
			else
			{
				vStartLocation = pChar->m_pPathfinder->m_tPath[iCurrentPoint]->m_vWorldLocation;
				pBB->SetValueAsVector("StartLocation", pChar->GetActorLocation());
				pChar->SetActorLocation(pChar->GetActorLocation());
				fCurrentDistance = 0.f;
				pBB->SetValueAsFloat("CurrentDistance", fCurrentDistance);
				iCurrentPoint--;
				pBB->SetValueAsInt("CurrentPointIndex", iCurrentPoint);
			}
		}
	}
	else
	{
		pBB->SetValueAsBool("IsSet", false);
	}

	if (pChar->m_pPathfinder->m_pNoxCharacter)
	{
		if (inFovForAnxiety)
		{
			if (pChar->m_pPathfinder->m_pNoxCharacter)
			{
				if (bTriggerAnxiety)
				{
					pChar->m_pPathfinder->m_pNoxCharacter->AnxietyComp->AnxietyIncrementTime = pChar->m_fAnxietyIncrementTime;
					pChar->m_pPathfinder->m_pNoxCharacter->AnxietyComp->bLooksToRejection = true;
				}
				else
				{
					pChar->m_pPathfinder->m_pNoxCharacter->AnxietyComp->bLooksToRejection = false;
				}
			}
		}
		else
		{
			FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(pChar->GetActorLocation(),
				pChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation());

			FVector vDirection = pChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation() - pChar->GetActorLocation();
			vDirection.Normalize();

			vDirection = UKismetMathLibrary::ProjectVectorOnToPlane(vDirection, pChar->GetActorUpVector());
			vDirection.Normalize();

			FRotator newRot = UKismetMathLibrary::MakeRotFromXZ(vDirection, pChar->GetActorUpVector());

			pChar->SetActorRotation(newRot);

			if (pChar->m_pPathfinder->m_pNoxCharacter)
			{
				pChar->m_pPathfinder->m_pNoxCharacter->AnxietyComp->bLooksToRejection = false;
			}
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}