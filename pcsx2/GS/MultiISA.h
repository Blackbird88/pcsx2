/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2021 PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "PCSX2Base.h"

// For multiple-isa compilation
#ifdef MULTI_ISA_UNSHARED_COMPILATION
	// Preprocessor should have MULTI_ISA_UNSHARED_COMPILATION defined to `isa_sse4`, `isa_avx`, or `isa_avx2`
	#define CURRENT_ISA MULTI_ISA_UNSHARED_COMPILATION
#else
	// Define to isa_native in shared section in addition to multi-isa-off so if someone tries to use it they'll hopefully get a linker error and notice
	#define CURRENT_ISA isa_native
#endif

#if defined(MULTI_ISA_UNSHARED_COMPILATION)
	// Preprocessor should have MULTI_ISA_IS_FIRST defined to 0 or 1
	/// Use with `#if MULTI_ISA_COMPILE_ONCE` to make a single definition of something in an otherwise multi-isa-compiled file.
	#define MULTI_ISA_COMPILE_ONCE MULTI_ISA_IS_FIRST
#elif !defined(MULTI_ISA_SHARED_COMPILATION)
	#define MULTI_ISA_COMPILE_ONCE 1
#else
	#define MULTI_ISA_COMPILE_ONCE static_assert(0, "You don't need MULTI_ISA_COMPILE_ONCE in a non-multi-isa file!");
#endif

#ifndef MULTI_ISA_SHARED_COMPILATION
	/// Mark the start of a header defining code that will be compiled multiple times in multi-isa mode
	/// Anything between this and a `MULTI_ISA_UNSHARED_END` will be placed in a different namespace for each of the multilple compilations
	#define MULTI_ISA_UNSHARED_START namespace CURRENT_ISA {
	/// Mark the end of a header defining code that will be compiled multiple times in multi-isa mode
	#define MULTI_ISA_UNSHARED_END }
	/// Mark the beginning of a file implementing things that will be compiled multiple times in multi-isa mode
	/// Takes advantage of the fact that a `using namespace` declaration will also affect any implementations of things as long as they're not valid without it
	/// Fully global variables are valid as-is, however, and will need to have `CURRENT_ISA::` manually prepended to them.
	/// If you forget to do this, it will show up as a linker error (either multiple definitions of the function/variable, or a "failed to find isa_native::function")
	#define MULTI_ISA_UNSHARED_IMPL using namespace CURRENT_ISA
#else
	#define MULTI_ISA_UNSHARED_START static_assert(0, "This file should not be included by multi-isa shared compilation!");
	#define MULTI_ISA_UNSHARED_IMPL static_assert(0, "This file should be compiled unshared in multi-isa mode!");
	#define MULTI_ISA_UNSHARED_END
#endif

struct ProcessorFeatures
{
	enum class VectorISA { None, SSE4, AVX, AVX2 };
	VectorISA vectorISA;
	bool hasFMA;
	bool hasSlowGather;
};

extern const ProcessorFeatures g_cpu;

#if defined(MULTI_ISA_UNSHARED_COMPILATION) || defined(MULTI_ISA_SHARED_COMPILATION)
	#define MULTI_ISA_DEF(...) \
		namespace isa_sse4 { __VA_ARGS__ } \
		namespace isa_avx  { __VA_ARGS__ } \
		namespace isa_avx2 { __VA_ARGS__ }

	#define MULTI_ISA_FRIEND(klass) \
		friend class isa_sse4::klass; \
		friend class isa_avx ::klass; \
		friend class isa_avx2::klass;

	#define MULTI_ISA_SELECT(fn) (\
		::g_cpu.vectorISA == ProcessorFeatures::VectorISA::AVX2 ? isa_avx2::fn : \
		::g_cpu.vectorISA == ProcessorFeatures::VectorISA::AVX  ? isa_avx ::fn : \
		                                                          isa_sse4::fn)
#else
	#define MULTI_ISA_DEF(...) namespace isa_native { __VA_ARGS__ }
	#define MULTI_ISA_FRIEND(klass) friend class isa_native::klass;
	#define MULTI_ISA_SELECT(fn) (isa_native::fn)
#endif