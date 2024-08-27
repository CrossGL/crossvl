#pragma once

#ifndef EXCLUDE_STDHEADERS
#include <cassert>
#include <string>
#include <string_view>
#include <memory>
#include <functional>
#include <format>
#include <fstream>
#include <vector>
#include <array>
#include <utility>
#include <typeindex>
#include <cstddef>

#include <Core/Types.h>
#endif

#if defined (CGL_RHI_METAL)
#include "Foundation/Foundation.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "Metal/Metal.hpp"
#endif
