#include "main.hpp"


void PrintIntroduction()
{
	std::cout << R("");
}

/**
 * \brief ����Ƿ��Թ���ԱȨ������
 * \return trueΪ�Թ���ԱȨ�����У�falseΪ�ǹ���ԱȨ������
 */
bool IsRunningAsAdmin() {
	BOOL isAdmin = FALSE;
	PSID adminGroup;
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

	// ΪBUILTIN\Administrators�鴴��һ��SID
	if (!AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup))  // �޷�����SID
	{
		Output::OutputError("�޷�����Ƿ�ӵ�й���ԱȨ�ޣ�");
		return false;
	}

	// ��������Ƿ���й���Ա��SID��
	if (!CheckTokenMembership(nullptr, adminGroup, &isAdmin)) {
		isAdmin = FALSE;
	}

	// �ͷ�SID���ڴ档
	FreeSid(adminGroup);

	return isAdmin;
}


/**
 * \brief �������
 * \note ʹ��termcolor��������
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
 * \brief �����ʾ��
 */
void PrintPrompt()
{
	WCHAR PCName[MAX_COMPUTERNAME_LENGTH + 1];//ֱ��ʹ��WCHAR���Ͷ���
	WCHAR UserName[255];
	DWORD size = std::size(PCName);//�������鳤�ȣ�std::size()�����Ĳ�����������������ֵ�����鳤��
	GetComputerName(PCName, &size);//GetComputerName()�����ĵ�һ��������������LPWSTR������ֱ��ʹ��WCHAR����ǰ������飬WCHAR��wchar_t�ı�����������Windows API���
	GetUserName(UserName, &size);//GetUserName()����ͬ��

	std::wcout << termcolor::bright_green << UserName << "@" << PCName << termcolor::reset << ':';
	std::cout << termcolor::bright_blue << std::filesystem::current_path().string() << termcolor::reset << "$ ";
	/**
	 * std::filesystem::current_path() ���÷��ص�ǰ����Ŀ¼�� std::filesystem::path ����
	 * ����.string() �Ƕ�������ص� std::filesystem::path ������õĳ�Ա������
	 * ����·������ת��Ϊһ����׼���ַ�����std::string����ʾ��
     */
}

/**
 * \brief ��CommandMap�л�ȡ��Ϊ���Ƶ�ָ��
 * \param Command ��Ҫ���ҵ�ָ��
 *
 * \note �����������ҵ�����ָ��ʱ�����Ӧ��Ϣ
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
	Output::OutputError(std::format("���ܵ�ָ�{}", current_highest_score_command));
}

/**
 * \brief ִ��ָ���ֵ��е�ָ��
 */
void ExecuteCommand()
{
	const std::string Command = args[0];
	if (auto CommandMapIterator = CommandMap.find(Command); CommandMapIterator != CommandMap.end())//����ָ�ʹ��if-init��ʼ��
	{
		CommandMapIterator->second();//ִ��ָ��
	}
	else//δ�ҵ�ָ��
	{
		Output::OutputError(std::format("{}��δ�ҵ�ָ��\n", Command));
		FindPerhapsCommand(Command);
	}
}

/**
 * \brief ���������в��������ַ�������args��
 *
 * \note ���������""�ڽ��������⴦��
 */
void ParseArguments()
{
	do
	{
		std::string arguments;
		if(std::cin.peek() != '\"')//����Ƿ�����"\""��ͷ������
			std::cin >> arguments;
		else
		{
			std::cin.ignore(1);//��"\""����
			std::getline(std::cin, arguments, '"');
		}
		args.push_back(arguments);
	} while (!std::cin.eof());
}

/**
 * \brief ������
 * \return 0Ϊ�����˳���1Ϊ�쳣�˳�
 */
int main()//һ�����飬������ÿ��ָ���ַ������׵�ַ
{
	if (IsRunningAsAdmin())
	{
		Output::OutputError("WiNux Shell��Ҫ����ԱȨ�����У�\n���Ҽ�ѡ��ʹ�ù���Ա���б�����");
	}
	else
		Output::OutputWarning("�ɹ���⵽����ԱȨ�ޣ�");
	while (true)
	{
		PrintPrompt();
		ParseArguments();
		ExecuteCommand();
		std::vector<std::string>{}.swap(args);//swap trick ���args
		std::cin.clear();//���eof��־λ
	}
}
