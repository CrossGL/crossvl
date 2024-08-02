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

-- Create new sample
task("cgl-create")
	set_category("action")
	on_run("CreateSample")

    -- Set the command line options for the plugin. There are no parameter options here, just the plugin description.
    set_menu
    {
        -- Settings menu usage
        usage = "xmake cgl-create [sample-name]",
        -- Setup menu description
        description = "Create a new CGL sample app project",

        -- Set menu options, if there are no options, you can set it to {}
        options =
        {
            -- Set kv as the key-value parameter and set the default value: black
            {'p', "project", "kv", "CGLSampleApp", "Set the app name." }
        }
    }
task_end()