-- Contains project package definitions

-- Use debug version of packages?
local use_package_debug = is_mode("debug")

-- Use DirectXMath package
add_requires("directxmath",
	{
		debug =	use_package_debug,
		configs =
		{
			lto = true,  -- Enable link time optimization
		}
	})

-- Use SDL2 for all projects
add_requires("libsdl",
	{
		debug =	use_package_debug,
		configs =
		{
			sdlmain = false
		}
	})


-- Include the GLEW package when building with OpenGL RHI on any operating system
if has_config("rhi") then
	local rhi = string.upper(get_config("rhi"))
	if rhi == "OPENGL" then
		add_requires("glew")
	end
end

if is_os("linux") then
	add_requires("ncurses", { debug = use_package_debug })
end
