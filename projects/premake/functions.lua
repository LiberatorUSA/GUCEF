function project_setup(project_name)
    to_compiler = { ["vs2005"] = "MVC8", ["vs2003"] = "MVC7.1", ["vs2002"] = "MVC7", ["vs6"] = "MVC6" }

    if(to_compiler[target]) then
        compiler = to_compiler[target]
    else
        compiler = target
    end

    global_defines = { "_UNICODE", "UNICODE" }

    if(windows) then
        additional_defines = { "_WINDLL", "WIN32", "_WINDOWS", "_USRDLL" }
    end

    for k,v in pairs(additional_defines) do table.insert(global_defines, v) end

    project.name = project_name.."_"..compiler
    project.path = "../"..compiler
end

function add_package(package_name)
    package = newpackage()

    package.name = package_name.."_"..compiler
    package.path = "../../"..package_name.."/projects/"..compiler
    package.kind = "dll"
    package.language = "c"

    package.files = {
        matchrecursive("../../include/*.h", "../../src/*.c", "../../src/*.cpp", "../../dependancy/*.c", "../../dependancy/*.cpp", "../../dependancy/*.h")
    }

    package.includepaths = { "../../include", "../../dependancy", "../../../common/include" }

    if(windows) then
    package.links = { "" }
    end

    package.config["Debug"].bindir = "../../bin/Debug_"..compiler
    package.config["Debug"].libdir = "../../bin/Debug_"..compiler
    package.config["Debug"].objdir = "../../bin/Debug_"..compiler

    package.config["Debug"].defines = { "DEBUG", "_DEBUG" }
    package.config["Debug"].target = package_name.."_"..compiler.."_d"

    package.config["Release"].bindir = "../../bin/Release_"..compiler
    package.config["Release"].libdir = "../../bin/Release_"..compiler
    package.config["Release"].objdir = "../../bin/Release_"..compiler

    package.config["Release"].target = package_name.."_"..compiler

    for k,v in pairs(global_defines) do table.insert(package.defines, v) end

    table.insert(package.defines, string.upper(package_name).."_BUILD_MODULE")

    if(compiler == "MVC8") then
        table.insert(package.defines, string.upper(package_name).."_VC8_EXPORTS")
    end
end

function add_package_dependancy(package_name)
    table.insert(package.includepaths, 3, "../../../"..package_name.."/include")
    table.insert(package.config["Debug"].libpaths, "../../../"..package_name.."/bin/Debug_"..compiler)
    table.insert(package.config["Debug"].links, package_name.."_"..compiler.."_d")
    table.insert(package.config["Release"].libpaths, "../../../"..package_name.."/bin/Release_"..compiler)
    table.insert(package.config["Release"].links, package_name.."_"..compiler)
end

function add_library_dependancy(lib_name)
   table.insert(package.links, lib_name)
end