// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CardDataAsset.generated.h"

class UCardEffectAbstract;


UCLASS(Blueprintable, BlueprintType)
class TFMPROJECT_API UCardDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UniqueID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AttackPoints = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UCardEffectAbstract*> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTargetsPlayer = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

};
