dofile("functions.lua")

project_setup("GUCEF")

add_package("gucefMT")

add_package("gucefCORE")
depends_on_package("gucefMT")

add_package("gucefCOMCORE")
depends_on_package("gucefMT")
depends_on_package("gucefCORE")
