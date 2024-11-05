#include <boost/filesystem.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <Cownloadpp.hpp>
#include <iostream>
#include <fstream>

const bool Cownloadpp::download(const std::string& link, const std::string& path = ".")
{
	if (!boost::filesystem::exists(path))
	{
		if (!boost::filesystem::create_directories(path))
		{
			std::cerr << "Directory can't be created: " << path << std::endl;
			return false;
		}
	}

	std::string out = fetchFilename(link);

	if (out.empty())
	{
		std::cerr << "Filename is empty." << std::endl;
		return false;
	}

	std::ofstream file(path + "/" + out, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << path + "/" + out << std::endl;
		return false;
	}

	try
	{
		cURLpp::initialize();

		cURLpp::Cleanup cleanup;
		cURLpp::Easy request;
		request.setOpt(cURLpp::Options::Url(link));
		request.setOpt(cURLpp::Options::FollowLocation(true));
		request.setOpt(cURLpp::Options::WriteFunction(
			[&file](char* ptr, size_t size, size_t nmemb) -> size_t
			{
				size_t totalSize = size * nmemb;
				file.write(ptr, totalSize);
				return totalSize;
			}
		));

		request.perform();
	}
	catch (const cURLpp::RuntimeError& e)
	{
		std::cerr << "Runtime error: " << e.what() << std::endl;
		file.close();
		return false;
	}
	catch (const cURLpp::LogicError& e)
	{
		std::cerr << "Logic error: " << e.what() << std::endl;
		file.close();
		return false;
	}

	file.close();
	return true;
}
