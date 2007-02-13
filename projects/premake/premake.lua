project_name = "GUCEF"
dofile("project_setup.lua")

package = newpackage()
package_name = "gucefMT"
dofile("package_setup.lua")


package = newpackage()
package_name = "gucefCORE"
dofile("package_setup.lua")
table.insert(package.includepaths, 2, "../../../gucefMT/include")