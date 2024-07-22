function main(argv)
	print("Cleaning configs...")
    os.exec("xmake f -c -q")
	print("Cleaning targets...")
    os.exec("xmake clean -a -q")
    -- Set xmake theme (optional)
    os.exec("xmake g -q --theme=powershell")
end
