#include "GpReaderBase.h"
#include "StreamReader.h"
#include "GpReaderError.h"
#include "Model.h"
#include <cstdint>
#include <string>
#include <algorithm>
#include <limits>
#include <cmath>

namespace libgp
{
	GpReaderBase::GpReaderBase(std::set<GpVersion> supportedVersions) :
		supportedVersions_(supportedVersions)
	{
	}

	GpReaderBase::~GpReaderBase() = default;

	GpVersion GpReaderBase::version() const noexcept { return version_; }

	void GpReaderBase::readAndValidateVersion(StreamReader& reader)
	{
		auto version = readVersion(reader);
		
		if (supportedVersions_.find(version) == supportedVersions_.end()) {
			throw GpReaderError("Unsupported version: " + version.full());
		}

		version_ = version;
	}

	GpVersion GpReaderBase::readVersion(StreamReader& reader) const
	{
		// There are always 30 bytes allocated to the version string, but the first
		// byte of the file holds the actual string length
		auto size = reader.readUnsignedByte() & 0xFF;
		auto strLen = size >= 0 && size <= 30 ? size : 30;

		auto version = reader.readString(30);
		version.resize(strLen);

		try {
			return GpVersion::parse(version);
		} catch (const std::exception e) {
			throw GpReaderError("Unable to read version");
		}
	}

	int16_t GpReaderBase::byteToChannelShort(int8_t byte) const noexcept
	{
		auto shortMax = std::numeric_limits<int16_t>::max();
		auto shortMin = std::numeric_limits<int16_t>::min();
		auto shifted = static_cast<int16_t>((byte << 3) - 1);
		auto value = std::max(shortMin, std::min(shortMax, shifted));
		return std::max(value, (int16_t)-1) + 1;
	}
}
