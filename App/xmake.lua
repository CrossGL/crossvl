target("VisualizerApp")
	set_default(true)
	set_kind("binary")
	set_group("CrossGL")

	add_packages("libsdl")

	add_includedirs("..")
	add_files("**.cpp")
	add_headerfiles("**.h", { install = false })

	add_deps("VisualizerCore")
	add_links("VisualizerCore")
	
	add_tests("compile_pass", {build_should_pass = true})
target_end()
