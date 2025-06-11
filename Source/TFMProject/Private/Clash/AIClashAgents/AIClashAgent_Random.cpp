// © 2025 Beatriz Sabater Serna <dev@beatrizsabater.es>


#include "Clash/AIClashAgents/AIClashAgent_Random.h"

#include "Clash/ClashSubsystem.h"
#include "Cards/CardDataAsset.h"


UCardDataAsset* UAIClashAgent_Random::PickCard(const UObject* WorldContextObject, TArray<UCardDataAsset*>& AvailableCards)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return nullptr;

	const UClashSubsystem* ClashSubsystem = World->GetSubsystem<UClashSubsystem>();
	if (!IsValid(ClashSubsystem)) return nullptr;
	
	UCardDataAsset* PickedCard = nullptr;
	
	while (PickedCard == nullptr)
	{
		const int32 CardIndex = ClashSubsystem->GetClashRandomStream().RandRange(0, AvailableCards.Num() - 1);
		
		if (!ClashSubsystem->GetPlayedCards().Contains(AvailableCards[CardIndex]))
		{
			PickedCard = AvailableCards[CardIndex];
			AvailableCards.RemoveAtSwap(CardIndex);
			break;
		}
	}
	
	return PickedCard;
}
