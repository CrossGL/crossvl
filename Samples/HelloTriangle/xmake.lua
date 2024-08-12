target("HelloTriangle")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(true)
	set_kind("binary")
	set_group("Samples")
	
	add_packages("libsdl", "directxmath")

	add_includedirs("..", "$(projectdir)")
	add_files("**.cpp")
	add_headerfiles("**.h", { install = false })

	add_deps("VisualizerCore")

	-- Throw errors if trying to build using RHI for an unsupported platform
	on_config(function (target)
		if is_plat("macosx", "linux") then
			if has_config("rhi") then
				local rhi = string.upper(get_config("rhi"))
				if rhi == "DX11" or rhi == "DX12" then
					raise("Trying to build for " .. rhi .. " on an unsupported platform!")
				end
			end
		end

		if is_plat("windows", "linux") then
			if has_config("rhi") then
				local rhi = string.upper(get_config("rhi"))
				if rhi == "METAL" then
					raise("Trying to build for " .. rhi .. " on an unsupported platform!")
				end
			end
		end
	end)

	-- Add RHI specific rules
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

		if rhi == "DX11" or rhi == "DX12" then
			add_rules("utils.bin2c", { extensions = { ".hlsl" } })
			add_files("Assets/DirectX/**.hlsl")
		elseif rhi == "VULKAN" or rhi == "OPENGL" then
			add_rules("utils.bin2c", { extensions = { ".vert", ".frag" } })
			if rhi == "OPENGL" then
				add_files("Assets/OpenGL/**.vert", "Assets/OpenGL/**.frag")
			end
		elseif rhi == "METAL" then
			add_rules("utils.bin2c", { extensions = { ".metal" } })
			add_files("Assets/**.metal")
		end
	end

	add_tests("compile_pass", {build_should_pass = true})
	add_tests("run_5_seconds", { runargs = {"-test"} })
target_end()
