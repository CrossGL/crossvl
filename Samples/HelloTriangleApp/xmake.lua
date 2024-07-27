target("HelloTriangleApp")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(false)
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

	on_config (function (target)
			local asset_dir = path.join("$(scriptdir)", "Assets")
			local macro = "CGL_ASSET_DIR=".. "\"" .. asset_dir .. "\""
			target:add("defines", macro)
		end)

	add_tests("compile_pass", {build_should_pass = true})
target_end()
