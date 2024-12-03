// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSCheckRejectionInFOV.h"
#include "../RejectionCharacter.h"
#include "../RejectionController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"

UBTSCheckRejectionInFOV::UBTSCheckRejectionInFOV()
{
	bNotifyTick = true;
}

void UBTSCheckRejectionInFOV::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARejectionController* pController = Cast<ARejectionController>(OwnerComp.GetOwner());
	ARejectionCharacter* pRejectionChar = pController->GetPawn<ARejectionCharacter>();
	UBlackboardComponent* pBB = pController->GetBlackboardComponent();

	// Anxiety Check
	if (pRejectionChar->m_pPathfinder->m_pNoxCharacter)
	{
		FVector MeToPlayer = pRejectionChar->GetActorLocation() - pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation();
		FVector ForwardVector = pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->GetForwardVector();

		MeToPlayer = MeToPlayer.GetSafeNormal();
		ForwardVector = ForwardVector.GetSafeNormal();
	
		bool anxietyFOVCheck = InRangeFOVX(ForwardVector, MeToPlayer, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->FieldOfView);
		anxietyFOVCheck = anxietyFOVCheck && InRangeFOVY(ForwardVector, MeToPlayer, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->FieldOfView);

		pBB->SetValueAsBool("InRangeForAnxiety", anxietyFOVCheck);
	
		// MovementCheck - Left
		FVector MeToPlayerLeft = pRejectionChar->GetActorLocation() - 
			pRejectionChar->GetActorRightVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleRadius() -
			pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation();
		MeToPlayerLeft = MeToPlayerLeft.GetSafeNormal();

		bool leftFOVCheck = InRangeFOVX(ForwardVector, MeToPlayerLeft, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->FieldOfView);
		pBB->SetValueAsBool("InRangeLeftMove", leftFOVCheck);

		// MovementCheck - Right
		FVector MeToPlayerRight = pRejectionChar->GetActorLocation() +
			pRejectionChar->GetActorRightVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleRadius() - 
			pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation();
		MeToPlayerRight = MeToPlayerRight.GetSafeNormal();

		bool rightFOVCheck = InRangeFOVX(ForwardVector, MeToPlayerRight, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->FieldOfView);
		pBB->SetValueAsBool("InRangeRightMove", rightFOVCheck);
		
		// MovementCheck - Up
		FVector MeToPlayerUp = pRejectionChar->GetActorLocation() +
			pRejectionChar->GetActorRightVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleHalfHeight() -
			pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation();
		MeToPlayerUp = MeToPlayerUp.GetSafeNormal();

		bool upFOVCheck = InRangeFOVY(ForwardVector, MeToPlayerUp, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->FieldOfView);
		pBB->SetValueAsBool("InRangeUpMove", upFOVCheck);

		// MovementCheck - Down
		FVector MeToPlayerDown = pRejectionChar->GetActorLocation() -
			pRejectionChar->GetActorRightVector() * pRejectionChar->m_pVisionCapsule->GetScaledCapsuleHalfHeight() -
			pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetActorLocation();
		MeToPlayerDown = MeToPlayerDown.GetSafeNormal();

		bool downFOVCheck = InRangeFOVY(ForwardVector, MeToPlayerDown, pRejectionChar->m_pPathfinder->m_pNoxCharacter->GetCameraComponent()->FieldOfView);
		pBB->SetValueAsBool("InRangeDownMove", downFOVCheck);
	}
}

bool UBTSCheckRejectionInFOV::InRangeFOVX(FVector& _v1, FVector& _v2, float fov)
{
	float fAngle1 = FMath::Atan2(_v1.Y, _v1.X) - FMath::Atan2(_v2.Y, _v2.X);

	if (fAngle1 < 0.f)
	{
		fAngle1 += 2.f * PI;
	}

	float fAngle2 = 2.f * PI - fAngle1;

	float fAngleTotal = fAngle1 < fAngle2 ? fAngle1 : fAngle2;
	float fHalfFOV = FMath::DegreesToRadians(fov) / 2.f;

	return fAngleTotal < fHalfFOV;
}

bool UBTSCheckRejectionInFOV::InRangeFOVY(FVector& _v1, FVector& _v2, float fov)
{
	float fAngle1 = FMath::Atan2(_v1.Z, _v1.X) - FMath::Atan2(_v2.Z, _v2.X);

	if (fAngle1 < 0.f)
	{
		fAngle1 += 2.f * PI;
	}

	float fAngle2 = 2.f * PI - fAngle1;

	float fAngleTotal = fAngle1 < fAngle2 ? fAngle1 : fAngle2;
	float fHalfFOV = FMath::DegreesToRadians(fov) / 2.f;

	return fAngleTotal < fHalfFOV;
}