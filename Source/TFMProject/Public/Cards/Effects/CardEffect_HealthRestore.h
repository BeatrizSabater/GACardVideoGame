// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Cards/CardEffectAbstract.h"
#include "CardEffect_HealthRestore.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UCardEffect_HealthRestore : public UCardEffectAbstract
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealthToRestore = 1;

	virtual bool ApplyEffectToUnit(FUnitStats& UnitStats) override;
};
