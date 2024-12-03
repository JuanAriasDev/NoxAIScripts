// Fill out your copyright notice in the Description page of Project Settings.


#include "RejectionController.h"
#include "BehaviorTree/BlackboardComponent.h"

ARejectionController::ARejectionController()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
}

void ARejectionController::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsBool("IsSet", false);
	//Blackboard->SetValueAsBool("IsLookingAtMe", false);
	Blackboard->SetValueAsInt("CurrentPointIndex", 0);
	Blackboard->SetValueAsVector("Direction", FVector::ZeroVector);
	Blackboard->SetValueAsFloat("CurrentDistance", 0.f);
	Blackboard->SetValueAsFloat("TotalDistance", 0.f);

	Init();
}

void ARejectionController::Tick(float DeltaTime)
{
}

void ARejectionController::Init_Implementation()
{

}