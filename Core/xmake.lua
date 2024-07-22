target("VisualizerCore")
	set_default(false)
	set_kind("static")
	set_group("CrossGL")

	add_includedirs("..")
	add_files("**.cpp")
	add_headerfiles("**.h", { install = false })
	add_links("user32.lib")

	add_tests("compile_pass", { build_should_pass = true })
target_end()
