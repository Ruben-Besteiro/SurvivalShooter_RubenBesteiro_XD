// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseEvent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"
#include "Player/ShooterController.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
    // Esto es un comentario
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Empiezo con el timer a este valor para poder atacar desde el principio.
	Timer = TimeBetweenAttacks;

	CurrentHealth = MaxHealth;
	CurrentReserveAmmo = MaxReserveAmmo;
	CurrentChargerAmmo = MaxChargerAmmo;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	auto ShooterController = Cast<AShooterController>(GetController());

	CurrentHealth -= DamageAmount;
	//Se lo comunico al controlador para que actualice información: por ejemplo, barra de vida.
	ShooterController->UpdateHealth(CurrentHealth / MaxHealth);
	
	if (CurrentHealth <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
		
		//Comunico al controlador que hemos muerto
		ShooterController->PawnDead();
		Destroy();
	}
	return CurrentHealth;
}


void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (GetVelocity().Length() >= NoiseThreshold)
	{
		MakeNoise(1, this, GetActorLocation());
	}

	// --- Comprobación de suelo Jumpable ---
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 100.f); // hacia abajo

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	CanJump = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_GameTraceChannel2, // Jumpable
		Params
	);

	if (CanJump && Hit.GetActor())
	{
		IsJumping = false;
	}

	// Debug opcional
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.f, 0, 1.f);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
	Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Shoot);
	Input->BindAction(RechargeAction, ETriggerEvent::Started, this, &AShooterCharacter::Recharge);
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterCharacter::Jump);
}

void AShooterCharacter::Move(const FInputActionValue& InputActionValue)
{
	if (IsRecharging) return;
	//Interpreto el input en formato vector.
	FVector2D InputVector = InputActionValue.Get<FVector2D>();
	
	FRotator YawRotator = FRotator(0, GetControlRotation().Yaw, 0);

	
	FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);
	
	
	//Alante, atrás.
	AddMovementInput(ForwardVector, InputVector.Y);
	
	AddMovementInput(RightVector, InputVector.X);
}

void AShooterCharacter::Look(const FInputActionValue& InputActionValue)
{
	FVector2D MouseLook = InputActionValue.Get<FVector2D>();

	//Hago rotar al controlador para derivar esta rotación en cámara/personaje, etc....
	AddControllerPitchInput(-MouseLook.Y);
	AddControllerYawInput(MouseLook.X);
	
}

void AShooterCharacter::DoShotEffect()
{
	//Spawn Emitter Attached para meter las partículas en el Socket.
	UGameplayStatics::SpawnEmitterAttached(ShootParticles, GetMesh(),TEXT("Muzzle_01"));
}

void AShooterCharacter::Shoot()
{
	if (IsRecharging) return;
	if (Timer >= TimeBetweenAttacks && CurrentChargerAmmo > 0)
	{
		CurrentChargerAmmo--;
		UE_LOG(LogTemp, Warning, TEXT("%d %d"), CurrentReserveAmmo, CurrentChargerAmmo)
		auto ShooterController = Cast<AShooterController>(GetController());
		ShooterController->ActualizarMunicionDesdeFueraPorqueSiNoNoFunciona(CurrentReserveAmmo, CurrentChargerAmmo);
		DoShotEffect();

		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		FHitResult ShotResult;

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//Ahora, después de esta linea, ViewPointLocation y ViewPointRotation TIENEN un valor.
		GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

		
		//Rotator.Vector --> Pasar la rotación a una dirección.
		FVector EndLocation = ViewPointLocation + ViewPointRotation.Vector() * AttackRange;


		DrawDebugLine(GetWorld(), ViewPointLocation, EndLocation, FColor::Green, false, 2);
		if (GetWorld()->LineTraceSingleByChannel(ShotResult, ViewPointLocation, EndLocation, ECC_GameTraceChannel1, CollisionParams))
		{
			// Event Location es dónde se originó el daño y lo
			UGameplayStatics::SpawnEmitterAtLocation(
	GetWorld(),
	HitParticles,
	ShotResult.Location,
	ShotResult.Normal.Rotation(),   // rotar hacia la superficie
	true                            // autodestruir
);
			UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		ShotResult.GetActor(),     // actor que recibe daño
		this,                      // instigador (quien hace el daño)
		BaseDamage,                      // cantidad de daño (o la tuya)
		GetActorLocation(),        // ubicación del evento
		ShotResult.ImpactPoint     // impacto
	);
			//UAIPerceptionSystem::ReportPerceptionEvent(GetWorld(), UAISenseEvent::);
			UGameplayStatics::ApplyDamage(ShotResult.GetActor(), BaseDamage, GetController(), this, UDamageType::StaticClass());
		}
		Timer = 0;
	}
}

// NOTA: Para la animación de recargar, las únicas animaciones que había en Mixamo eran sin moverse o moviéndose solo hacia delante
// así que cogí la animación menos mala, aunque pasa que el personaje recarga un arma invisible con el arma verdadera flotando por ahí
void AShooterCharacter::Recharge()
{
	if (CurrentChargerAmmo == MaxChargerAmmo) return;
	if (IsRecharging) return;
    
	IsRecharging = true;
    
	FTimerHandle RechargeTimerHandle;
	GetWorldTimerManager().SetTimer(
		RechargeTimerHandle,
		[this]()
		{
			int AmmoToTransfer = FMath::Min(MaxChargerAmmo - CurrentChargerAmmo, CurrentReserveAmmo);
            
			CurrentChargerAmmo += AmmoToTransfer;
			CurrentReserveAmmo -= AmmoToTransfer;
            
			IsRecharging = false;
            
			auto ShooterController = Cast<AShooterController>(GetController());
			ShooterController->ActualizarMunicionDesdeFueraPorqueSiNoNoFunciona(CurrentReserveAmmo, CurrentChargerAmmo);
		},1.5f,false
	);
}

void AShooterCharacter::Jump()
{
	if (CanJump)
	{
		UE_LOG(LogTemp, Warning, TEXT("Jumping"));
		LaunchCharacter(FVector(0, 0, 600), false, true);
		IsJumping = true;
		CanJump = false;
	}
}

void AShooterCharacter::Cure(int HPAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Vida vieja: %f"), CurrentHealth);
	CurrentHealth += HPAmount;
	if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;
	
	auto ShooterController = Cast<AShooterController>(GetController());
	UE_LOG(LogTemp, Warning, TEXT("Vida nueva: %f"), CurrentHealth);
	ShooterController->ActualizarVidaDesdeFueraPorqueSiNoNoFunciona(CurrentHealth / MaxHealth);
}

void AShooterCharacter::AddAmmo(int AmmoAmount)
{
	// Llenamos el cargador, y lo que sobre, a la reserva
	int ChargerAmmoAmount = MaxChargerAmmo - CurrentChargerAmmo;
	int ReserveAmmoAmount = AmmoAmount - ChargerAmmoAmount;
	UE_LOG(LogTemp, Warning, TEXT("%d %d"), ChargerAmmoAmount, ReserveAmmoAmount);
	
	CurrentChargerAmmo += ChargerAmmoAmount;
	CurrentReserveAmmo += ReserveAmmoAmount;

	auto ShooterController = Cast<AShooterController>(GetController());
	ShooterController->ActualizarMunicionDesdeFueraPorqueSiNoNoFunciona(CurrentReserveAmmo, CurrentChargerAmmo);
}