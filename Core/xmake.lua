target("VisualizerCore")
	if has_config("rhi") then
		set_suffixname("_" .. string.upper(get_config("rhi")))
	end

	set_default(false)
	set_kind("static")
	set_group("CrossGL")

	add_packages("libsdl", "dxmath")
	set_options("rhi")

	add_includedirs("..")

	before_build(function (target)
		import("core.project.config")

		local rhi = string.upper(get_config("rhi"))
		local plat = string.upper(config.get("plat"))
		cprint(format("Configuring Visualizer Core using:\n${cyan}RHI: ${clear}%s\n${cyan}Platform: ${clear}%s", rhi, plat))
		end)

	-- Recursively add all files except:
	-- -> Files in the Platform folder
	-- -> Files in the Graphics/RHI folder
	add_files("**.cpp|Platform/**.cpp|Graphics/RHI/**.cpp")
	add_headerfiles("**.h|Platform/**.h|Graphics/RHI/**.h")

	-- Add platform settings
	if is_os("windows") then
		add_files("Platform/Win32/**.cpp")
		add_headerfiles("Platform/Win32/**.h")
		add_links("user32.lib")
	elseif is_os("linux") then
		add_files("Platform/Linux/**.cpp")
		add_headerfiles("Platform/Linux/**.h")
		add_packages("ncurses")
		add_links("ncurses")
	elseif is_os("macosx") then
		add_files("Platform/MacOSX/**.cpp")
		add_headerfiles("Platform/MacOSX/**.h")
		add_frameworks("Foundation", "Metal", "MetalFX", "QuartzCore")
		add_packages("ncurses")
		add_links("ncurses")
	end

	-- Add RHI files
	if has_config("rhi") then
		local rhi = string.upper(get_config("rhi"))

		if rhi == "DX11" or rhi == "DX12" then
			add_links("dxgi.lib", "d3dcompiler.lib", "dxguid.lib")
			add_headerfiles("Graphics/RHI/D3DCommon.h")

			if rhi == "DX11" then
				add_files("Graphics/RHI/D3D11/**.cpp")
				add_headerfiles("Graphics/RHI/D3D11/**.h")
				add_links("d3d11.lib")
			elseif rhi == "DX12" then
				add_files("Graphics/RHI/D3D12/**.cpp")
				add_headerfiles("Graphics/RHI/D3D12/**.h")
				add_links("d3d12.lib")
			end
		elseif rhi == "OPENGL" then
			add_files("Graphics/RHI/OpenGL/**.cpp")
			add_headerfiles("Graphics/RHI/OpenGL/**.h")
			add_packages("glew")
			add_links("GL")
		elseif rhi == "VULKAN" then
			add_files("Graphics/RHI/Vulkan/**.cpp")
			add_headerfiles("Graphics/RHI/Vulkan/**.h")
		elseif rhi == "METAL" then
			add_files("Graphics/RHI/Metal/**.cpp")
			add_headerfiles("Graphics/RHI/Metal/**.h")
		end
	end

	add_tests("compile_pass", { build_should_pass = true })
target_end()
