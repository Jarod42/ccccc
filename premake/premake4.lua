--PremakeRoot = path.getabsolute("./")
Root = path.getabsolute("..")
ThirdRoot = path.getabsolute("../3rd")

if (_ACTION == nil) then
	return
end

ActionsData = { ["codelite"] = {["Dir"]="CL"},
				["codeblocks"] = {["Dir"]="codeblocks"},
				["vs2005"] = {["Dir"]="vs2005"}, ["vs2008"] = {["Dir"]="vs2008"}, ["vs2010"] = {["Dir"]="vs2010"}
			}

if (ActionsData[_ACTION] == nil) then
	error (_ACTION .. " unsupported")
end

ConfigurationsData = {["Debug"] = {["Dir"] = "Debug", ["Flags"] = {"Symbols"}, ["Defines"] = {"DEBUG"}},
						["Release"] = {["Dir"] = "Release", ["Flags"] = {"Optimize"}, ["Defines"] = {"NDEBUG"}}
					}

LocationDir = path.join(Root, "project/" .. ActionsData[_ACTION].Dir)

--LibDir3rd = path.join(ThirdRoot, "lib")
--IncludeDir3rd = path.join(ThirdRoot, "include")

ClangRoot = path.join(ThirdRoot, "clang+llvm-3.1-i386-mingw32-EXPERIMENTAL")
ClangLibDir = path.join(ClangRoot, "lib")
ClangIncludeDir = path.join(ClangRoot, "include")

function DefaultConfiguration()
	for config,data in pairs(ConfigurationsData) do
		configuration(config)
			objdir(path.join(Root, "obj/" .. ActionsData[_ACTION].Dir)) -- premake add $(configName)/$(AppName)
			targetdir(path.join(Root, "bin/" .. ActionsData[_ACTION].Dir .. "/" .. data.Dir))
			flags (data.Flags)
			defines(data.Defines)
	end
end

solution "cccc_clang"
	location ( LocationDir )
	configurations { "Debug", "Release" }

	libdirs(ClangLibDir)
	includedirs(ClangIncludeDir)

	project "cccc_clang"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "src/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		links { "clang" }

		DefaultConfiguration()

	project "test"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "test/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		links { "clang" }

		DefaultConfiguration()
