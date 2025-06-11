// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Units/UnitModifierAbstract.h"
#include "UnitModifier_Spice.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UUnitModifier_Spice : public UUnitModifierAbstract
{
	GENERATED_BODY()

public:
	
	virtual void ActivateModifier(UObject* WorldContextObject, FUnitStats& UnitStats) override;
};
