#pragma once

#include <exception>

namespace libgp
{
	class GpReaderError : public std::exception
	{
	public:
		explicit GpReaderError(const std::string& message = "Guitar Pro reader error");

		const char* what() const override;

	private:
		std::string message_;
	};
}