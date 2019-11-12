/**
*	@file	main.cpp
*	@author	Takuya Yokoyama
*/
#include "GameEngine.h"

#ifdef _DEBUG

#include <crtdbg.h>

#endif

int main()
{

	GameEngine& engine = GameEngine::Instance();

	if (!engine.Init(glm::vec2(1048, 800), "title")){
		return -1;
	}

	//デフォルトシーンの追加コードをここに記述

	engine.Run();

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG


	return 0;
}



