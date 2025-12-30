// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputAction.h"
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SURVIVALSHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();
	
	void Cure(int HPAmount);
	void AddAmmo(int AmmoAmount);
	void ApplyBerserkEffect(int Seconds);
	void UnapplyBerserkEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float CurrentHealth;

private:
	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* RechargeAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category= "Effects")
	UParticleSystem* ShootParticles;

	UPROPERTY(EditAnywhere, Category= "Effects")
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category= "Stats")
	float TimeBetweenAttacks = 0.15f;
	
	UPROPERTY(EditAnywhere, Category= "Stats")
	float AttackRange = 20000;

	UPROPERTY(EditAnywhere, Category= "Stats")
	float BaseDamage = 20;

	UPROPERTY(EditAnywhere, Category= "Stats")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category= "Stats")
	int MaxReserveAmmo = 500;

	int CurrentReserveAmmo;

	UPROPERTY(EditAnywhere, Category= "Stats")
	int MaxChargerAmmo = 20;

	int CurrentChargerAmmo;

	UPROPERTY(EditAnywhere, Category= "Stats")
	float NoiseThreshold = 400;

	UPROPERTY(EditAnywhere, Category= "Sounds")
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, Category= "Sounds")
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere, Category= "Sounds")
	USoundBase* RechargeSound;
	
	UPROPERTY(EditAnywhere, Category= "Sounds")
	USoundBase* DeadSound;
	
	float Timer;

	bool CanJump = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool IsRecharging;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool IsJumping;

	FPostProcessSettings OGPostProcess;
	float OGTimeBetweenAttacks;
	float OGMoveSpeed;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void DoShotEffect();
	void Shoot();
	void Recharge();
	void Jump();
};
