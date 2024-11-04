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

namespace Cownloadpp
{
	const bool download(const std::string& link, const std::string& path);
} // namespace Cownloadpp