package.name = "gucefMT"
package.kind = "dll"
package.language = "c"

package.files = {
    matchfiles("../../include/*.h", "../../src/*.c", "../../src/*.cpp")
}

package.includepaths = { "../../include", "../../../common/include" }

if(windows) then
package.links = { "winmm" }
else
end

package.config["Debug"].bindir = "../../bin/Debug_"..target
package.config["Debug"].objdir = "../../bin/Debug_"..target
package.config["Release"].bindir = "../../bin/Release_"..target
package.config["Release"].objdir = "../../bin/Release_"..target

package.defines = { "GUCEFMT_BUILD_MODULE", "GUCEFMT_VC8_EXPORTS", "_WINDLL", "_UNICODE", "UNICODE" }