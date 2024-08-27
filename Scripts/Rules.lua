-- Rule to check for RHI-OS compatibility
rule("RHICompat")
    on_config(function (target)
        if is_plat("macosx", "linux") then
			if has_config("rhi") then
				local rhi = string.upper(get_config("rhi"))
				if rhi == "DX11" or rhi == "DX12" then
					raise("RULE Trying to build for " .. rhi .. " on an unsupported platform!")
				end
			end
		end

		if is_plat("windows", "linux") then
			if has_config("rhi") then
				local rhi = string.upper(get_config("rhi"))
				if rhi == "METAL" then
					raise("RULE Trying to build for " .. rhi .. " on an unsupported platform!")
				end
			end
		end
	end)
rule_end()

-- Rule to copy obj files
rule("CopyOBJ")
    after_build(function (target)
        cprint("Copying .obj files to build directory...")
        local assetdir = path.join(target:scriptdir(), "Assets", "**.obj")
        local copydir = path.join(path.directory(target:targetfile()), "Models/")

        if not os.isdir(copydir) then
            os.mkdir(copydir)
        end

        os.cp(assetdir, copydir)
    end)
rule_end()