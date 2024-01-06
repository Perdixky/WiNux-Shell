#pragma once
#include <filesystem>
#include <iostream>
#include <sstream>
#include <functional>
#include <map>
#include <format>
#include <Windows.h>
#include <vector>

#include <rapidfuzz/fuzz.hpp>
#include <termcolor/termcolor.hpp>
#include <boost/asio.hpp>

#include "Commands.hpp"


/**
 * \brief 封装指令函数的可执行对象
 * \note 使用a()即可调用a对象
 */
using CommandFun = std::function<void()>;

/**
 * \brief 储存指令的向量，将指令分段储存，同时""内的内容按一条信息储存
 */
inline std::vector<std::string> args;

/**
 * \brief 指令字典
 */
inline constexpr std::map<std::string, CommandFun> CommandMap =
{
	{"echo", Echo},
	{"cd", ChangeDictionary},
	{"ls", FindFile},
	{"mkdir", MakeDictionary},
	{"touch", MakeFile},
	{"rm", DeleteFile},
	{"rmdir", DeleteDictionary},
	{"mv", MoveFile},
	{"cp", CopyFile},
	{"cat", ReadFile},
	{"exit", Exit},
	{"help", Help}
};

namespace Output
{
	void OutputError(const std::string& OutPutContext);
	void OutputInfo(const std::string& OutPutContext);
	void OutputWarning(const std::string& OutPutContext);
}