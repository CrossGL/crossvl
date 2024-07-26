function main(argv)
    os.exec(
        "curl -o metalcpp.zip https://developer.apple.com/metal/cpp/files/metal-cpp_macOS14.2_iOS17.2.zip"
    )

    os.exec("unzip ./metalcpp.zip -d .")
    os.exec("clear")

    print("downloaded metal-cpp")
    
    os.exec("rm metalcpp.zip")
end
