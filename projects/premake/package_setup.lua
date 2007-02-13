package.name = package_name.."_"..compiler
package.path = "../../"..package_name.."/projects/"..compiler
package.kind = "dll"
package.language = "c"

package.files = {
    matchfiles("../../include/*.h", "../../src/*.c", "../../src/*.cpp")
}

package.includepaths = { "../../include", "../../../common/include" }

if(windows) then
package.links = { "winmm" }
end

package.config["Debug"].bindir = "../../bin/Debug_"..compiler
package.config["Debug"].objdir = "../../bin/Debug_"..compiler

package.config["Debug"].defines = { "DEBUG", "_DEBUG" }
package.config["Debug"].target = package_name.."_"..compiler.."_d"

package.config["Release"].bindir = "../../bin/Release_"..compiler
package.config["Release"].objdir = "../../bin/Release_"..compiler

package.config["Release"].target = package_name.."_"..compiler

package.defines = global_defines

table.insert(package.defines, string.upper(package_name).."_BUILD_MODULE")

if(compiler == "MVC8") then
    table.insert(package.defines, string.upper(package_name).."_VC8_EXPORTS")
end
