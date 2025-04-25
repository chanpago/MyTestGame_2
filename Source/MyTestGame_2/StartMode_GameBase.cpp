// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMode_GameBase.h"
#include "StartMenu_HUD.h"
//#include "Controller_StartMenu.h"

AStartMode_GameBase::AStartMode_GameBase()
{
	HUDClass = AStartMenu_HUD::StaticClass();
	DefaultPawnClass = nullptr;
	//PlayerControllerClass = AController_StartMenu::StaticClass();
}

