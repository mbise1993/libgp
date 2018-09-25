#include "GpVersion.h"
#include <exception>

namespace libgp
{
	std::string GpVersion::full() const noexcept { return full_; }

	uint8_t GpVersion::major() const noexcept { return major_; }

	uint8_t GpVersion::minor() const noexcept { return minor_; }

	GpVersion GpVersion::parse(std::string version)
	{
		auto pos = version.find_last_of('v');
		if (pos == std::string::npos) {
			throw std::runtime_error("Invalid version string format");
		}

		auto versionStr = version.substr(pos + 1);
		pos = versionStr.find('.');
		auto majorStr = versionStr.substr(0, pos);
		auto minorStr = versionStr.substr(pos + 1);

		GpVersion gpVersion;
		gpVersion.full_ = version;

		try {
			gpVersion.major_ = std::stoi(majorStr);
			gpVersion.minor_ = std::stoi(minorStr);
		} catch (const std::invalid_argument& e) {
			throw std::runtime_error("Invalid version string format");
		}

		return gpVersion;
	}

	bool operator==(const GpVersion& lhs, const GpVersion& rhs) { return lhs.full() == rhs.full(); }

	bool operator<(const GpVersion& lhs, const GpVersion& rhs)
	{
		return lhs.major() != rhs.major() 
			? (lhs.major() < rhs.major()) 
			: (lhs.minor() < rhs.minor());
	}
}