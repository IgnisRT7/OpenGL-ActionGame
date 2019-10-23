/**
*	@file	main.cpp
*	@author	Takuya Yokoyama
*/
#include "GameEngine.h"

int main()
{

	GameEngine& engine = GameEngine::Instance();

	if (engine.Init(glm::vec2(2048, 1060), "title")){
		return -1;
	}

	//デフォルトシーンの追加コードをここに記述

	engine.Run();

	return 0;
}



