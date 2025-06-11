// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Units/UnitStats.h"
#include "UnitModifierAbstract.generated.h"

class UUnitModifierAbstract;

UENUM(BlueprintType)
enum class EUnitModifierTriggerPhase : uint8
{
	None,
	Attacking,
	ModifierReceived,
	BeingAttacked,
	DamageReceived,
	TurnEnded,
	CounterReduction	
};


USTRUCT()
struct FUnitModifier : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ModifierDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUnitModifierTriggerPhase ModifierTriggerPhase = EUnitModifierTriggerPhase::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ModifierPriority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUnitModifierAbstract> ModifierClass;
};

USTRUCT(BlueprintType, Blueprintable)
struct FAppliedUnitModifier
{
	GENERATED_BODY()

	FAppliedUnitModifier(){}

	FAppliedUnitModifier(const FDataTableRowHandle& UnitModifierRow, int32 InModifierPoints)
	: ModifierPoints(InModifierPoints)
	{
		FUnitModifier* InUnitModifier = UnitModifierRow.GetRow<FUnitModifier>(TEXT("FAppliedUnitModifier"));
		if (InUnitModifier != nullptr)
		{
			UnitModifier = *InUnitModifier;
		}
	}
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FUnitModifier UnitModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ModifierPoints = 0;
};

UCLASS(Blueprintable, BlueprintType, Abstract)
class TFMPROJECT_API UUnitModifierAbstract : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAppliedUnitModifier AppliedUnitModifier;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats) PURE_VIRTUAL(UUnitModifierAbstract::ActivateModifier);

	bool ShouldBeRemoved() const { return bShouldBeRemoved; }
	
protected:
	UPROPERTY()
	bool bShouldBeRemoved = false;
};
