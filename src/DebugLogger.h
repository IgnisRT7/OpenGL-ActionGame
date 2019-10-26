/**
*	@file DebugLogger.h
*/
#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace DebugLogger{

	/// ���O�o�͂̃^�C�v
	enum LogType {
		Infomation = 0x01,	// �ʏ�̏o��(�w�肪�Ȃ��ꍇ�͔�)
		Warning = 0x02,		// �x�����̏o��(���F)
		Error = 0x04,		// �G���[�������̏o��(��)
		AllBits	= 0x07,		// ���ׂẴ^�C�v�̗�
	};

	/// ���O�̐F
	enum LogColor {
		//30:black, 31:red, 32:green, 33:yellow, 34:blue, 35:magenta, 36:cyan, 37:white
		Block = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35, 
		Cyan = 36,
		White = 37,

	}; 

	class LogBuffer{
	public:
		static LogBuffer& Instance();

		/**
		*	���O����������
		*
		*	@param str	�������ޕ�����
		*	@param type	���O�̎��
		*/
		void Log(const char* str, LogType type = LogType::Infomation);
		
		/**
		*	���O���o�͂���
		*/
		void Output() const;

		/**
		*	�t�B���^��ݒ肷��
		*
		*	@param �t�B���^��������^�C�v(�����I����)
		*/
		void Filter(LogType types) { filter = types; }



	private:

		LogBuffer() = default;
		~LogBuffer() {}
		LogBuffer(const LogBuffer&) = delete;
		const LogBuffer& operator=(const LogBuffer&) = delete;

	private:

		using LogMapType = std::pair<LogType, std::string>;

		LogType filter = LogType::AllBits;				///< �o�͂���t�B���^
		std::vector<LogMapType> logBuf;			///< ���O�𗭂߂Ă����o�b�t�@
	};


}