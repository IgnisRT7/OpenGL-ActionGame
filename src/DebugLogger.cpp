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

	void LogBuffer::LogtoBuffer(const char* str, LogType type){

		LogBuffer::Instance().logBuf.push_back(LogMapType(type, str));
	}

	void LogBuffer::LogInfo(bool isUsedTag, bool dontDuplicateTag, LogType fileter){

		this->isOutputTag = isUsedTag;
		this->isDuplicateOutputTag = dontDuplicateTag;
		this->filter = fileter;
	}

	void LogBuffer::Output() const {

		auto& inst = LogBuffer::Instance();

		LogColor strColor = LogColor::White;
		std::string logTypeStr;

		for (auto& logData : inst.logBuf) {
	
			if (prevLogType != logData.first) {
				//�����̐F�Ǝw�蕶����̐ݒ�

				switch (logData.first) {
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
			}
			else {
				logTypeStr = "\t";
			}

			std::string newStr = "\033[m\033[m";
			//���O�R�[�h�̑}��
			newStr.insert(sizeof("\033["), logData.second);
			//�^�O�R�[�h�̑}��
			newStr.insert(sizeof("\033["), logTypeStr);
			//�F�R�[�h�̑}��
			newStr.insert(sizeof("\033[") - 1, std::to_string(strColor));

			std::cout << newStr << std::endl;
			inst.prevLogType = logData.first;
		}

		inst.logBuf.clear();
	}


}