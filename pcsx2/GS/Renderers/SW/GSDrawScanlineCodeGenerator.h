/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2023 PCSX2 Dev Team
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

#include "GS/Renderers/SW/GSScanlineEnvironment.h"
#include "GS/Renderers/SW/GSNewCodeGenerator.h"
#include "GS/GSUtil.h"
#include "GS/MultiISA.h"

MULTI_ISA_UNSHARED_START

class GSDrawScanlineCodeGenerator : public Xbyak::CodeGenerator
{
	GSDrawScanlineCodeGenerator(const GSDrawScanlineCodeGenerator&) = delete;
	void operator=(const GSDrawScanlineCodeGenerator&) = delete;

	GSScanlineSelector m_sel;

public:
	GSDrawScanlineCodeGenerator(u64 key, void* code, size_t maxsize);
};

MULTI_ISA_UNSHARED_END
