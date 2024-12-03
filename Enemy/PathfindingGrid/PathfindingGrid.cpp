// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingGrid.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../NoxCharacter.h"

// Sets default values
APathfindingGrid::APathfindingGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = m_pScene;

	m_pBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	m_pBillboard->SetupAttachment(m_pScene);

	m_pTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	m_pTriggerBox->SetupAttachment(m_pScene);
}

// Called when the game starts or when spawned
void APathfindingGrid::BeginPlay()
{
	Super::BeginPlay();
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 20, 20, FColor::Blue, true);
	GenerateMapData();
}

void APathfindingGrid::OnConstruction(const FTransform& Transform)
{
	// Grid Debug
	FlushPersistentDebugLines(GetWorld());

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(m_vGridSizeWorld.X * 0.5f, m_vGridSizeWorld.Y * 0.5f, 5), GetActorRotation().Quaternion(),
		m_cGridColor, true, 1000000, 0, 10);

	DrawDebugBox(GetWorld(), GetGridBottomLeft(), FVector(20, 20, 5), GetActorQuat(), 
		m_cBottomLeftColor, true, 1, 0, 10);

	DrawDebugLine(GetWorld(), GetGridBottomLeft(), GetGridBottomLeft() + GetActorRightVector() * 100.f, FColor::Green, false, 1000000, 0, 10.f);
	DrawDebugLine(GetWorld(), GetGridBottomLeft(), GetGridBottomLeft() + GetActorForwardVector() * 100.f, FColor::Red, false, 1000000, 0, 10.f);

	GenerateMapData();

	if (m_bIsDebugging)
	{
		DrawTiles();
	}
}

// Called every frame
void APathfindingGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* NoxCharacter = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (NoxCharacter && m_pTriggerBox->IsOverlappingActor(NoxCharacter))
	{
		m_bPlayerInGrid = true;
	}
	else
	{
		m_bPlayerInGrid = false;
	}
}

FVector APathfindingGrid::GetGridBottomLeft()
{
	// Get location of the bottom left part of the Grid
	FVector bottomLeftPos, xFactor, yFactor;

	xFactor = GetActorForwardVector() * (m_vGridSizeWorld.X / 2.f);
	xFactor = GetActorLocation() - xFactor;

	yFactor = GetActorRightVector() * (m_vGridSizeWorld.Y / 2.f);
	bottomLeftPos = xFactor - yFactor;

	return bottomLeftPos;
}

void APathfindingGrid::SetTileNumber()
{
	// Set the total number of tiles in the grid
	FVector2D auxVector;
	auxVector = m_vGridSizeWorld / m_fTileSize;

	m_iX = FMath::Floor(auxVector.X);
	m_iY = FMath::Floor(auxVector.Y);
}

bool APathfindingGrid::MySphereTrace(AActor*& _actorHit, FVector _tilePos, ECollisionChannel _traceChannel)
{
	// Sphere trace to detect if the grid collides with any walkable terrain or obstacle
	float radius = m_fTileSize - m_fTileSizeOffset;
	FHitResult hitResult;
	TArray<AActor*> toIgnore;

	FCollisionShape sphereShape = FCollisionShape::MakeSphere(radius);

	ETraceTypeQuery channel = UEngineTypes::ConvertToTraceType(_traceChannel);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), _tilePos, _tilePos, radius, channel, false, toIgnore,
		EDrawDebugTrace::None, hitResult, true);

	_actorHit = hitResult.GetActor();

	return bHit;
}

void APathfindingGrid::SetTilePos(FVector& pos_, int x, int y)
{
	// Set the tile world location
	FVector xFactor;
	FVector yFactor;
	xFactor = GetActorForwardVector() * (x * m_fTileSize + m_fTileSize * 0.5f);
	yFactor = GetActorRightVector() * (y * m_fTileSize + m_fTileSize * 0.5f);

	pos_ = GetGridBottomLeft() + xFactor + yFactor;
}

void APathfindingGrid::GenerateMapData()
{
	// Save in a MAP all the tiles created

	SetTileNumber();

	FVector tilePos = FVector::ZeroVector;

	AActor* actorHit;

	for (int x = 0; x < m_iX; x++)
	{
		for (int y = 0; y < m_iY; y++)
		{
			SetTilePos(tilePos, x, y);

			// Check if is ground
			if (MySphereTrace(actorHit, tilePos, ECollisionChannel::ECC_GameTraceChannel2))
			{
				// Check if is obstacle
				if (MySphereTrace(actorHit, tilePos, ECollisionChannel::ECC_GameTraceChannel3))
				{
					// TODO: Cast to obstacle BP
					AddTile(x, y, tilePos, GTImpossible); // GTElQueVenga en obstacle BP - Hacer componente obstaculo?
				}
				else
				{
					AddTile(x, y, tilePos, GTWalkable);
					//DrawDebugSphere(GetWorld(), tilePos, 20, 16, FColor::Black, true);
				}
			}
			else
			{
				AddTile(x, y, tilePos, GTNone);
			}
		}
	}

	m_pTriggerBox->SetBoxExtent(
		{ m_vGridSizeWorld.X / 2.f, 
		m_vGridSizeWorld.Y / 2.f, 
		m_pTriggerBox->GetScaledBoxExtent().Z }, 
		true);
}

void APathfindingGrid::AddTile(int _x, int _y, FVector _tilePos, GroundTypes _groundType)
{
	FTile tile = FTile();
	tile.m_vWorldLocation = _tilePos;
	tile.m_vGridIndex.Set(_x, _y);
	tile.m_tGroundType = _groundType;
	tile.m_iTileCost = GetTileCost(_groundType);

	m_mTiles.Add(tile.m_vGridIndex, tile);
	//DrawDebugSphere(GetWorld(), tile.m_vWorldLocation, 20, 16, FColor::Black, true, 10000.f);
}

void APathfindingGrid::DrawTiles()
{
	// Debug of all tiles as planes
	TArray<FVector2D> keys;
	m_mTiles.GetKeys(keys);
	FTile* tile;

	for (FVector2D& index : keys)
	{
		tile = m_mTiles.Find(index);
		MyDrawDebugPlane(tile->m_vWorldLocation, tile->m_tGroundType);
	}
}

void APathfindingGrid::MyDrawDebugPlane(FVector _location, GroundTypes _groundType)
{
	FColor tileColor;

	switch (_groundType)
	{
	case GTWalkable:
		tileColor = FColor::Green;
		break;
	case GTImpossible:
		tileColor = FColor::Red;
		break;
	case GTNone:
		tileColor = FColor(0, 0, 0, 0);
		break;
	}

	FPlane myPlane = FPlane(_location, RootComponent->GetUpVector().GetSafeNormal());

	DrawDebugSolidPlane(GetWorld(), myPlane, _location, m_fTileSize * 0.5f - m_fTileSizeOffset * 0.5f, tileColor, true, 1, 0);
}

int APathfindingGrid::GetTileCost(GroundTypes _groundType)
{
	switch (_groundType)
	{
	case GTWalkable:
		return 1;
		break;
	case GTImpossible:
		return 999;
		break;
	case GTNone:
		return 999;
		break;
	default:
		return 999;
		break;
	}
	return INT_MAX;
}

FVector2D APathfindingGrid::GetClosestTile(FVector _loc)
{
	FVector2D closestTile;
	float minDist = 0;
	float dist = 0;

	TArray<FVector2D> keys;
	m_mTiles.GetKeys(keys);
	FTile* tile = m_mTiles.Find(keys[0]);
	closestTile = keys[0];

	dist = FVector::DistSquared(tile->m_vWorldLocation, _loc);
	minDist = dist;

	for (FVector2D& index : keys)
	{
		tile = m_mTiles.Find(index);
		
		dist = FVector::DistSquared(tile->m_vWorldLocation, _loc);

		if (dist < minDist)
		{
			closestTile = index;
			minDist = dist;
		}
	}

	return closestTile;
}
