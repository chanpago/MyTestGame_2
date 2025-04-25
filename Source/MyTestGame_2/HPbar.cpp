// Fill out your copyright notice in the Description page of Project Settings.


#include "HPbar.h"

void UHPbar::NativeConstruct()
{

}

void UHPbar::SetBarValuePercent(float const value)
{
	HPValue->SetPercent(value);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HPbar is : %f"), value));
}