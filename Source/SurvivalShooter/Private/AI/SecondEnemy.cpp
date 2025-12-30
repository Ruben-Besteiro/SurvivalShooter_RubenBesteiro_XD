// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SecondEnemy.h"
#include "BrainComponent.h"
#include "AI/SecondEnemyController.h"
#include "AI/SecondEnemyStates.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/ShooterController.h"

// Sets default values
ASecondEnemy::ASecondEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackPoint = CreateDefaultSubobject<USceneComponent>("AttackPoint");
	AttackPoint->SetupAttachment(RootComponent); //Pongo el empty como hijo del comp. raíz.
}

// Called when the game starts or when spawned
void ASecondEnemy::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Hola desde el enemigo"));
	LastRotation = GetActorRotation();
}

void ASecondEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculamos la velocidad de rotación
	FRotator CurrentRotation = GetActorRotation();
	FRotator DeltaRotation = (CurrentRotation - LastRotation).GetNormalized();
	RotationSpeed = DeltaRotation.Yaw / DeltaTime;
	LastRotation = CurrentRotation;

	ASecondEnemyController* AIController = Cast<ASecondEnemyController>(GetController());
	if (!AIController) return;
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (!BBComp) return;

	ESecondEnemyStates CurrentState = static_cast<ESecondEnemyStates>(BBComp->GetValueAsEnum("CurrentState2"));

	if (CurrentState == ESecondEnemyStates::Chase)
	{
		AActor* Target = Cast<AActor>(BBComp->GetValueAsObject("Target"));
		if (!Target) return;

		FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator TargetRot = Direction.Rotation();
		SetActorRotation(TargetRot);
	}
}



float ASecondEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= DamageAmount;

	ASecondEnemyController* AIController = Cast<ASecondEnemyController>(GetController());
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	ESecondEnemyStates CurrentState = static_cast<ESecondEnemyStates>(BBComp->GetValueAsEnum("CurrentState2"));
	
	if (Health <= 40 && CurrentState != ESecondEnemyStates::Flee)
	{
		AIController->ChangeEnemyState(static_cast<uint8>(ESecondEnemyStates::Flee));
		int State = BBComp->GetValueAsEnum("CurrentState2");
		UE_LOG(LogTemp, Warning, TEXT("Vida: %f. Va a huir... estado %d"), Health, State);
	}
	else if (Health <= 0)
	{
		if (!Muerto) Muerto = true;
		else return 0;
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
		if (auto MyController = Cast<ASecondEnemyController>(GetController()))
		{
			MyController->GetBrainComponent()->StopLogic("Death");
			SetLifeSpan(3.0f);
			AShooterController* A = Cast<AShooterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			A->ActualizarKillsDesdeAquiPorqueSiNoNoFunciona();
		}
	}
	//Puedo retornar ncualquier tipo de float para dar información del golpe al causante del daño.
	return Health;
}

// Called to bind functionality to input
void ASecondEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASecondEnemy::PlayAttackMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("Reproduciendo el montaje..."));
	PlayAnimMontage(AttackMontage);
}

//Se ejecuta bajo un AnimNotify.
void ASecondEnemy::OnAttackHit()
{
	TArray<FOverlapResult> Results;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRadius);
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	DrawDebugSphere(GetWorld(), AttackPoint->GetComponentLocation(), AttackRadius, 16, FColor::Red, false, 2.0f);
	GetWorld()->OverlapMultiByChannel(Results, AttackPoint->GetComponentLocation(), FQuat::Identity, ECC_GameTraceChannel1, CollisionShape, CollisionParams);

	for (FOverlapResult Result : Results)
	{
		// Si el enemigo pega a otro enemigo, no ocurre nada
		if (Result.GetActor()->ActorHasTag("Player")) UGameplayStatics::ApplyDamage(Result.GetActor(), BaseDamage, GetController(), this, UDamageType::StaticClass());
	}
}