dofile("functions.lua")

project_setup("GUCEF")

add_package("gucefMT")
add_library_dependancy("winmm")

add_package("gucefCORE")
add_package_dependancy("gucefMT")

add_package("gucefCOMCORE")
add_package_dependancy("gucefMT")
add_package_dependancy("gucefCORE")
add_library_dependancy("ws2_32")

add_package("gucefCOM")
add_package_dependancy("gucefMT")
add_package_dependancy("gucefCORE")
add_package_dependancy("gucefCOMCORE")

add_package("gucefVFS")
add_package_dependancy("gucefMT")
add_package_dependancy("gucefCORE")

add_package("gucefIMAGE")
add_package_dependancy("gucefMT")
add_package_dependancy("gucefCORE")

add_package("gucefINPUT")
add_package_dependancy("gucefMT")
add_package_dependancy("gucefCORE")

add_package("gucefPATCHER")
add_package_dependancy("gucefMT")
add_package_dependancy("gucefCORE")
add_package_dependancy("gucefVFS")
add_package_dependancy("gucefCOMCORE")
add_package_dependancy("gucefCOM")