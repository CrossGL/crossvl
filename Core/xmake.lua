target("VisualizerCore")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(false)
	set_kind("static")
	set_group("CrossGL")

	add_packages("libsdl")
	set_options("rhi")

	add_includedirs("..")

	on_config(function (target)
		import("core.project.config")
		local msg = format("Using RHI: %s\nUsing platform: %s", string.upper(get_config("rhi")), string.upper(config.get("plat")))
		print(msg)
		end)

	if is_os("windows") then
	    add_files("**.cpp|Platform/Linux/**.cpp|Platform/MacOSX/**.cpp|Graphics/RHI/Metal/**.cpp")
	    add_headerfiles("**.h|Platform/Linux/**.h|Platform/MacOSX/**.hpp", { install = false })
	    add_links("user32.lib")
	elseif is_os("linux") then
		add_packages("glew")
	    add_packages("ncurses")
	    add_files("**.cpp|Platform/Win32/**.cpp|Platform/MacOSX/**.cpp|Graphics/RHI/**.cpp", "Graphics/RHI/OPENGL/**.cpp")
	    add_headerfiles("**.h|Platform/Win32/**.h|Platform/MacOSX/**.hpp", { install = false })

	    add_links("GL")
	    add_links("ncurses")

	elseif is_os("macosx") then
		add_packages("ncurses")
	    add_files(
			"**.cpp|Platform/Linux/**.cpp|Platform/Win32/**.cpp|Graphics/RHI/**.cpp",
		    "Graphics/RHI/Metal/**.cpp"
		)
	    add_headerfiles("**.h|Platform/Linux/**.h|Platform/Win32/**.hpp", { install = false })

		add_links("ncurses")
	    add_frameworks("Foundation", "Metal", "MetalFX", "QuartzCore")
	end

	add_tests("compile_pass", { build_should_pass = true })
target_end()
