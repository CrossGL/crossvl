import("core.base.option")

projectName   = nil
templatesPath = nil
rootDir       = nil
projectDir    = nil  -- Samples project directory

function CreateXmakeFile()
	-- Get file paths
	local templateFilePath = path.join(templatesPath, "XmakeTemplate")
	local outputFilePath = path.join(projectDir, "xmake.lua")

	 -- Get template file contents
	 local templateFile = io.open(templateFilePath, "r")
	 if not templateFile then
			raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
	 end
	 local templateContent = templateFile:read("*all")
	 templateFile:close()

	 -- Replace '%PROJECT_NAME%' with the actual project name
	 local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

	 -- Create and write to the output file
	local outputFile = io.open(outputFilePath, "w")
	if not outputFile then
		error("Failed to write xmake project file. Expected path: " .. outputFilePath)
	end
	outputFile:write(modifiedContent)
	outputFile:close()

	cprint("Created project file: ${underline}%s", outputFilePath)
end

function CreateMainCpp()
	-- Get file paths
	local templateFilePath = path.join(templatesPath, "MainTemplate")
	local outputFilePath = path.join(projectDir, "Main.cpp")

	-- Get template file contents
	local templateFile = io.open(templateFilePath, "r")
	if not templateFile then
		raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
	end
	local templateContent = templateFile:read("*all")
	templateFile:close()

	-- Replace '%PROJECT_NAME%' with the actual project name
	local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

	-- Create and write to the output file
	local outputFile = io.open(outputFilePath, "w")
	if not outputFile then
		error("Failed to write xmake project file. Expected path: " .. outputFilePath)
	end
	outputFile:write(modifiedContent)
	outputFile:close()

	cprint("Created main file: ${underline}%s", outputFilePath)
end

function CreateApplicationCpp()
	-- Get file paths
	local templateFilePath = path.join(templatesPath, "AppCppTemplate")
	local outputFilePath = path.join(projectDir,projectName .. ".cpp")

	-- Get template file contents
	local templateFile = io.open(templateFilePath, "r")
	if not templateFile then
		raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
	end
	local templateContent = templateFile:read("*all")
	templateFile:close()

	-- Replace '%PROJECT_NAME%' with the actual project name
	local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

	-- Create and write to the output file
	local outputFile = io.open(outputFilePath, "w")
	if not outputFile then
		error("Failed to write xmake project file. Expected path: " .. outputFilePath)
	end
	outputFile:write(modifiedContent)
	outputFile:close()

	cprint("Created application cpp file: ${underline}%s", outputFilePath)
end

function CreateApplicationHeader()
	-- Get file paths
	local templateFilePath = path.join(templatesPath, "AppHeaderTemplate")
	local outputFilePath = path.join(projectDir, projectName .. ".h")

	-- Get template file contents
	local templateFile = io.open(templateFilePath, "r")
	if not templateFile then
		raise("Failed to open xmake template file. Expected path: " .. templateFilePath)
	end
	local templateContent = templateFile:read("*all")
	templateFile:close()

	-- Replace '%PROJECT_NAME%' with the actual project name
	local modifiedContent = templateContent:gsub("%%PROJECT_NAME%%", projectName)

	-- Create and write to the output file
	local outputFile = io.open(outputFilePath, "w")
	if not outputFile then
		error("Failed to write xmake project file. Expected path: " .. outputFilePath)
	end
	outputFile:write(modifiedContent)
	outputFile:close()

	cprint("Created application header file: ${underline}%s", outputFilePath)
end

function CreateAssetsFolder()
	-- Get asset directory
	local assetDir = path.join(projectDir, "Assets")

	-- Create assets folder
	os.mkdir(assetDir)

	cprint("Created assets directory: ${underline}%s", assetDir)
end

function main()
	-- Get parameter content and display information
	projectName = option.get("project")

	-- Get current project directory
	rootDir = os.projectdir()

	-- Get templates path
	templatesPath = path.join(rootDir, "Scripts", "Templates")

	-- Get project directory
	projectDir = path.join(rootDir, "Samples", projectName)

	print("Creating sample: " .. projectName)
	print("Templates path: " .. templatesPath)
	print("Project directory: " .. projectDir)

	CreateXmakeFile()
	CreateMainCpp()
	CreateApplicationCpp()
	CreateApplicationHeader()
	CreateAssetsFolder()

	cprint("${green}Done")
end