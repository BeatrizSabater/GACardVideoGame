// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Cards/CardEffectAbstract.h"
#include "CardEffect_DoubleSpice.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UCardEffect_DoubleSpice : public UCardEffectAbstract
{
	GENERATED_BODY()
	
public:
	virtual bool ApplyEffectToUnit(FUnitStats& UnitStats) override;
};
