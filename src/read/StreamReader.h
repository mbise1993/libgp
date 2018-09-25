#pragma once

#include "GpReaderError.h"
#include <cstdint>
#include <istream>
#include <string>
#include <type_traits>

namespace libgp
{
	class StreamReader
	{
	public:
		explicit StreamReader(std::istream& stream) :
			stream_(stream)
		{
		}

		void skip(uint32_t numBytes) 
		{
			for (auto i = 0; i < numBytes; ++i) {
				readUnsignedByte();
			}
		}

		int8_t readSignedByte() { return read<int8_t>(); }
		uint8_t readUnsignedByte() { return read<uint8_t>(); }

		int16_t readSignedShort() { return read<int16_t>(); }
		uint16_t readUnsignedShort() { return read<uint16_t>(); }

		int32_t readSignedInt() { return read<int32_t>(); }
		uint32_t readUnsignedInt() { return read<uint32_t>(); }

		bool readBoolean() { return static_cast<bool>(readUnsignedByte()); }

		std::string readString(const size_t size)
		{
			auto buffer = new char[size];
			if (!stream_.read(buffer, size)) {
				throw GpReaderError("An error occurred while reading the stream");
			}

			return std::string(buffer, size);
		}

		std::string readByteSizedString() { return readStringAfter<uint8_t>(); }

		std::string readIntSizedString() { return readStringAfter<uint32_t>(); }

		std::string readIntByteSizedString()
		{
			skip(4);
			return readByteSizedString();
		}

	private:
		std::istream& stream_;

		template<typename T, size_t Size = sizeof(T)>
		T read()
		{
			T value;
			if (!stream_.read(reinterpret_cast<char*>(&value), Size)) {
				throw GpReaderError("An error occurred while reading the stream");
			}

			return value;
		}

		template<typename T, typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
		std::string readStringAfter()
		{
			auto strLen = read<T>();
			return readString(strLen);
		}
	};
}