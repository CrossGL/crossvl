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
	add_files("**.cpp")
	add_headerfiles("**.h", { install = false })
	add_links("user32.lib")

	add_tests("compile_pass", { build_should_pass = true })
target_end()
