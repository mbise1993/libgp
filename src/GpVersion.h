#pragma once

#include <string>
#include <cstdint>

namespace libgp
{
	class GpVersion
	{
	public:
		std::string full() const noexcept;
		uint8_t major() const noexcept;
		uint8_t minor() const noexcept;

		static GpVersion parse(std::string version);

	private:
		std::string full_;
		uint8_t major_;
		uint8_t minor_;
	};

	bool operator==(const GpVersion& lhs, const GpVersion& rhs);
	bool operator<(const GpVersion& lhs, const GpVersion& rhs);
}