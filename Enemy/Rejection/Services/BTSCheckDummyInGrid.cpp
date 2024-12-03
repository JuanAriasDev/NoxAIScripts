// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSCheckDummyInGrid.h"
#include "../RejectionCharacter.h"
#include "../RejectionController.h"
#include "Kismet/GameplayStatics.h"
#include "../../PathfindingGrid.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTSCheckDummyInGrid::UBTSCheckDummyInGrid()
{
	bNotifyTick = true;
}

void UBTSCheckDummyInGrid::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ARejectionController* pController = Cast<ARejectionController>(OwnerComp.GetOwner());
	ARejectionCharacter* pRejectionChar = pController->GetPawn<ARejectionCharacter>();
	UBlackboardComponent* pBB = pController->GetBlackboardComponent();

	UObject* oDummy = pBB->GetValueAsObject("Dummy");
	ANoxDummy* pDummy = Cast<ANoxDummy>(oDummy);

	APathfindingGrid* pGrid = pRejectionChar->m_pPathfinder->m_pGrid;

	if (m_tDummies.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANoxDummy::StaticClass(), m_tDummies);
	}

	for (AActor* dummy : m_tDummies)
	{
		ANoxDummy* currentDummy = Cast<ANoxDummy>(dummy);

		if (pRejectionChar && pRejectionChar->m_pPathfinder && pGrid && currentDummy->m_bIsPlaced && pGrid->m_pTriggerBox->IsOverlappingActor(dummy))
		{
			if (pDummy)
			{
				if (currentDummy->GetDistanceTo(pRejectionChar) < pDummy->GetDistanceTo(pRejectionChar))
				{
					pDummy = currentDummy;
				}
			}
			else
			{
				pDummy = currentDummy;
			}
		}
	}

	pBB->SetValueAsObject("Dummy", pDummy);
}