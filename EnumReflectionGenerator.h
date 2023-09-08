// No pragma once here! We want to be able to include this header
// multiple times in the same source file.

// Define ENUM_REFLECTION_TYPE to be the type name of the enum
// you want to produce reflection data for.
#ifndef ENUM_REFLECTION_TYPE
	#error ENUM_REFLECTION_TYPE must be defined before including EnumReflectionGenerator.h!
#endif

// Define ENUM_REFLECTION_LIST to your preprocessor symbol
// representing the enum list (ie. the list made up of
// FGCSDK_ENUM_MAPPING(...) entries)
#ifndef ENUM_REFLECTION_LIST
	#error ENUM_REFLECTION_LIST must be defined before including EnumReflectionGenerator.h!
#endif

#include "EnumReflectionBase.h"

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

namespace Reflection
{
	template<>
	struct EnumReflection<ENUM_REFLECTION_TYPE>
	{
		using EnumType = ENUM_REFLECTION_TYPE;

		static constexpr const char* const TYPE_NAME = STRINGIFY(ENUM_REFLECTION_TYPE);

		static constexpr EnumType VALUES[] = {
#define ENUM_REFLECTION_FIELD_MAPPING(entry, desc) entry,
#define ENUM_REFLECTION_FIELD_MAPPING_ARGS(entry, desc, ...) entry,
			ENUM_REFLECTION_LIST
#undef ENUM_REFLECTION_FIELD_MAPPING_ARGS
#undef ENUM_REFLECTION_FIELD_MAPPING
		};

		static constexpr const char* const NAMES[] = {
#define ENUM_REFLECTION_FIELD_MAPPING(entry, desc) #entry,
#define ENUM_REFLECTION_FIELD_MAPPING_ARGS(entry, desc, ...) #entry,
			ENUM_REFLECTION_LIST
#undef ENUM_REFLECTION_FIELD_MAPPING_ARGS
#undef ENUM_REFLECTION_FIELD_MAPPING
		};

		static constexpr const char* const DESCRIPTIONS[] = {
#define ENUM_REFLECTION_FIELD_MAPPING(entry, desc) desc,
#define ENUM_REFLECTION_FIELD_MAPPING_ARGS(entry, desc, ...) desc,
			ENUM_REFLECTION_LIST
#undef ENUM_REFLECTION_FIELD_MAPPING_ARGS
#undef ENUM_REFLECTION_FIELD_MAPPING
		};
	};
	constexpr EnumReflection<ENUM_REFLECTION_TYPE>::EnumType EnumReflection<ENUM_REFLECTION_TYPE>::VALUES[];
	constexpr const char* const EnumReflection<ENUM_REFLECTION_TYPE>::NAMES[];
	constexpr const char* const EnumReflection<ENUM_REFLECTION_TYPE>::DESCRIPTIONS[];
}  // namespace Reflection

// Undefine these at the end, to be safe.
// If this starts to cause problems in future, we can change it.
#undef ENUM_REFLECTION_TYPE
#undef ENUM_REFLECTION_LIST

#undef STRINGIFY
#undef STRINGIFY_HELPER