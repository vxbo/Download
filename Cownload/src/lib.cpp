#include <boost/filesystem.hpp>
#include <curl/curl.h>
#include <iostream>
#include <lib.hpp>
#include <fstream>

const static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::ofstream* userp)
{
	size_t totalSize = size * nmemb;
	userp->write(static_cast<const char*>(contents), totalSize);
	return totalSize;
}

const bool Cownload::download(const std::string& link, const std::string& path = ".")
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

	CURL* curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			curl_easy_cleanup(curl);
			return false;
		}

		curl_easy_cleanup(curl);
	}

	file.close();
	curl_global_cleanup();
	return true;
}
