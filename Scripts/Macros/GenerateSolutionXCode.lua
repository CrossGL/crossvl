function main(argv)
	print("Generating solution files for XCode...")
    os.exec("xmake project -k xcode -y")
end
