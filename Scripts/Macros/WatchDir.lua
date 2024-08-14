function main(argv)
	print("Enabling xmake watch...")
	os.exec("xmake watch -d \"Core/*;Samples/*;Scripts/*\"")
end
