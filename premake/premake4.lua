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

ClangRoot = path.join(ThirdRoot, "clang_llvm-3.1-i386-mingw32-EXPERIMENTAL")
ClangBinDir = path.join(ClangRoot, "bin")
ClangIncludeDir = path.join(ClangRoot, "include")
ClangLibDir = path.join(ClangRoot, "lib")

UnitTestPPRoot = "d:/UnitTest++-1.3"
UnitTestPPIncludeDir = path.join(UnitTestPPRoot, "src")
UnitTestPPLibDir = path.join(UnitTestPPRoot, "Release")


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
		kind "StaticLib"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "src/**.*") }
		excludes { path.join(Root, "src/main.cpp") }
		flags { "ExtraWarnings", "FatalWarnings"}

		buildoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --cxxflags" .. ")" }
		--links { "clang", "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewrite", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }
		--links { "psapi", "imagehlp" }
		--linkoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --libs" .. ")" }

		DefaultConfiguration()

	project "cccc_clang_app"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "src/main.cpp") }
		flags { "ExtraWarnings", "FatalWarnings"}

		buildoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --cxxflags" .. ")" }
		links { "cccc_clang"}
		links { "clang", "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewrite", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }
		links { "psapi", "imagehlp" }
		linkoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --libs" .. ")" }

		if (_PREMAKE_VERSION >= "4.4") then
			--debugdir(Root)
			debugargs {"../../../samples/test.c"}
		end

		DefaultConfiguration()
	project "test"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "test/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		includedirs { path.join(Root, "src") }
		includedirs {UnitTestPPIncludeDir}
		libdirs {UnitTestPPLibDir}

		buildoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --cxxflags" .. ")" }
		buildoptions { "-fexceptions" } -- llvm seem to disable exceptions, but UnitTest++ uses them


		links { "cccc_clang" }
		links { "clang", "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewrite", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }
		links { "psapi", "imagehlp" }
		links { "unittest++" }

		linkoptions { "$(shell " .. path.join(ClangBinDir, "llvm-config") .. " --libs" .. ")" }

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
