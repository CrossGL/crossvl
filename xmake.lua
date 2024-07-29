set_xmakever("2.9.2")
add_rules("mode.debug", "mode.release")
set_allowedmodes("debug", "release")

-- Set C/C++ language version
set_languages("c17", "cxx23")

-- Project name and version
set_project("CrossGLVisualizer")
set_version("0.0.1")

-- Enable all warnings and handle them as compilation errors
set_policy("build.warning", true)
set_warnings("all", "extra")

-- Enable link time optimization
set_policy("build.optimization.lto", true)
-- Install packages locally instead of system cache
set_policy("package.install_locally", true)
-- Allow auto build before running
set_policy("run.autobuild", true)
-- Generate compile_commands.json on build_should_pas
add_rules("plugin.compile_commands.autoupdate")

-- user debug version of packages
local use_package_debug = is_mode("debug")

if is_os("windows") then
	add_defines("NOMINMAX", "NOMCX", "NOSERVICE", "NOHELP", "WIN32_LEAN_AND_MEAN")
	add_defines("CGL_PLATFORM_WINDOWS")
elseif is_os("linux") then
	add_defines("CGL_PLATFORM_LINUX")
	add_requires("ncurses",
	{
		debug = use_package_debug
	})
elseif is_os("macosx") then
	add_defines("CGL_PLATFORM_MACOSX")
	add_includedirs("./metal-cpp/")
end

if is_mode("debug") then
	add_defines("CGL_BUILD_DEBUG", "_DEBUG")
	set_runtimes("MDd")
elseif is_mode("release") then
	add_defines("CGL_BUILD_RELEASE", "NDEBUG")
	set_runtimes("MD")
end

-- Use SDL2 library
add_requires("libsdl",
	{
		debug =	use_package_debug,
		configs =
		{
			sdlmain = false
		}
	})

-- Add DirectXMath library
-- add_requires("directxmath",
-- 	{
-- 		debug =	use_package_debug,
-- 		configs =
-- 		{
-- 			lto = true,
-- 		}
-- 	})

option("rhi")
	set_description("Set Graphics API",
					"opengl",
					"dx11",
					"dx12",
					"vulkan",
					"metal")

	set_showmenu(true)
	set_values("opengl", "dx11", "dx12", "vulkan", "metal")
	set_default("dx11")
	set_category("CrossGL")
option_end()

if has_config("rhi") then
	local type =  get_config("rhi")
	local macro = "CGL_RHI_" .. string.upper(type)
	add_defines(macro)
end


-- Custom directxmath package that supports mac
package("dxmath")

	set_kind("library", {headeronly = true})
	set_homepage("https://github.com/microsoft/DirectXMath")
	set_description("DirectXMath is an all inline SIMD C++ linear algebra library for use in games and graphics apps.")
	set_license("MIT")

	local tag = {
		["2022.12"] = "dec2022",
		["2024.02"] = "feb2024"
	}
	add_urls("https://github.com/microsoft/DirectXMath/archive/refs/tags/$(version).zip", {version = function (version) return tag[tostring(version)] end})
	add_urls("https://github.com/microsoft/DirectXMath.git")
	add_versions("2022.12", "2ed0ae7d7fe5d11ad11f6d3d9b31ce686024a551cf82ade723de86aa7b4b57e1")
	add_versions("2024.02", "214d71420107249dfb4bbc37a573f288b0951cc9ffe323dbf662101f3df4d766")

	if is_plat("linux", "macosx") then
		add_resources(">=2022.12", "headers", "https://raw.githubusercontent.com/dotnet/runtime/2201016c1e13bdb9abf49e2e38cadf4ee0568df2/src/coreclr/pal/inc/rt/sal.h", "7dae281adc3a09a691291fb90526f05e4f9ef8b16d7f33d716ba690f7241a492")
	end

	add_deps("cmake")
	add_includedirs("include/directxmath")

	on_install("windows", "mingw", "linux", "macosx", function (package)
		if package:is_plat("linux", "macosx") then
			os.cp("../resources/headers/sal.h", package:installdir("include", "directxmath"))
		end
		import("package.tools.cmake").install(package, {"-DBUILD_TESTING=OFF"})
	end)

	on_test(function (package)
		assert(package:check_cxxsnippets({test = [[
			void test() {
				DirectX::XMVECTOR v = DirectX::XMVectorSet(1.0f, 2.0f, 3.0f, 4.0f);
				DirectX::XMMATRIX m = DirectX::XMMatrixIdentity();
				DirectX::XMVECTOR vResult = DirectX::XMVector4Transform(v, m);
			}
		]]}, {configs = {languages = "cxx17"}, includes = "DirectXMath.h"}))
	end)
package_end()

add_requires("dxmath",
	{
		debug =	use_package_debug,
		configs =
		{
			lto = true,
		}
	})

includes("**/xmake.lua")
