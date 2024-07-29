import("core.project.config")
import("core.base.option")

function set_mode(mode)
    os.exec("xmake f -m " .. mode .. " -y -q")
end

function set_rhi(rhi)
    os.exec("xmake f --rhi=" .. rhi .. " -y -q")
end

function run_test(rhilist)
    for _, rhi in ipairs(rhilist) do
        cprint("\n${blue}Testing for " .. rhi .. " config...${clear}")
        set_rhi(rhi)
        os.exec("xmake test -y")
    end
end

function main()
    local platform = string.upper(option.get("platform"))
    local modes = { "release", "debug" }

    for _, mode in ipairs(modes) do
        cprint("\n\n${magenta}Testing for " .. mode .. " config...${clear}")
        set_mode(mode)

        if platform == "WINDOWS" then
            local rhis = { "dx11", "dx12", "opengl", "vulkan" }
            run_test(rhis)
        elseif platform == "LINUX" then
            local rhis = { "opengl", "vulkan" }
            run_test(rhis)
        elseif platform == "MACOSX" then
            local rhis = { "opengl", "metal", "vulkan" }
            run_test(rhis)
        end
    end
end