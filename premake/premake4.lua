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
						["Release"] = {["Dir"] = "Release", ["Flags"] = {"Optimize"}, ["Defines"] = {"NDEBUG"}},
						["DebugWithDLL"] = {["Dir"] = "DebugWithDLL", ["Flags"] = {"Symbols"}, ["Defines"] = {"DEBUG"}},
						["ReleaseWithDLL"] = {["Dir"] = "ReleaseWithDLL", ["Flags"] = {"Optimize"}, ["Defines"] = {"NDEBUG"}}
					}

LocationDir = path.join(Root, "project/" .. ActionsData[_ACTION].Dir)

--LibDir3rd = path.join(ThirdRoot, "lib")
--IncludeDir3rd = path.join(ThirdRoot, "include")

ClangRoot = path.join(ThirdRoot, "clang_llvm-3.1-i386-mingw32-EXPERIMENTAL")
ClangBinDir = path.join(ClangRoot, "bin")
ClangIncludeDir = path.join(ClangRoot, "include")
ClangLibDir = path.join(ClangRoot, "lib")

UnitTestPPRoot = "d:/UnitTest++-1.3"
UnitTestPPIncludeDir = path.join(UnitTestPPRoot, "src")
UnitTestPPLibDir = path.join(UnitTestPPRoot, "Release")

CTemplateRoot = path.join(ThirdRoot, "ctemplate-2.2");


GenGetOptExe = "D:\\Programs\\Msys\\msys\\1.0\\local\\bin\\gengetopt.exe"


function DefaultConfiguration()
	for config,data in pairs(ConfigurationsData) do
		configuration(config)
			objdir(path.join(Root, "obj/" .. ActionsData[_ACTION].Dir)) -- premake add $(configName)/$(AppName)
			targetdir(path.join(Root, "bin/" .. ActionsData[_ACTION].Dir .. "/" .. data.Dir))
			flags (data.Flags)
			defines(data.Defines)
	end
end

function UseCTemplate()
	includedirs { path.join(CTemplateRoot, "src")}
	libdirs { path.join(CTemplateRoot, ".libs") }
	links { "ctemplate_nothreads" }
end

solution "ccccc"
	location ( LocationDir )
	configurations { "Debug", "Release", "DebugWithDLL", "ReleaseWithDLL" }

	includedirs(ClangIncludeDir)
	
	configuration "*WithDLL"
		libdirs(ClangBinDir)
	configuration "not *WithDLL"
		libdirs(ClangLibDir)

	project "files_generator"
		kind "ConsoleApp"
		language "C"

		files { path.join(Root, "src/files_generator/**.*") }
		postbuildcommands { GenGetOptExe .. " -i " .. path.join(Root, "src/files_generator/parameters.ggo") }
		DefaultConfiguration()

	project "ccccc_lib"
		kind "StaticLib"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "src/lib/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		buildoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --cppflags" .. ")" }

		--links { "clang", "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewrite", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }
		--links { "psapi", "imagehlp" }
		--linkoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --libs" .. ")" }

		DefaultConfiguration()

	project "ccccc_app"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "src/app/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		includedirs { path.join(Root, "src/lib/") }

		UseCTemplate()

		buildoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --cppflags" .. ")" }

		links { "ccccc_lib"}
		configuration "*WithDLL"
			links { "clang" }
		configuration "not *WithDLL"
			links { "clang", "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewrite", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }
			links { "psapi", "imagehlp" }
			linkoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --libs" .. ")" }

		configuration "*"
		if (_PREMAKE_VERSION >= "4.4") then
			--debugdir(Root)
			debugargs {"../../../samples/test.c"}
		end

		DefaultConfiguration()
	project "ccccc_test"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "test/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		includedirs { path.join(Root, "src/lib/") }
		includedirs {UnitTestPPIncludeDir}
		libdirs {UnitTestPPLibDir}

		buildoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --cppflags" .. ")" }
		
		links { "ccccc_lib" }
		links { "unittest++" }
		configuration "*WithDLL"
			links { "clang" }
		configuration "not *WithDLL"
			links { "clang", "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewrite", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }
			links { "psapi", "imagehlp" }

			linkoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --libs" .. ")" }

		configuration "*"
		if (_PREMAKE_VERSION >= "4.4") then
			--debugdir(Root)
		end

		DefaultConfiguration()

	project "sample"
		kind "StaticLib"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "samples/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		DefaultConfiguration()

	project "3rd"
		kind "StaticLib"
		language "C++"
		files { path.join(ClangIncludeDir, "**.h") }

		DefaultConfiguration()
