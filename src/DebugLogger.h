/**
*	@file DebugLogger.h
*/
#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace DebugLogger{

	/// ログ出力のタイプ
	enum LogType {
		Infomation = 0x01,	// 通常の出力(指定がない場合は白)
		Warning = 0x02,		// 警告時の出力(黄色)
		Error = 0x04,		// エラー発生時の出力(赤)
		AllBits	= 0x07,		// すべてのタイプの列挙
	};

	static std::string logString[] = {
		"[Info]: ",
		"[Warning]: ",
		"[Error]: ",
	};

	/// ログの色
	enum LogColor {
		//30:black, 31:red, 32:green, 33:yellow, 34:blue, 35:magenta, 36:cyan, 37:white
		Block	= 30,
		Red		= 31,
		Green	= 32,
		Yellow	= 33,
		Blue	= 34,
		Magenta = 35, 
		Cyan	= 36,
		White	= 37,

	}; 

	class LogBuffer{
	public:
		static LogBuffer& Instance();

		/**
		*	ログを書き込む
		*
		*	@param str	書き込む文字列
		*	@param type	ログの種類
		*/
		static void Log(const char* str, LogType type = LogType::Infomation);

		/**
		*	ログの詳細設定
		*	
		*	@param isUsedTag		情報タグの表示/非表示フラグ
		*	@param dontDuplicateTag	重複するタグが続けて書き込まれた場合はタグを付けない
		*	@param filter			表示するフィルタ
		*/
		void LogInfo(bool isUsedTag = true, bool dontDuplicateTag = true, LogType fileter = LogType::AllBits);

	private:

		LogBuffer() = default;
		~LogBuffer() {}
		LogBuffer(const LogBuffer&) = delete;
		const LogBuffer& operator=(const LogBuffer&) = delete;

	private:

		using LogMapType = std::pair<LogType, std::string>;

		bool isOutputTag = true;						///< タグを表示するかどうか
		bool isDuplicateOutputTag = false;				///< タグの重複表示をするかどうか
		LogType prevLogType = LogType::AllBits;			///< 前回に出力したログのタイプ

		LogType filter = LogType::AllBits;				///< 出力するフィルタ
		std::vector<LogMapType> logBuf;			///< ログを溜めておくバッファ
	};


}