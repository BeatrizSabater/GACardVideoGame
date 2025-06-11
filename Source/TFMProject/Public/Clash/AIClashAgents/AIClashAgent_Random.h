// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Clash/AIClashAgentAbstract.h"
#include "AIClashAgent_Random.generated.h"

/**
 * 
 */
UCLASS()
class TFMPROJECT_API UAIClashAgent_Random : public UAIClashAgentAbstract
{
	GENERATED_BODY()

public:

	virtual UCardDataAsset* PickCard(const UObject* WorldContextObject, TArray<UCardDataAsset*>& AvailableCards) override;
};
