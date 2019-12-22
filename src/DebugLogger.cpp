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

	void LogBuffer::Log(const char* str, LogType type){

		auto& inst = LogBuffer::Instance();

		auto strInfo = (inst.colorMap.find(type) != inst.colorMap.end()) ? 
			inst.colorMap.find(type)->second : StringInfo();

		std::string newStr = "\033[m\033[m";
		//���O�̑}��
		newStr.insert(sizeof("\033["), str);
		//�^�O�R�[�h�̑}��
		newStr.insert(sizeof("\033["), inst.isOutputTag && inst.prevLogType != type ? strInfo.tagString : "\t");
		//�F�R�[�h�̑}��
		newStr.insert(sizeof("\033[") - 1, std::to_string(strInfo.logColor));

		std::cout << newStr << std::endl;
		inst.prevLogType = type;
	}

	void LogBuffer::LogInfo(bool isUsedTag, bool dontDuplicateTag, LogType fileter){

		this->isOutputTag = isUsedTag;
		this->isDuplicateOutputTag = dontDuplicateTag;
		this->filter = fileter;
	}

}