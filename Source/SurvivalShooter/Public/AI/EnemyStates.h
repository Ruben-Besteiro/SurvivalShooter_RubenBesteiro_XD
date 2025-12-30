// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class EEnemyStates : uint8
{
	Patrol   UMETA(DisplayName = "Patrol"),
	Chase    UMETA(DisplayName = "Chase"),
	Investigate    UMETA(DisplayName = "Investigate"),
	Attack	UMETA(DisplayName= "Attack")
	
};
