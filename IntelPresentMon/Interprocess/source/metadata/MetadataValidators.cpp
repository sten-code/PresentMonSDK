#include "../IntrospectionMacroHelpers.h"
#include "../IntrospectionMetadata.h"

namespace pmon::ipc::intro
{
	namespace {
		// function not reference or called, but compiling this allows compiler to check if all enums/enum keys have coverage
		bool ValidateEnumCompleteness()
		{
			// validate each registered enum's keys
#pragma warning(push)
#pragma warning(disable : 4060) // empty enum for NULL enum triggers this
#define X_REG_KEYS(enum_frag, key_frag, name, short_name, description) case MAKE_KEY_SYMBOL(enum_frag, key_frag): return false;
#define X_REG_ENUMS(master_frag, enum_frag, name, short_name, description) \
			switch (MAKE_ENUM_SYMBOL(enum_frag)(0)) { \
				MAKE_LIST_SYMBOL(enum_frag)(X_REG_KEYS) \
			}

			ENUM_KEY_LIST_ENUM(X_REG_ENUMS)

#undef X_REG_ENUMS
#undef X_REG_KEYS
#pragma warning(pop)

				// validate the enumeration that records introspectable enums
#define X_REG_KEYS(enum_frag, key_frag, name, short_name, description) case MAKE_KEY_SYMBOL(enum_frag, key_frag): return false;
			switch (PM_ENUM(0)) {
				ENUM_KEY_LIST_ENUM(X_REG_KEYS)
			}
#undef X_REG_KEYS

			// validate the list of metrics against the metrics enum
#define X_REG_METRICS(metric, ...) case metric: return false;
			switch (PM_METRIC(0)) {
				METRIC_LIST(X_REG_METRICS)
			}
#undef X_REG_METRICS

			return true;
		}
	}
}