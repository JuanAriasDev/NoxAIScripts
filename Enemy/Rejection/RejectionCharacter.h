// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NoxAICharacter.h"
#include "../PathfinderComponent.h"
#include "Components/CapsuleComponent.h"
#include "RejectionCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNOX_API ARejectionCharacter : public ANoxAICharacter
{
	GENERATED_BODY()

public:

	ARejectionCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPathfinderComponent* m_pPathfinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* m_pVisionCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* m_pRealCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fAnxietyIncrementTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_fKillDist = 0.f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Looking at Rejection")
	void IsMoving();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Not looking at Rejection")
	void IsNotMoving();

	UFUNCTION(BlueprintImplementableEvent, Category = "Kill player")
	void KillPlayer();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Kill dummy")
	void KillDummy();

	bool m_bIsMoving = false;
};
