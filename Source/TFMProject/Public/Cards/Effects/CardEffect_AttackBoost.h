// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Cards/CardEffectAbstract.h"
#include "CardEffect_AttackBoost.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UCardEffect_AttackBoost : public UCardEffectAbstract
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackBoost = 1;

	virtual bool ApplyEffectToUnit(FUnitStats& UnitStats) override;
};
