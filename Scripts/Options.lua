-- Defines the option to toggle build API
option("rhi")
	set_description("Set Graphics API",
					"opengl",
					"dx11",
					"dx12",
					"vulkan",
					"metal")

	set_showmenu(true)
	set_values("opengl", "dx11", "dx12", "vulkan", "metal")
	set_default("dx11")
	set_category("CrossGL")
option_end()

if has_config("rhi") then
	local type =  get_config("rhi")
	local macro = "CGL_RHI_" .. string.upper(type)
	add_defines(macro)
end
