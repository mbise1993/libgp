#include "GpReaderError.h"

namespace libgp
{
	GpReaderError::GpReaderError(const std::string& message) :
		message_(message)
	{
	}

	const char* GpReaderError::what() const
	{
		return message_.c_str();
	}
}