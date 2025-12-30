// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SecondEnemy.generated.h"

UCLASS()
class SURVIVALSHOOTER_API ASecondEnemy : public ACharacter
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ASecondEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayAttackMontage();

private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* AttackPoint;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float AttackRadius = 75;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float BaseDamage = 50;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float Health = 200;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool bIsStrafing;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float RotationSpeed;

	bool Muerto = false;

	FRotator LastRotation;

	
	int32 CurrentIndex = -1;
	

	UFUNCTION(BlueprintCallable)
	void OnAttackHit();
};
