#pragma once

#include <string>

static std::string fetchFilename(const std::string& url)
{
	size_t last = url.find_last_of('/');
	if (last == std::string::npos)
	{
		return url;
	}

	return url.substr(last + 1);
}

namespace Cownload
{
	const bool getAssets(const std::string& repo, const std::string& tag, const std::string& path);
	const bool getFile(const std::string& link, const std::string& path);
	const bool download(const std::string& link, const std::string& path);
} // namespace Cownload