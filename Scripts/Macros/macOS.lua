function main(argv)
    os.exec(
        "curl -o ./Core/Platform/MacOSX/metalcpp.zip https://developer.apple.com/metal/cpp/files/metal-cpp_macOS14.2_iOS17.2.zip"
    )

    print("extracting metal-cpp")

    os.exec("unzip ./Core/Platform/MacOSX/metalcpp.zip -d ./Core/Platform/MacOSX/")

    os.exec("rm ./Core/Platform/MacOSX/metalcpp.zip")
end
