#include "main.hpp"


void PrintIntroduction()
{
	std::cout << R("");
}

/**
 * \brief 检查是否以管理员权限运行
 * \return true为以管理员权限运行，false为非管理员权限运行
 */
bool IsRunningAsAdmin() {
	BOOL isAdmin = FALSE;
	PSID adminGroup;
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

	// 为BUILTIN\Administrators组创建一个SID
	if (!AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup))  // 无法分配SID
	{
		Output::OutputError("无法检查是否拥有管理员权限！");
		return false;
	}

	// 检查令牌是否具有管理员组SID。
	if (!CheckTokenMembership(nullptr, adminGroup, &isAdmin)) {
		isAdmin = FALSE;
	}

	// 释放SID的内存。
	FreeSid(adminGroup);

	return isAdmin;
}


/**
 * \brief 输出函数
 * \note 使用termcolor库进行输出
 * 
 */
inline void Output::OutputError(const std::string& OutPutContext)
{
	std::cerr << termcolor::red << OutPutContext << termcolor::reset << std::endl;
}

inline void OutputInfo(const std::string& OutPutContext)
{
	std::cout << OutPutContext << std::endl;
}

inline void OutputWarning(const std::string& OutPutContext)
{
	std::cout << termcolor::yellow << OutPutContext << termcolor::reset << std::endl;
}

/**
 * \brief 输出提示词
 */
void PrintPrompt()
{
	WCHAR PCName[MAX_COMPUTERNAME_LENGTH + 1];//直接使用WCHAR类型定义
	WCHAR UserName[255];
	DWORD size = std::size(PCName);//计算数组长度，std::size()函数的参数是数组名，返回值是数组长度
	GetComputerName(PCName, &size);//GetComputerName()函数的第一个参数的类型是LPWSTR，所以直接使用WCHAR定义前面的数组，WCHAR是wchar_t的别名，常见于Windows API编程
	GetUserName(UserName, &size);//GetUserName()函数同上

	std::wcout << termcolor::bright_green << UserName << "@" << PCName << termcolor::reset << ':';
	std::cout << termcolor::bright_blue << std::filesystem::current_path().string() << termcolor::reset << "$ ";
	/**
	 * std::filesystem::current_path() 调用返回当前工作目录的 std::filesystem::path 对象，
	 * 接着.string() 是对这个返回的 std::filesystem::path 对象调用的成员函数，
	 * 它将路径对象转换为一个标准的字符串（std::string）表示。
     */
}

/**
 * \brief 从CommandMap中获取最为相似的指令
 * \param Command 需要查找的指令
 *
 * \note 本函数会在找到相似指令时输出相应信息
 */
void FindPerhapsCommand(const std::string& Command)
{
	double current_highest_score{ 0 };
	std::string current_highest_score_command;
	for(const std::pair<std::string, CommandFun>& CommandPair : CommandMap)
	{
		const double score = rapidfuzz::fuzz::ratio(CommandPair.first, Command);
		if (score > current_highest_score)
		{
			current_highest_score = score;
			current_highest_score_command = CommandPair.first;
		}
	}
	if (current_highest_score < 60)
		return;
	Output::OutputError(std::format("可能的指令：{}", current_highest_score_command));
}

/**
 * \brief 执行指令字典中的指令
 */
void ExecuteCommand()
{
	const std::string Command = args[0];
	if (auto CommandMapIterator = CommandMap.find(Command); CommandMapIterator != CommandMap.end())//查找指令，使用if-init初始化
	{
		CommandMapIterator->second();//执行指令
	}
	else//未找到指令
	{
		Output::OutputError(std::format("{}：未找到指令\n", Command));
		FindPerhapsCommand(Command);
	}
}

/**
 * \brief 解析命令行并储存在字符串向量args中
 *
 * \note 程序中针对""内进行了特殊处理
 */
void ParseArguments()
{
	do
	{
		std::string arguments;
		if(std::cin.peek() != '\"')//检查是否是以"\""开头的内容
			std::cin >> arguments;
		else
		{
			std::cin.ignore(1);//将"\""舍弃
			std::getline(std::cin, arguments, '"');
		}
		args.push_back(arguments);
	} while (!std::cin.eof());
}

/**
 * \brief 主函数
 * \return 0为正常退出，1为异常退出
 */
int main()//一个数组，内容是每个指令字符串的首地址
{
	if (IsRunningAsAdmin())
	{
		Output::OutputError("WiNux Shell需要管理员权限运行！\n请右键选择使用管理员运行本程序！");
	}
	else
		Output::OutputWarning("成功检测到管理员权限！");
	while (true)
	{
		PrintPrompt();
		ParseArguments();
		ExecuteCommand();
		std::vector<std::string>{}.swap(args);//swap trick 清空args
		std::cin.clear();//清空eof标志位
	}
}
