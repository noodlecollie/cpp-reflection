#pragma once

#include <type_traits>

namespace Reflection
{
	template<typename ENUM, typename = typename std::enable_if<std::is_enum<ENUM>::value>::type>
	struct EnumReflection
	{
		// This struct is empty for arbitrary types.
	};
}  // namespace Reflection
