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
 * \brief ��װָ����Ŀ�ִ�ж���
 * \note ʹ��a()���ɵ���a����
 */
using CommandFun = std::function<void()>;

/**
 * \brief ����ָ�����������ָ��ֶδ��棬ͬʱ""�ڵ����ݰ�һ����Ϣ����
 */
inline std::vector<std::string> args;

/**
 * \brief ָ���ֵ�
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