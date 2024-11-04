#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <iostream>
#include <lib.hpp>
#include <fstream>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::ofstream* userp)
{
	size_t totalSize = size * nmemb;
	userp->write(static_cast<const char*>(contents), totalSize);
	return totalSize;
}

const bool Cownload::getAssets(const std::string& repo, const std::string& tag, const std::string& path = ".")
{
	CURL* curl;
	CURLcode res;
	std::string url = "https://api.github.com/repos/" + repo + "/releases/tags/" + tag;
	std::string readBuffer;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0"); // GitHub API requires a user-agent
		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			curl_easy_cleanup(curl);
			return false;
		}

		curl_easy_cleanup(curl);
	}

	nlohmann::json j = nlohmann::json::parse(readBuffer);
	if (j.contains("assets"))
	{
		for (const auto& asset : j["assets"])
		{
			std::string downloadName = asset["name"];
			std::string downloadUrl = asset["url"];;

			if (downloadName.empty())
			{
				std::cerr << "Asset name is empty, skipping." << std::endl;
				continue;
			}

			std::ofstream file(path + "/" + downloadName, std::ios::binary);
			if (!file)
			{
				std::cerr << "Could not open file for writing: " << downloadName << std::endl;
				return false;
			}

			curl = curl_easy_init();
			if (curl)
			{
				curl_easy_setopt(curl, CURLOPT_URL, downloadUrl.c_str());
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Just in case
				res = curl_easy_perform(curl);
				if (res != CURLE_OK)
				{
					std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
				}
				curl_easy_cleanup(curl);
			}
			file.close();
		}
	}
	return true;
}

const bool Cownload::getFile(const std::string& link, const std::string& path = ".")
{
	std::string out = fetchFilename(link);
	std::ofstream file(path + "/" + out, std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << path + out << std::endl;
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

const bool Cownload::download(const std::string& link, const std::string& path = ".")
{
	if (link.find("gh:") == 0)
	{
		std::string repo, tag;

		std::string rest = link.substr(3);
		size_t tagPos = rest.find('#');
		if (tagPos != std::string::npos)
		{
			repo = rest.substr(0, tagPos);
			std::string tagAndFile = rest.substr(tagPos + 1);
			size_t filePos = tagAndFile.find('@');
			if (filePos != std::string::npos)
			{
				tag = tagAndFile.substr(0, filePos);
			}
			else
			{
				tag = tagAndFile;
			}
			return getAssets(repo, tag, path);
		}
		else
		{
			std::cerr << "Invalid GitHub link format." << std::endl;
			return false;
		}
	}
	else
	{
		return getFile(link, path);
	}
}
