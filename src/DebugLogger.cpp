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

		LogColor strColor = LogColor::White;
		std::string logTypeStr;

		//�����̐F�Ǝw�蕶����̐ݒ�
		switch (type) {
		case LogType::Warning:
			strColor = LogColor::Yellow;
			logTypeStr = "[Warning]: ";
			break;
		case LogType::Error:
			strColor = LogColor::Red;
			logTypeStr = "[Error]: ";
			break;
		default:
			strColor = LogColor::Cyan;
			logTypeStr = "[Info]: ";
			break;
		}

		std::string newStr = "\033[m\033[m";
		//���O�R�[�h�̑}��
		newStr.insert(sizeof("\033["), str);
		//�^�O�R�[�h�̑}��
		newStr.insert(sizeof("\033["), inst.isOutputTag && inst.prevLogType != type ? logTypeStr : "\t");
		//�F�R�[�h�̑}��
		newStr.insert(sizeof("\033[") - 1, std::to_string(strColor));

		std::cout << newStr << std::endl;
		inst.prevLogType = type;
	}

	void LogBuffer::LogInfo(bool isUsedTag, bool dontDuplicateTag, LogType fileter){

		this->isOutputTag = isUsedTag;
		this->isDuplicateOutputTag = dontDuplicateTag;
		this->filter = fileter;
	}

}