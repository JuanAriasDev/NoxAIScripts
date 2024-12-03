// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfinderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rejection/RejectionCharacter.h"
#include "Rejection/RejectionController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPathfinderComponent::UPathfinderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	// ...
}


// Called when the game starts
void UPathfinderComponent::BeginPlay()
{
	Super::BeginPlay();

	m_fCurrentTimer = m_fTimer;
}


// Called every frame
void UPathfinderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//DrawDebugSphere(GetWorld(), FVector::ZeroVector, 20, 16, FColor::Yellow, false, -1.f);

	if (!m_pNoxCharacter)
	{
		m_pNoxCharacter = Cast<ANoxPlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ANoxCharacter::StaticClass()));
		m_vEndLocation = m_pNoxCharacter->GetActorLocation();
	}

	if (m_fCurrentTimer >= m_fTimer)
	{
		if (m_pNoxCharacter && m_pGrid)
		{
			ARejectionController* pController = Cast<ARejectionController>(Cast<ARejectionCharacter>(GetOwner())->GetController());
			pController->GetBlackboardComponent()->SetValueAsBool("IsSet", false);

			m_vEndTile = m_pGrid->GetClosestTile(m_vEndLocation);
			m_vStartTile = m_pGrid->GetClosestTile(GetOwner()->GetActorLocation());
			InitPathfinding(m_vStartTile, m_vEndTile, m_pStartTile, m_pTargetTile);
			m_pCurrentTile = m_pStartTile;

			while (!bPathIsFinished)
			{
				if (m_vOpenNodes.Num() > 0)
				{
					PathfindingStep(m_pTargetTile, m_vOpenNodes, m_vClosedNodes);
				}
				else
				{
					return;
				}

				/*for (APathfindingGrid::Tile*& tile : m_vOpenNodes)
				{
					DrawDebugSphere(GetWorld(), tile->m_vWorldLocation, 20, 16, FColor::Blue, false, -1.f);
				}

				for (APathfindingGrid::Tile*& tile : m_vClosedNodes)
				{
					DrawDebugSphere(GetWorld(), tile->m_vWorldLocation, 20, 16, FColor::Red, false, -1.f);
				}*/

				/*DrawDebugBox(GetWorld(), m_pTargetTile->m_vWorldLocation, FVector(20, 20, 20), FColor::White);
				DrawDebugBox(GetWorld(), m_pStartTile->m_vWorldLocation, FVector(20, 20, 20), FColor::Green);
				DrawDebugBox(GetWorld(), m_pCurrentTile->m_vWorldLocation, FVector(20, 20, 20), FColor::Yellow);*/
			}

			if (m_tPath.Num() != 0)
			{
				m_tPath.Empty();
			}

			if (m_tPath.Num() == 0)
			{
				FTile* iter = m_pTargetTile;
				while (iter->m_vParentTile != FVector2D(-1, -1))
				{
					m_tPath.AddUnique(iter);
					iter = m_pGrid->m_mTiles.Find(iter->m_vParentTile);
				}
				m_tPath.AddUnique(m_pStartTile);
			}

			for (int32 i = 0; i < m_tPath.Num(); ++i)
			{
				DrawDebugSphere(GetWorld(), m_tPath[i]->m_vWorldLocation, 20, 16, FColor::Green, false, -1.f);
			}
		}
		m_fCurrentTimer = 0.f;
	}
	else
	{
		m_fCurrentTimer += DeltaTime;
	}
	
	/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, m_vStartTile.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, m_vEndTile.ToString());*/
	
	//FindPathToTarget(m_vStartTile, m_vEndTile);
}


// Calcula los vecinos del player
TArray<FVector2D> UPathfinderComponent::GetTileNeighbors(const FVector2D& _pTile)
{
	TArray<FVector2D> outNeighbors;
	TArray<FVector2D> possibleNeighbors = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1} };

	for (FVector2D& vec : possibleNeighbors)
	{
		FTile* tile = m_pGrid->m_mTiles.Find(_pTile + vec);

		if (tile)
		{
			if (tile->m_tGroundType != GroundTypes::GTNone && tile->m_tGroundType != GroundTypes::GTImpossible)
			{
				outNeighbors.AddUnique(_pTile + vec);
			}
		}
	}

	return outNeighbors;
}

void UPathfinderComponent::FindPathToTarget(FVector2D _startTile, FVector2D _targetTile)
{
	// OTRA PRUEBA PATHFINDING
	FTile* pStartTile = nullptr;
	FTile* pTargetTile = nullptr;
	InitPathfinding(_startTile, _targetTile, pStartTile, pTargetTile);

	while (m_vOpenNodes.Num() != 0 && m_vOpenNodes.Num() < m_pGrid->m_mTiles.Num())
	{
		PathfindingStep(pTargetTile, m_vOpenNodes, m_vClosedNodes);
	}
}

bool UPathfinderComponent::InitPathfinding(FVector2D _startTile, FVector2D _targetTile,
	FTile*& pStartTile_, FTile*& pTargetTile_)
{
	m_vClosedNodes.Empty();
	m_vOpenNodes.Empty();
	bPathIsFinished = false;

	pStartTile_ = m_pGrid->m_mTiles.Find(_startTile);
	pTargetTile_ = m_pGrid->m_mTiles.Find(_targetTile);

	if (!pTargetTile_ ||
		pTargetTile_->m_tGroundType == GroundTypes::GTImpossible ||
		pTargetTile_->m_tGroundType == GroundTypes::GTNone)
	{
		return false;
	}

	if (!pStartTile_ || _startTile == _targetTile)
	{
		return false;
	}

	m_pGrid->m_mTiles.GetKeys(m_vIndexes);

	for (FVector2D& index : m_vIndexes)
	{
		FTile* tile = m_pGrid->m_mTiles.Find(index);
		tile->m_gCost = 9999;
		tile->m_hCost = 9999;
		tile->m_fCost = 9999;
		tile->m_vParentTile = { -1, -1 };
	}

	pStartTile_->m_gCost = 0;
	pStartTile_->m_hCost = abs(pTargetTile_->m_vGridIndex.X - pStartTile_->m_vGridIndex.X)
		+ abs(pTargetTile_->m_vGridIndex.Y - pStartTile_->m_vGridIndex.Y);
	pStartTile_->m_fCost = pStartTile_->m_hCost;
	pStartTile_->m_vParentTile = FVector2D(-1, -1);

	m_vOpenNodes.AddUnique(pStartTile_);

	return true;
}

bool UPathfinderComponent::PathfindingStep(FTile* _pTargetTile, TArray<FTile*>& _vOpenedTiles_,
	TArray<FTile*>& _vClosedTile_)
{
	FTile* currentTile = _vOpenedTiles_.Pop(0);
	_vClosedTile_.AddUnique(currentTile);
	m_pCurrentTile = currentTile;

	for (FVector2D& index : GetTileNeighbors(currentTile->m_vGridIndex))
	{
		FTile* auxTile = m_pGrid->m_mTiles.Find(index);
		if (_vClosedTile_.Contains(auxTile))
		{
			continue;
		}
		int iBufferG = 0;
		if (auxTile->m_vParentTile != FVector2D(-1, -1))
		{
			iBufferG = m_pGrid->m_mTiles.Find(auxTile->m_vParentTile)->m_gCost + 1;
		}
		int iBufferH = GetEstimatedCostToTarget(auxTile->m_vGridIndex, _pTargetTile->m_vGridIndex);
		int iBufferF = iBufferH + iBufferG;
		if (auxTile->m_vParentTile != FVector2D(-1, -1))
		{
			if (iBufferF > auxTile->m_fCost)
			{
				continue;
			}
		}
		auxTile->m_gCost = iBufferG;
		auxTile->m_hCost = iBufferH;
		auxTile->m_fCost = iBufferF;
		auxTile->m_vParentTile = currentTile->m_vGridIndex;
		_vOpenedTiles_.AddUnique(auxTile);
	}
	_vOpenedTiles_.Sort([](const FTile& A, const FTile& B) -> bool
		{
			return A.m_fCost > B.m_fCost;
		});

	if (_pTargetTile->m_vParentTile != FVector2D(-1, -1))
	{
		if (_vOpenedTiles_.Num() == 0)
		{
			bPathIsFinished = true;
		}
		else if (_pTargetTile->m_fCost < _vOpenedTiles_[0]->m_fCost)
		{
			bPathIsFinished = true;
			return false;
		}
	}
	return true;
}

int UPathfinderComponent::GetEstimatedCostToTarget(FVector2D _currentTile, FVector2D _targetTile)
{
	FVector2D diff = _targetTile - _currentTile;
	int cost = diff.X * diff.X + diff.Y * diff.Y;
	//int cost = FMath::RoundHalfFromZero(FMath::Abs(diff.X) + FMath::Abs(diff.Y));

	return cost;
}
