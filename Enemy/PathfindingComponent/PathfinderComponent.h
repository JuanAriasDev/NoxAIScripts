// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingGrid.h"
#include "../NoxPlayerCharacter.h"
#include "PathfinderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNOX_API UPathfinderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	TArray<FTile*> m_vOpenNodes;
	TArray<FTile*> m_vClosedNodes;

	TArray<FVector2D> m_vIndexes;

public:
	
	FVector2D m_vStartTile;
	FVector2D m_vEndTile;
	FVector2D m_vLastTargetTile;
	ANoxPlayerCharacter* m_pNoxCharacter;

	FVector m_vEndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APathfindingGrid* m_pGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fTimer = 1.f;

	float m_fCurrentTimer = 0.f;

private:
	TArray<FVector2D> GetTileNeighbors(const FVector2D& _pTile);

public:

	void FindPathToTarget(FVector2D _startTile, FVector2D _targetTile);

	int GetEstimatedCostToTarget(FVector2D _currentTile, FVector2D _targetTile);

	bool InitPathfinding(FVector2D _startTile, FVector2D _targetTile, 
		FTile*& pStartTile_, FTile*& pTargetTile_);
	bool PathfindingStep(FTile* _pTargetTile, TArray<FTile*>& _vOpenedTiles_,
		TArray<FTile*>& _vClosedTile_);

	bool bHaComenzado = false;
	FTile* m_pStartTile;
	FTile* m_pTargetTile;
	FTile* m_pCurrentTile;

	bool bPathIsFinished = false;

	TArray<FTile*> m_tPath;
};
