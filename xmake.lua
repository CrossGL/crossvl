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

if is_os("windows") then
	add_defines("NOMINMAX", "NOMCX", "NOSERVICE", "NOHELP", "WIN32_LEAN_AND_MEAN")
	add_defines("CGL_PLATFORM_WINDOWS")
elseif is_os("linux") then
	add_defines("CGL_PLATFORM_LINUX")
elseif is_os("macosx") then
	add_defines("CGL_PLATFORM_MACOSX")
end

if is_mode("debug") then
	add_defines("CGL_BUILD_DEBUG", "_DEBUG")
	set_runtimes("MDd")
elseif is_mode("release") then
	add_defines("CGL_BUILD_RELEASE", "NDEBUG")
	set_runtimes("MD")
end

includes("**/xmake.lua")
