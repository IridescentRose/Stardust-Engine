#pragma once

#if CURRENT_PLATFORM == PLATFORM_PSP
#include <pspmath.h>
#endif

namespace Stardust::Math {
#if CURRENT_PLATFORM == PLATFORM_PSP

	inline void srand(unsigned int x) { vfpu_srand(x); }
	inline float randf(float min, float max) { return vfpu_randf(min, max); }
	inline float sinf(float x) { return vfpu_sinf(x); }
	inline float cosf(float x) { return vfpu_cosf(x); }
	inline float tanf(float x) { return vfpu_tanf(x); }
	inline float asinf(float x) { return vfpu_asinf(x); }
	inline float acosf(float x) { return vfpu_acosf(x); }
	inline float atanf(float x) { return vfpu_atanf(x); }
	inline float atan2f(float x, float y) { return vfpu_atan2f(x, y); }
	inline float sinhf(float x) { return vfpu_sinhf(x); }
	inline float coshf(float x) { return vfpu_coshf(x); }
	inline float tanhf(float x) { return vfpu_tanhf(x); }
	inline float expf(float x) { return vfpu_expf(x); }
	inline float logf(float x) { return vfpu_logf(x); }
	inline float powf(float x, float y) { return vfpu_powf(x, y); }
	inline float fmodf(float x, float y) { return vfpu_fmodf(x, y); }

#endif
}