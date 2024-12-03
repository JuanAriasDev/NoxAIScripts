// Fill out your copyright notice in the Description page of Project Settings.


#include "NoxAIController.h"
#include "EngineGlobals.h"
#include "Engine.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "../NoxPlayerController.h"

ANoxAIController::ANoxAIController()
{	
	SetGenericTeamId(FGenericTeamId(1));
	m_pPerceptionCmp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	m_pSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	m_pHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	m_pPerceptionCmp->ConfigureSense(*m_pSight);
	m_pPerceptionCmp->ConfigureSense(*m_pHearing);
	m_pPerceptionCmp->SetDominantSense(m_pHearing->GetSenseImplementation());
	m_pPerceptionCmp->OnPerceptionUpdated.AddDynamic(this, &ANoxAIController::AbandonmentPerceptionUpdated);
}

void ANoxAIController::BeginPlay()
{
	Super::BeginPlay();

	if (m_pBehaviorTree)
	{
		UseBlackboard(m_pBlackboardData, Blackboard);
		RunBehaviorTree(m_pBehaviorTree);
	}
}
ETeamAttitude::Type ANoxAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* pChar = Cast<APawn>(&Other);
	if (pChar)
	{
		const IGenericTeamAgentInterface* pTeamAgent = Cast<IGenericTeamAgentInterface>(pChar->GetController());
		if (pTeamAgent)
		{
			if (pTeamAgent->GetGenericTeamId() != GetGenericTeamId())
			{
				return ETeamAttitude::Hostile;
			}
		}
	}
	return ETeamAttitude::Neutral;
}

void ANoxAIController::AbandonmentPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> tHeardActors;
	m_pPerceptionCmp->GetPerceivedHostileActorsBySense(UAISense_Hearing::StaticClass(), tHeardActors);
	m_tHeardActors = tHeardActors;
	if (tHeardActors.Num() != 0)
	{
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Blue, GetDebugName(tHeardActors[0]));
		}*/
		wasHeard = true;
	}
	else if(wasHeard)
	{
		wasHeard = false;

		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, TEXT("Stopped Being Heard"));
		//}
	}

	TArray<AActor*> tSeenActors;
	m_pPerceptionCmp->GetPerceivedHostileActorsBySense(UAISense_Sight::StaticClass(), tSeenActors);
	m_tSeenActors = tSeenActors;
	if (tSeenActors.Num() != 0)
	{
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, GetDebugName(tSeenActors[0]));
		}*/
		wasSeen = true;
	}
	else if(wasSeen)
	{
		wasSeen = false; 

		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Red, TEXT("Stopped Being Seen"));
		//}
	}
}
