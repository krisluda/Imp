// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpWidget.h"
#include "ImpWidgetController.h"

void UImpWidget::SetWidgetController(UImpWidgetController* InWidgetController) {
    WidgetController = InWidgetController;
    OnWidgetControllerSet();
}

