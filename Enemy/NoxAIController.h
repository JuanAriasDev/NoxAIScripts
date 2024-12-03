// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NoxAIController.generated.h"

#define DEBUGING_AI

/**
 * 
 */
class UBehaviorTree;
class UBlackboardComponent;
class UBlackboardData;
UCLASS()
class PROJECTNOX_API ANoxAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANoxAIController();

	UFUNCTION()
		void AbandonmentPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;
	TArray<AActor*>& GetSeenActors() { return m_tSeenActors; }
	TArray<AActor*>& GetHeardActors() { return m_tHeardActors; }

protected:
	virtual void BeginPlay() override;
private:

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
		UBehaviorTree* m_pBehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behaviour")
		UBlackboardData* m_pBlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
		class UAIPerceptionComponent* m_pPerceptionCmp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
		class UAISenseConfig_Sight* m_pSight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Senses")
		class UAISenseConfig_Hearing* m_pHearing;
protected:
private:
	bool wasSeen = false;
	bool wasHeard = false;

	TArray<AActor*> m_tSeenActors;
	TArray<AActor*> m_tHeardActors;
};
