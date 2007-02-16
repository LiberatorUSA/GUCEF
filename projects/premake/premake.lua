dofile("functions.lua")

project_setup("GUCEF")

add_package("gucefMT")
add_lib("winmm")

add_package("gucefCORE")
depends_on_package("gucefMT")

add_package("gucefCOMCORE")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")
add_lib("ws2_32")

add_package("gucefCOM")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")
depends_on_package("gucefCOMCORE")

add_package("gucefVFS")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")

add_package("gucefIMAGE")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")

add_package("gucefINPUT")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")

add_package("gucefPATCHER")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")
depends_on_package("gucefVFS")
depends_on_package("gucefCOMCORE")
depends_on_package("gucefCOM")