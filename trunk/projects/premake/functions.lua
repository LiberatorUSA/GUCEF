
--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Main build script setup function                                       ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function project_setup(project_name)
    addoption("compiler", "Specify compiler type (eg. MVC8, MVC7.1, GCC)")
    
    to_compiler = { ["vs2005"] = "MVC8", ["vs2003"] = "MVC7.1", ["vs2002"] = "MVC7", ["vs6"] = "MVC6" }

    if(options["compiler"]) then
        compiler = options["compiler"]
    elseif(to_compiler[target]) then
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
    
    common = { ["Bin/Debug"]   = "common\\bin\\Debug_"..compiler,
               ["Bin/Release"] = "common\\bin\\Release_"..compiler }
    os.execute("mkdir \"..\\..\\"..common["Bin/Debug"].."\"")
    os.execute("mkdir \"..\\..\\"..common["Bin/Release"].."\"")
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Adds a module to the build script                                      ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_package( package_name )
    package = newpackage()

    package.name = package_name.."_"..compiler
    package.path = "../../"..package_name.."/projects/"..compiler
    package.language = "c"
    package.kind = "dll"
    package.files = {
        matchrecursive("../../include/*.h", "../../include/mswin/*.h", "../../src/*.c", "../../src/*.cpp", "../../mswin/src/*.cpp", "../../dependancy/*.c", "../../dependancy/*.cpp", "../../dependancy/*.h")
    }

    package.includepaths = { "../../include", "../../include/mswin", "../../dependancy", "../../../common/include" }

    package.config["Debug"].bindir = "../../../"..package_name.."/bin/Debug_"..compiler
    package.config["Debug"].libdir = "../../../"..package_name.."/bin/Debug_"..compiler
    package.config["Debug"].objdir = "../../../"..package_name.."/bin/Debug_"..compiler

    package.config["Debug"].defines = { "DEBUG", "_DEBUG" }
    package.config["Debug"].target = package_name.."_"..compiler.."_d"
    
    package.config["Debug"].postbuildcommands = { "copy &quot;..\\..\\..\\"..package_name.."\\bin\\Debug_"..compiler.."\\"..package.config["Debug"].target..".dll&quot; &quot;..\\..\\..\\"..common["Bin/Debug"].."&quot;" }
    
    package.config["Release"].bindir = "../../../"..package_name.."/bin/Release_"..compiler
    package.config["Release"].libdir = "../../../"..package_name.."/bin/Release_"..compiler
    package.config["Release"].objdir = "../../../"..package_name.."/bin/Release_"..compiler

    package.config["Release"].target = package_name.."_"..compiler

    package.config["Release"].postbuildcommands = { "copy &quot;..\\..\\..\\"..package_name.."\\bin\\Release_"..compiler.."\\"..package.config["Release"].target..".dll&quot; &quot;..\\..\\..\\"..common["Bin/Debug"].."&quot;" }

    for k,v in pairs(global_defines) do table.insert(package.defines, v) end

    table.insert(package.defines, string.upper(package_name).."_BUILD_MODULE")

    if(compiler == "MVC8") then
        table.insert(package.defines, string.upper(package_name).."_VC8_EXPORTS")
    end
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Adds an application to the build script                                ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_application( package_name )
    package = newpackage()

    package.name = package_name.."_"..compiler
    package.path = "../../"..package_name.."/projects/"..compiler
    package.language = "c"
    package.kind = "winexe"
    package.files = {
        matchrecursive("../../include/*.h", "../../include/mswin/*.h", "../../src/*.c", "../../src/*.cpp", "../../mswin/src/*.cpp", "../../dependancy/*.c", "../../dependancy/*.cpp", "../../dependancy/*.h")
    }

    package.includepaths = { "../../include", "../../include/mswin", "../../dependancy", "../../../common/include" }

    package.config["Debug"].bindir = "../../../"..package_name.."/bin/Debug_"..compiler
    package.config["Debug"].libdir = "../../../"..package_name.."/bin/Debug_"..compiler
    package.config["Debug"].objdir = "../../../"..package_name.."/bin/Debug_"..compiler

    package.config["Debug"].defines = { "DEBUG", "_DEBUG" }
    package.config["Debug"].target = package_name.."_"..compiler.."_d"
    
    package.config["Debug"].postbuildcommands = { "copy &quot;..\\..\\..\\"..package_name.."\\bin\\Debug_"..compiler.."\\"..package.config["Debug"].target..".dll&quot; &quot;..\\..\\..\\"..common["Bin/Debug"].."&quot;" }
    
    package.config["Release"].bindir = "../../../"..package_name.."/bin/Release_"..compiler
    package.config["Release"].libdir = "../../../"..package_name.."/bin/Release_"..compiler
    package.config["Release"].objdir = "../../../"..package_name.."/bin/Release_"..compiler

    package.config["Release"].target = package_name.."_"..compiler

    package.config["Release"].postbuildcommands = { "copy &quot;..\\..\\..\\"..package_name.."\\bin\\Release_"..compiler.."\\"..package.config["Release"].target..".dll&quot; &quot;..\\..\\..\\"..common["Bin/Debug"].."&quot;" }

    for k,v in pairs(global_defines) do table.insert(package.defines, v) end

    table.insert(package.defines, string.upper(package_name).."_BUILD_MODULE")

    if(compiler == "MVC8") then
        table.insert(package.defines, string.upper(package_name).."_VC8_EXPORTS")
    end
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Adds a project dependency to the project                               ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_package_dependency(package_name)
    table.insert(package.includepaths, 3, "../../../"..package_name.."/include")
    table.insert(package.links, package_name.."_"..compiler)
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Adds a library dependency to the project                               ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_library_dependency(lib_name)
    table.insert(package.links, lib_name)
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Specialization for the windows platform, no-op on other platforms      ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_windows_library_dependency( lib_name )
    if ( windows ) then
        add_library_dependency( lib_name )
    end
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Function to add packages defined in a file                             ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_packages_from_file( file_path )
    original_cwd = os.getcwd()
    os.chdir( file_path )
    dopackage( file_path )
    os.chdir( original_cwd )
end

--[[---------------------------------------------------------------------------]]
--[[                                                                           ]]
--[[    Function to add packages defined in a file passed as an option         ]]
--[[                                                                           ]]
--[[---------------------------------------------------------------------------]]

function add_packages_from_file_option( option_value )
    if ( options[ option_value ] ) then
        add_packages_from_file( options[ option_value ] )
    end
end