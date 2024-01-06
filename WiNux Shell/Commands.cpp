#include "Commands.hpp"



/**
 * \brief ���������Ϣ
 */
void PrintHelpInfo()
{
	
}

/**
 * \brief cd����л�Ŀ¼
 */
void ChangeDictionary()
{
	try
	{
		std::filesystem::current_path(args[1]);
	}
	catch (std::filesystem::filesystem_error& e)//ʹ�������������쳣����ֹ����Ҫ�ĸ��ƺ�Ǳ�ڵ���Ƭ����
	{
		Output::OutputError(std::format("{}���޷��л�����Ӧ��Ŀ¼", args[1]));
		Output::OutputError(std::format("�������ݣ�{}", e.what()));
	}
}

/**
 * \brief echo����
 * \note "-e"���� ����ת���ַ�
 */
void Echo()
{
	if(args[std::size(args) - 1] == "-e")//���������ת���ַ��Ĳ�������"\x"ת��Ϊת���ַ�
	{
		const unsigned char EscapeCharacterIterator = args[2].find('\\');
		switch (args[2][EscapeCharacterIterator + 1])
		{
		case 'n': args[2].replace(EscapeCharacterIterator, EscapeCharacterIterator + 1, "\n");break;
		case 't': args[2].replace(EscapeCharacterIterator, EscapeCharacterIterator + 1, "\t");break;
		case 'r': args[2].replace(EscapeCharacterIterator, EscapeCharacterIterator + 1, "\r");break;
		case '\'': args[2].replace(EscapeCharacterIterator, EscapeCharacterIterator + 1, "\'");break;
		case '\"': args[2].replace(EscapeCharacterIterator, EscapeCharacterIterator + 1, "\"");break;
		default:break;
		}
	}
	if(args[2] == ">")//�ض�д
	{
		std::ofstream OutFileStream(args[3]);
		if (!OutFileStream) 
			Output::OutputError(std::format("{}��δ�ɹ����ļ���", args[3]));
		OutFileStream << args[1];
		OutFileStream.close();
	}
	/*if(args.size() == 2)
	{
		Output::OutputInfo(args[1]);
	}
	else
	{
		if(args.size() != 4)
			if (args[2] == ">>")
			{
				std::ofstream OutFileStream(args[3]);
				OutFileStream << args[1];
			}
			else if (str == ">")
			{
				std::string content;
				std::cin >> content;
				std::ofstream out(content);
				out << str;
			}
			else
				Output::OutputInfo(str);
	}*/
}

/**
 * \brief �о�Ŀ¼
 */
void FindFile()
{
	if(args[std::size(args) - 1] == "--help")//�����"--help"�����������������
	{
		Output::OutputInfo("ls [path]���о�Ŀ¼�µ������ļ�");
	}
	else {
		std::filesystem::path Path;
		if (std::size(args) == 1)//���ֻ��"ls"ָ������ǰĿ¼�µ������ļ�
			Path = ".";
		else
			Path = args[1];
		for (const auto& entry : std::filesystem::directory_iterator(Path))
			//������directory_iterator()�������ص�ָ��PathĿ¼�µĶ���ĵ��������˵������ڱ���ʱ�Զ�ָ�������ļ���
		{
			if (std::filesystem::is_directory(entry.path()))//�����Ŀ¼
				std::cout << termcolor::bright_blue << entry.path().filename() << termcolor::reset << std::endl;
			else
				std::cout << entry.path().filename() << std::endl;
		}
	}
}

/**
 * \brief �½�Ŀ¼
 */
void MakeDictionary()
{
	std::filesystem::create_directory(args[1]);
}

/**
 * \brief �����ļ�
 */
void MakeFile()
{
	std::ofstream OutFileStream(args[1]);
	if (!OutFileStream)
		Output::OutputError(std::format("{}��δ�ɹ������ļ���", args[1]));
	OutFileStream.close();
}