// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CardEffectAbstract.generated.h"


struct FUnitStats;

UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class TFMPROJECT_API UCardEffectAbstract : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	virtual bool ApplyEffectToUnit(FUnitStats& UnitStats) PURE_VIRTUAL(UCardEffectAbstract::ApplyEffectToUnitCard, return false;);
};
