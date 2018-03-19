// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipComponentHealthWidget.h"
#include "ShipComponents/ShipComponentBase.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

void UShipComponentHealthWidget::SetShip(const AStarShip* ship)
{
	
	if (Ship == ship)
		return;

	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	check(RootWidget);

	Ship = ship;
	if (Ship)
	{
		Ship->GetComponents(SubComps);
		for (auto Bar : HealthBars)
		{
			Bar->RemoveFromParent();
		}

		HealthBars.Empty(SubComps.Num());
		for (auto& Comp : SubComps)
		{
			UProgressBar* ProgressBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
			ProgressBar->SetFillColorAndOpacity(FLinearColor(0.f, 0.8f, 0.f, 1.f));
			RootWidget->AddChild(ProgressBar);
			HealthBars.Add(ProgressBar);
		}
	}
	else {
		SubComps.Empty(0);
		HealthBars.Empty(0);
	}
}

void UShipComponentHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	for (int i = 0; i < SubComps.Num(); ++i)
	{
		HealthBars[i]->SetPercent(SubComps[i]->GetHealthStatus());
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

