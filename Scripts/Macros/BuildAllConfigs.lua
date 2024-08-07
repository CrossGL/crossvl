function main(argv)
	print("-->Building release config")
	os.exec("xmake f -m release -y")
	os.exec("xmake build -a -y")

	print("\n\n-->Building debug config")
	os.exec("xmake f -m debug -y")
	os.exec("xmake build -a -y")
end
