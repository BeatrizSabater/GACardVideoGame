// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "unitStats.generated.h"

class UUnitModifierAbstract;

USTRUCT(BlueprintType)
struct TFMPROJECT_API FUnitStats
{
	GENERATED_BODY()

	FUnitStats() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackDamage = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Counter = 1;

	UPROPERTY(BlueprintReadOnly)
	TArray<UUnitModifierAbstract*> UnitModifiers;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentHealth = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentCounter = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CalculatedTurnAttack = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int32 CalculatedReceivedAttack = 0;

	void RestartTransientStats();
};