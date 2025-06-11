// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Cards/CardEffectAbstract.h"
#include "CardEffect_UnitModifier.generated.h"

class UUnitModifierAbstract;
/**
 * 
 */
UCLASS()
class TFMPROJECT_API UCardEffect_UnitModifier : public UCardEffectAbstract
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UnitModifierEffectPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle UnitModifierRow;

	virtual bool ApplyEffectToUnit(FUnitStats& UnitStats) override;
	
};
