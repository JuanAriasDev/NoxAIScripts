// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Math/Color.h"
#include "Math/Vector2D.h"
#include "PathfindingGrid.generated.h"

UENUM(BlueprintType)
enum GroundTypes
{
	GTNone,
	GTWalkable,
	GTImpossible
};

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

	GroundTypes m_tGroundType;
	FVector m_vWorldLocation;
	FVector2D m_vGridIndex;
	int m_iTileCost;
	int m_fCost;
	int m_gCost;
	int m_hCost;
	FVector2D m_vParentTile;
};

UCLASS()
class PROJECTNOX_API APathfindingGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathfindingGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_pScene;

	UPROPERTY(BlueprintReadWrite)
	UBillboardComponent* m_pBillboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D m_vGridSizeWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fTileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fTileSizeOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor m_cGridColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor m_cBottomLeftColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_pTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_bIsDebugging = false;

private:

	int m_iX, m_iY;

public:
	TMap<FVector2D, FTile> m_mTiles;

	bool m_bPlayerInGrid = false;

public:

	FVector GetGridBottomLeft();

	void SetTileNumber();

	FVector2D GetGridSize() { return m_vGridSizeWorld; };
	float GetTileSize() { return m_fTileSize + m_fTileSizeOffset; }

	bool MySphereTrace(AActor*& _actorHit, FVector _tilePos, ECollisionChannel _traceChannel);

	void SetTilePos(FVector& _pos, int x, int y);

	UFUNCTION(BlueprintCallable)
	void GenerateMapData();

	void AddTile(int _x, int _y, FVector _tilePos, GroundTypes _groundType);

	UFUNCTION(BlueprintCallable)
	void DrawTiles();

	void MyDrawDebugPlane(FVector _location, GroundTypes _groundType);

	int GetTileCost(GroundTypes _groundType);

	FVector2D GetClosestTile(FVector _loc);
};
