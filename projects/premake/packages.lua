
add_package("gucefMT")
add_windows_library_dependency("winmm")

add_application("gucefMT_TestApp")
add_package_dependency("gucefMT")

add_package("gucefCORE")
add_package_dependency("gucefMT")

add_application("gucefCORE_TestApp")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")

add_package("gucefCOMCORE")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_windows_library_dependency("ws2_32")

add_application("gucefCOMCORE_TestApp")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefCOMCORE")

add_package("gucefCOM")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefCOMCORE")

add_application("gucefCOM_TestApp")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefCOMCORE")
add_package_dependency("gucefCOM")

add_package("gucefVFS")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")

add_application("gucefVFS_TestApp")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefVFS")

add_package("gucefIMAGE")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")

add_application("gucefIMAGE_TestApp")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefIMAGE")

add_package("gucefINPUT")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")

add_package("gucefPATCHER")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefVFS")
add_package_dependency("gucefCOMCORE")
add_package_dependency("gucefCOM")

add_package("gucefDRN")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefCOMCORE")

add_application("gucefDRN_TestApp")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
add_package_dependency("gucefCOMCORE")
add_package_dependency("gucefDRN")

add_package("gucefGUI")
add_package_dependency("gucefMT")
add_package_dependency("gucefCORE")
