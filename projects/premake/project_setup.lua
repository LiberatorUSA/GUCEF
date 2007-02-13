to_compiler = { ["vs2005"] = "MVC8", ["vs2003"] = "MVC7.1", ["vs2002"] = "MVC7", ["vs6"] = "MVC6" }

if(to_compiler[target]) then
    compiler = to_compiler[target]
else
    compiler = target
end

global_defines = { "_UNICODE", "UNICODE" }

if(windows) then
    additional_defines = { "_WINDLL", "WIN32", "_DEBUG", "_WINDOWS", "_USRDLL" }
end

for k,v in pairs(additional_defines) do table.insert(global_defines, v) end

project.name = project_name.."_"..compiler
project.path = "../"..compiler
