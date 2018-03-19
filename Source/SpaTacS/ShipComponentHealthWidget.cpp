// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipComponentHealthWidget.h"
#include "ShipComponents/ShipComponentBase.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void UShipComponentHealthWidget::NativeConstruct()
{
   /* UVerticalBox* RootWidget = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("RootWidget"));
    check(RootWidget);
    WidgetTree->RootWidget = RootWidget;
    WidgetTree->Modify();
    */
    Super::NativeConstruct();
}

void UShipComponentHealthWidget::SetShip(const AStarShip* ship)
{
	
	if (Ship == ship)
		return;

    UVerticalBox* RootWidget = Cast<UVerticalBox>(GetRootWidget());
	check(RootWidget);

	Ship = ship;
	if (Ship)
	{
        RootWidget->ClearChildren();
		Ship->GetComponents(SubComps);
		HealthBars.Empty(SubComps.Num());
		for (auto& Comp : SubComps)
		{
            UCanvasPanel* Panel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
			UProgressBar* ProgressBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass());
            UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
			ProgressBar->SetFillColorAndOpacity(FLinearColor(0.f, 0.8f, 0.f, 1.f));
            RootWidget->AddChild(Panel);
            TextBlock->SetText(Comp->GetComponentName());
            TextBlock->Font.Size = 20;
            Panel->AddChild(ProgressBar);
            Panel->AddChild(TextBlock);
			HealthBars.Add(ProgressBar);

            UCanvasPanelSlot* PBSlot = Cast<UCanvasPanelSlot>(ProgressBar->Slot);
            PBSlot->SetSize(FVector2D(200, 50));


            UCanvasPanelSlot* TextSlot = Cast<UCanvasPanelSlot>(TextBlock->Slot);
            TextSlot->SetAlignment(FVector2D(0.f, -0.5f));
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

