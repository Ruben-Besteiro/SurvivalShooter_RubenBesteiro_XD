#include "AI/SecondEnemyController.h"

#include "AI/SecondEnemyStates.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

ASecondEnemyController::ASecondEnemyController()
{
    //PrimaryActorTick.bCanEverTick = true;
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception");
}

void ASecondEnemyController::BeginPlay()
{
    Super::BeginPlay();
    RunBehaviorTree(BehaviorTree);
    
    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ASecondEnemyController::OnSensed);
    ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Wait));
}

void ASecondEnemyController::OnSensed(AActor* Actor, FAIStimulus Stimulus)
{
    // No queremos que los enemigos se peguen entre sí
    if (!Actor->ActorHasTag("Player")) return;
    
    auto SenseType = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
    
    ESecondEnemyStates CurrentState = static_cast<ESecondEnemyStates>(GetBlackboardComponent()->GetValueAsEnum("CurrentState2"));
    UE_LOG(LogTemp, Warning, TEXT("El estado actual es %d"), static_cast<int>(CurrentState));

    // Si el enemigo está huyendo, deja de sentir cosas
    if (CurrentState == ESecondEnemyStates::Flee) return;
    
    if (SenseType == UAISense_Sight::StaticClass() || SenseType == UAISense_Hearing::StaticClass())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            GetBlackboardComponent()->SetValueAsObject("Target", Actor);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Chase));
        }
    }
    else if (SenseType == UAISense_Damage::StaticClass())
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            GetBlackboardComponent()->SetValueAsObject("Target", Actor);
            ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Chase));
        }
    }
}

void ASecondEnemyController::ChangeEnemyState(uint8 State)
{
    GetBlackboardComponent()->SetValueAsEnum("CurrentState2", State);
    if (State == static_cast<uint8>(ESecondEnemyStates::Flee))
    {
        ClearFocus(EAIFocusPriority::Gameplay);
        GetBlackboardComponent()->ClearValue("Target");
    }
    bRotating = false;
}