/**
*	@file FontRenderer.cpp
*/
#include "FontRenderer.h"

namespace Font {

	FontDataPtr Buffer::CreateFontFromFile(const char* filename){

		struct Impl : FontData {};
		auto p = std::make_shared<Impl>();
		
	}

}