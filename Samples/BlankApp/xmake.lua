target("BlankApp")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(true)
	set_kind("binary")
	set_group("Samples")

	add_packages("libsdl")

	add_includedirs("..", "$(projectdir)")
	add_files("**.cpp")
	add_headerfiles("**.h", { install = false })

	add_deps("VisualizerCore")
	if has_config("rhi") then
		add_links("VisualizerCore" .. "_" .. string.upper(get_config("rhi")))
	end

	add_tests("compile_pass", {build_should_pass = true})
	add_tests("run_5_seconds", { runargs = {"-test"} })
target_end()
