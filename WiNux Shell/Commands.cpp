#include "Commands.hpp"



/**
 * \brief 输出帮助信息
 */
void PrintHelpInfo()
{
	
}

/**
 * \brief cd命令，切换目录
 */
void ChangeDictionary()
{
	try
	{
		std::filesystem::current_path(args[1]);
	}
	catch (std::filesystem::filesystem_error& e)//使用引用来捕获异常，防止不必要的复制和潜在的切片问题
	{
		Output::OutputError(std::format("{}：无法切换到相应的目录", args[1]));
		Output::OutputError(std::format("报错内容：{}", e.what()));
	}
}

/**
 * \brief echo命令
 * \note "-e"参数 启用转义字符
 */
void Echo()
{
	if(args[std::size(args) - 1] == "-e")//如果有启用转义字符的参数，将"\x"转换为转义字符
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
	if(args[2] == ">")//截断写
	{
		std::ofstream OutFileStream(args[3]);
		if (!OutFileStream) 
			Output::OutputError(std::format("{}：未成功打开文件！", args[3]));
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
 * \brief 列举目录
 */
void FindFile()
{
	if(args[std::size(args) - 1] == "--help")//如果有"--help"参数，输出帮助内容
	{
		Output::OutputInfo("ls [path]：列举目录下的所有文件");
	}
	else {
		std::filesystem::path Path;
		if (std::size(args) == 1)//如果只有"ls"指令，输出当前目录下的所有文件
			Path = ".";
		else
			Path = args[1];
		for (const auto& entry : std::filesystem::directory_iterator(Path))
			//遍历由directory_iterator()函数返回的指向Path目录下的对象的迭代器（此迭代器在遍历时自动指向所有文件）
		{
			if (std::filesystem::is_directory(entry.path()))//如果是目录
				std::cout << termcolor::bright_blue << entry.path().filename() << termcolor::reset << std::endl;
			else
				std::cout << entry.path().filename() << std::endl;
		}
	}
}

/**
 * \brief 新建目录
 */
void MakeDictionary()
{
	std::filesystem::create_directory(args[1]);
}

/**
 * \brief 创建文件
 */
void MakeFile()
{
	std::ofstream OutFileStream(args[1]);
	if (!OutFileStream)
		Output::OutputError(std::format("{}：未成功建立文件！", args[1]));
	OutFileStream.close();
}