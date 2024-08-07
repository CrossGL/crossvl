set_xmakever("2.9.4")
add_rules("mode.debug", "mode.release")
set_allowedmodes("debug", "release")
set_allowedplats("windows", "linux", "macosx")

-- Set C/C++ language version
set_languages("c17", "cxx23")

-- Build debug mode by default
set_defaultmode("debug")

-- Project name and version
set_project("CrossGLVisualizer")
set_version("0.0.1")

-- Enable all warnings and handle them as compilation errors
set_policy("build.warning", true)
set_warnings("all", "extra")

-- Enable link time optimization
set_policy("build.optimization.lto", true)
-- Allow auto build before running
set_policy("run.autobuild", true)

-- Generate compile_commands.json on build_should_pas
add_rules("plugin.compile_commands.autoupdate")

 -- Include xmake build scripts
 includes("Scripts/Options.lua")
 includes("Scripts/Packages.lua")
 includes("Scripts/Plugins/xmake.lua")


-- Set RHI macros
if has_config("rhi") then
	local rhi = string.upper(get_config("rhi"))
	local macro = "CGL_RHI_" .. rhi
	add_defines(macro)

	if rhi == "DX11" or rhi == "DX12" then
		add_defines("CGL_RHI_D3D")
	end
end

-- Add OS specific macros
if is_os("windows") then
	add_defines("NOMINMAX", "NOMCX", "NOSERVICE", "NOHELP", "WIN32_LEAN_AND_MEAN")
	add_defines("CGL_PLATFORM_WINDOWS")
elseif is_os("linux") then
	add_defines("CGL_PLATFORM_LINUX")
elseif is_os("macosx") then
	add_defines("CGL_PLATFORM_MACOSX")
	add_includedirs("./metal-cpp/")
end

-- Add build config specific macros (and set runtimes)
if is_mode("debug") then
	add_defines("CGL_BUILD_DEBUG", "_DEBUG")
	set_runtimes("MDd")
elseif is_mode("release") then
	add_defines("CGL_BUILD_RELEASE", "NDEBUG")
	set_runtimes("MD")
end

 -- include xmake projects
includes("**/xmake.lua")
