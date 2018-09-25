#pragma once

#include "../GpVersion.h"
#include <string>
#include <istream>
#include <memory>
#include <set>

namespace libgp
{
	struct Song;
	class StreamReader;

	class GpReaderBase
	{
	public:
		explicit GpReaderBase(std::set<GpVersion> supportedVersions);
		virtual ~GpReaderBase();

		GpVersion version() const noexcept;

		virtual std::unique_ptr<Song> readSong(std::istream& stream) = 0;

	protected:
		void readAndValidateVersion(StreamReader& reader);
		int16_t byteToChannelShort(int8_t byte) const noexcept;

	private:
		GpVersion version_;
		std::set<GpVersion> supportedVersions_;

		GpVersion readVersion(StreamReader& reader) const;
	};
}