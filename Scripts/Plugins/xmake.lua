-- Run tests on all build configurations for every RHI
task("rhitest")
	set_category("plugin")
	on_run("TestRHI")

    -- Set the command line options for the plugin. There are no parameter options here, just the plugin description.
    set_menu
    {
        -- Settings menu usage
        usage = "xmake nui-project [project-name]",
        -- Setup menu description
        description = "Run compile test on all build configurations for every RHI",

        -- No options for this task
        options =
        {
            {'p', "platform", "kv", "Operating System", "The platform to test" }
        }
    }
task_end()