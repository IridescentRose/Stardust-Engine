#pragma once

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspmath.h>
#endif

namespace Stardust::Math {
#if CURRENT_PLATFORM == PLATFORM_PSP

	inline void srand(unsigned int x) { vfpu_srand(x); }
	inline auto randf(float min, float max) { return vfpu_randf(min, max); }
	inline auto sinf(float x)  -> float{ return vfpu_sinf(x); }
	inline auto cosf(float x)  -> float{ return vfpu_cosf(x); }
	inline auto tanf(float x)  -> float{ return vfpu_tanf(x); }
	inline auto asinf(float x) -> float { return vfpu_asinf(x); }
	inline auto acosf(float x) -> float { return vfpu_acosf(x); }
	inline auto atanf(float x) -> float { return vfpu_atanf(x); }
	inline auto atan2f(float x, float y) -> float{ return vfpu_atan2f(x, y); }
	inline auto sinhf(float x) -> float { return vfpu_sinhf(x); }
	inline auto coshf(float x) -> float { return vfpu_coshf(x); }
	inline auto tanhf(float x) -> float { return vfpu_tanhf(x); }
	inline auto expf(float x)  -> float{ return vfpu_expf(x); }
	inline auto logf(float x)  -> float{ return vfpu_logf(x); }
	inline auto powf(float x, float y) -> float { return vfpu_powf(x, y); }
	inline auto fmodf(float x, float y) -> float { return vfpu_fmodf(x, y); }

#endif
}
