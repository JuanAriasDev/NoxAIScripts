// Fill out your copyright notice in the Description page of Project Settings.

#include "RejectionCharacter.h"
#include "RejectionController.h"
#include "BehaviorTree/BlackboardComponent.h"

ARejectionCharacter::ARejectionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	m_pPathfinder = CreateDefaultSubobject<UPathfinderComponent>(TEXT("Pathfinder"));

	m_pVisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("VisionCapsuleComponent"));
	m_pVisionCapsule->SetupAttachment(RootComponent);

	m_pRealCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RealCapsuleComponent"));
	m_pRealCapsule->SetupAttachment(RootComponent);

	//Default value.
	m_fAnxietyIncrementTime = 0.5f;
}

void ARejectionCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ARejectionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
