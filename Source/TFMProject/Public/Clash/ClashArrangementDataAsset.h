// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Units/UnitStats.h"
#include "ClashArrangementDataAsset.generated.h"

class UCardDataAsset;
class UAIClashAgentAbstract;

UCLASS(Blueprintable, BlueprintType)
class TFMPROJECT_API UClashArrangementDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clash|Units")
	TArray<FUnitStats> PlayerUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clash|Units")
	TArray<FUnitStats> EnemyUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Clash|AI")
	TObjectPtr<UAIClashAgentAbstract> AIClashAgent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clash|Cards")
	TArray<UCardDataAsset*> ClashAvailableCards;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clash|General Config")
	int32 ClashRandomSeed = 12345;
};
