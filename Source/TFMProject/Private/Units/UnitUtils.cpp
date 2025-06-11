// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Units/UnitUtils.h"

#include "Cards/CardDataAsset.h"
#include "Units/UnitModifierAbstract.h"
#include "Units/UnitStats.h"

UUnitModifierAbstract* UUnitUtils::GetUnitModifierByClass(TSubclassOf<UUnitModifierAbstract> UnitModifierClass, const FUnitStats& UnitStats)
{
	const auto FoundUnitModifier= UnitStats.UnitModifiers.FindByPredicate([UnitModifierClass](const UUnitModifierAbstract* UnitModifier)
	{
		return UnitModifier->IsA(UnitModifierClass);
	});

	return FoundUnitModifier ? *FoundUnitModifier : nullptr;
}

UUnitModifierAbstract* UUnitUtils::GetUnitModifierByTriggerPhase(EUnitModifierTriggerPhase TriggerPhase, const FUnitStats& UnitStats)
{
	const auto FoundUnitModifier= UnitStats.UnitModifiers.FindByPredicate([TriggerPhase](const UUnitModifierAbstract* UnitModifier)
	{
		return UnitModifier->AppliedUnitModifier.UnitModifier.ModifierTriggerPhase == TriggerPhase;
	});

	return FoundUnitModifier ? *FoundUnitModifier : nullptr;
}

void UUnitUtils::GetCardsFromUniqueIDs(const TArray<UCardDataAsset*>& SearchCardList, const TArray<int32>& InCardsUniqueIDs, TArray<UCardDataAsset*>& OutCards)
{
	OutCards.Empty();
	
	for (auto CardUniqueID : InCardsUniqueIDs)
	{
		const auto FoundCardPtr= SearchCardList.FindByPredicate([CardUniqueID](const UCardDataAsset* Card)
		{
			return Card->UniqueID == CardUniqueID;
		});

		UCardDataAsset* FoundCard = FoundCardPtr ? *FoundCardPtr : nullptr;

		if (!IsValid(FoundCard))
		{
			// TODO: ERROR MESSAGE
			return;
		}

		OutCards.Emplace(FoundCard);
	}
}

void UUnitUtils::GetUniqueIDsFromCards(const TArray<UCardDataAsset*>& CardList, TArray<int32>& OutCardsUniqueIDs)
{
	OutCardsUniqueIDs.Empty();

	for (const auto Card : CardList)
	{
		OutCardsUniqueIDs.Emplace(Card->UniqueID);
	}
}
