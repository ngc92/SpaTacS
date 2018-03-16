// Fill out your copyright notice in the Description page of Project Settings.

#include "MagazineComponent.h"


// Sets default values for this component's properties
UMagazineComponent::UMagazineComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UMagazineComponent::BeginPlay()
{
	Super::BeginPlay();	
}

int UMagazineComponent::GetFromStorage(const UClass* DesiredType, int Amount)
{
    if (Amount < 0) return 0;

    if (!DesiredType) return 0;

    for (auto& Item : StoredItems)
    {
        if (Item.Type->GetClass() == DesiredType)
        {
            if (Item.Amount > Amount)
            {
                Item.Amount -= Amount;
                return Amount;
            }
            else
            {
                Amount = Item.Amount;
                Item.Amount = 0;
                return Amount;
            }
        }
    }
    return 0;
}

