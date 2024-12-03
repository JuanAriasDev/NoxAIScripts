// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNox/Characters/Enemies/NoxAICharacter.h"

// Sets default values
ANoxAICharacter::ANoxAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANoxAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANoxAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANoxAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

