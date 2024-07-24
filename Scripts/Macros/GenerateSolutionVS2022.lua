function main(argv)
	print("Generating solution files for Visual Studio 2022...")
    os.exec("xmake project -k vsxmake2022 -y")
end
