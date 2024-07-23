target("VisualizerCore")
	set_default(false)
	set_kind("static")
	set_group("CrossGL")

	add_includedirs("..")

	if is_os("windows") then 
		add_files("**.cpp|Platform/Linux/**.cpp|Platform/MacOSX/**.cpp")
		add_headerfiles("**.h|Platform/Linux/**.h|Platform/MacOSX/**.hpp", { install = false })
		add_links("user32.lib")

	elseif is_os("linux") then 
		add_files("**.cpp|Platform/Win32/**.cpp|Platform/MacOSX/**.cpp")
		add_headerfiles("**.h|Platform/Win32/**.h|Platform/MacOSX/**.hpp", { install = false })

	elseif is_os("macosx") then
		add_packages("sdl2")
		add_includedirs("/usr/local/include/")
		add_files("**.cpp|Platform/Linux/**.cpp|Platform/Win32/**.cpp")
		add_headerfiles("**.h|Platform/Linux/**.h|Platform/Win32/**.hpp", { install = false })

		add_linkdirs("/usr/local/lib/")
		add_links("SDL2")
	end

	add_tests("compile_pass", { build_should_pass = true })
target_end()
