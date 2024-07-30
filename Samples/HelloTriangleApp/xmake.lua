target("HelloTriangleApp")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(false)
	set_kind("binary")
	set_group("Samples")
	
	add_packages("libsdl")

	add_includedirs("..", "$(projectdir)")
	add_files("**.cpp", "Assets/**.hlsl")
	add_headerfiles("**.h", { install = false })
	add_rules("utils.bin2c", {extensions = {".hlsl"}})

	add_deps("VisualizerCore")
	if has_config("rhi") then
		add_links("VisualizerCore" .. "_" .. string.upper(get_config("rhi")))
	end

	add_tests("compile_pass", {build_should_pass = true})
target_end()
