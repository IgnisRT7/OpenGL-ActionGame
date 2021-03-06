/**
*	@file DebugLogger
*/
#include "DebugLogger.h"
#include <conio.h>
#include <stdio.h>
#include <Windows.h>

namespace DebugLogger {

	LogBuffer& LogBuffer::Instance(){

		static LogBuffer instance;
		return instance;
	}

	void LogBuffer::Log(const char* str, LogType type, bool isReturnCode){

		auto& inst = LogBuffer::Instance();

		auto strInfo = (inst.colorMap.find(type) != inst.colorMap.end()) ? 
			inst.colorMap.find(type)->second : StringInfo();

		std::string newStr = "\033[m\033[m";
		//ログの挿入
		newStr.insert(sizeof("\033["), str);
		//タグコードの挿入
		newStr.insert(sizeof("\033["), inst.isOutputTag && inst.prevLogType != type ? strInfo.tagString + ": " : "\t");
		//色コードの挿入
		newStr.insert(sizeof("\033[") - 1, std::to_string(strInfo.logColor));

		if (isReturnCode) {
			std::cout << newStr << std::endl;
		}
		else {
			std::cout << newStr;
		}
		inst.prevLogType = type;
	}

	void LogBuffer::LogInfo(bool isUsedTag, bool dontDuplicateTag, LogType fileter){

		this->isOutputTag = isUsedTag;
		this->isDuplicateOutputTag = dontDuplicateTag;
		this->filter = fileter;
	}

	void LogBuffer::ClearInfo() {

		this->prevLogType = LogType::AllBits;
	}

	void Log(const char* str, LogType type, bool isReturnCode) {

		LogBuffer::Instance().Log(str, type, isReturnCode);
	}

}