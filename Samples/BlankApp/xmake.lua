target("BlankApp")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(true)
	set_kind("binary")
	set_group("Samples")

	add_rules("RHICompat")
	add_packages("libsdl", "directxmath", "tinyobjloader")

	add_includedirs("..", "$(projectdir)")
	add_files("**.cpp")
	add_headerfiles("**.h", { install = false })

	add_deps("VisualizerCore")

	if has_config("rhi") then
		local rhi = string.upper(get_config("rhi"))
		add_links("VisualizerCore" .. "_" .. rhi)

		if rhi == "OPENGL" then
			add_packages("glew")
			if is_plat("windows") then
				add_links("opengl32")
			elseif is_plat("linux") then
				add_links("GL")
			end
		end
	end

	add_tests("compile_pass", {build_should_pass = true})
target_end()
