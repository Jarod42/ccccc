--PremakeRoot = path.getabsolute("./")
Root = path.getabsolute("..")
ThirdRoot = path.getabsolute("../3rd")

-- Some path to customize with your config.
LLVMRoot = "C:/Perso/msys/mingw/"
UnitTestPPRoot = "C:/UnitTest++-1.3"
CTemplateRoot = path.join(ThirdRoot, "ctemplate-2.2");

-- You should not modify this script below this line.

if (_ACTION == nil) then
	return
end

ActionsData = {
	["codelite"] = {["Dir"] = "CL", ["Compiler"] = "g++"},
	["codeblocks"] = {["Dir"] = "codeblocks", ["Compiler"] = "g++"},
	["vs2005"] = {["Dir"] = "vs2005", ["Compiler"] = "vc"},
	["vs2008"] = {["Dir"] = "vs2008", ["Compiler"] = "vc"},
	["vs2010"] = {["Dir"] = "vs2010", ["Compiler"] = "vc"},
	["gmake"] = {["Dir"] = "gmake", ["Compiler"] = "g++"}
}

CompilerData = {
	["g++"] = { ["buildoptions"] = {"-Wextra", "-Wno-unused-parameter", "-std=c++14"}},
	["clang++"] = {
		["buildoptions"] = {"-Wextra", "-Wno-unused-parameter", "-std=c++14"},
		["defines"] = {"__STDC_LIMIT_MACROS", "__STDC_CONSTANT_MACROS"},
		["includedirs"] = {
			"C:/Perso/TDM-GCC-64/include",
			"C:/Perso/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/5.1.0/include/c++",
			"C:/Perso/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/5.1.0/include/c++/x86_64-w64-mingw32",
			"C:/Perso/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/5.1.0/include/c++/backward",
			"C:/Perso/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/5.1.0/include",
			"C:/Perso/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/5.1.0/include-fixed"
		}
	},
	["vc"] = {}
}

if (ActionsData[_ACTION] == nil) then
	error (_ACTION .. " unsupported")
end

ConfigurationsData = {
	["Debug"] = {["Dir"] = "Debug", ["Flags"] = {"Symbols"}, ["Defines"] = {"DEBUG"}},
	["Release"] = {["Dir"] = "Release", ["Flags"] = {"Optimize"}, ["Defines"] = {"NDEBUG"}},
	["DebugWithDLL"] = {["Dir"] = "DebugWithDLL", ["Flags"] = {"Symbols"}, ["Defines"] = {"DEBUG"}},
	["ReleaseWithDLL"] = {["Dir"] = "ReleaseWithDLL", ["Flags"] = {"Optimize"}, ["Defines"] = {"NDEBUG"}}
}

LocationDir = path.join(Root, "project/" .. ActionsData[_ACTION].Dir)


LLVMIncludeDir = path.join(LLVMRoot, "include")
LLVMBinDir = path.join(LLVMRoot, "bin")
LLVMLibDir = path.join(LLVMRoot, "lib")

UnitTestPPIncludeDir = path.join(UnitTestPPRoot, "src")
UnitTestPPLibDir = path.join(UnitTestPPRoot, "Release")

function DefaultConfiguration()
	compilerData = CompilerData[ActionsData[_ACTION].Compiler]
	for config,data in pairs(ConfigurationsData) do
		configuration(config)
			objdir(path.join(Root, "obj/" .. ActionsData[_ACTION].Dir)) -- premake add $(configName)/$(AppName)
			targetdir(path.join(Root, "bin/" .. ActionsData[_ACTION].Dir .. "/" .. data.Dir))
			flags (data.Flags)
			defines(data.Defines)
			if (compilerData ~= nil) then
				if compilerData.buildoptions ~= nil then buildoptions(compilerData.buildoptions) end
				if compilerData.defines ~= nil then defines(compilerData.defines) end
				if compilerData.includedirs ~= nil then includedirs(compilerData.includedirs) end
			end
	end
end

function UseCTemplate()
	includedirs { path.join(CTemplateRoot, "src")}
	libdirs { path.join(CTemplateRoot, ".libs") }
	links { "ctemplate_nothreads" }
end

function LinkToClang()
	configuration "*WithDLL"
	if (_ACTION == "codelite") then
		links { "libclang" }
		linkoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --system-libs --ldflags --libs support" .. ")" }
	else
		links { "libclang" }
	end
	configuration "not *WithDLL"
	if (_ACTION == "codelite") then
		--links { "libclang" }
	else
		links { "clangIndex" }
		
	end
		links { "clangFrontend", "clangDriver", "clangSerialization", "clangParse",
						"clangSema", "clangAnalysis", "clangRewrite", "clangEdit",
						"clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }

		links { "psapi", "imagehlp" }
		linkoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --system-libs --ldflags --libs support" .. ")" }
end


solution "ccccc"
	location ( LocationDir )
	configurations { "Debug", "Release", "DebugWithDLL", "ReleaseWithDLL" }

	includedirs(LLVMIncludeDir)

	configuration "*WithDLL"
		libdirs(LLVMBinDir)
	configuration "not *WithDLL"
		libdirs(LLVMLibDir)

-- --------------------------------------
	project "ccccc_app"
		kind "ConsoleApp"
		language "C++"
		targetname("ccccc")
		files { path.join(Root, "src/app/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		includedirs { path.join(Root, "src/lib/") }

		UseCTemplate()

		buildoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --cppflags" .. ")" }

		links { "ccccc_lib"}
		LinkToClang()

		configuration "*"
		if (_PREMAKE_VERSION >= "4.4") then
			--debugdir(Root)
			debugargs {"../../../samples/namespace.cpp"}
		end

		DefaultConfiguration()

-- --------------------------------------
	project "ccccc_lib"
		kind "StaticLib"
		language "C++"
		targetname("ccccc")
		files { path.join(Root, "src/lib/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		buildoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --cppflags" .. ")" }

		DefaultConfiguration()

-- --------------------------------------
	project "ccccc_test"
		kind "ConsoleApp"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "test/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		includedirs { path.join(Root, "src/lib/") }
		includedirs {UnitTestPPIncludeDir}
		libdirs {UnitTestPPLibDir}

		buildoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --cppflags" .. ")" }

		links { "ccccc_lib" }
		links { "unittest++" }
		LinkToClang()

		configuration "*"
		if (_PREMAKE_VERSION >= "4.4") then
			--debugdir(Root)
		end

		DefaultConfiguration()

-- --------------------------------------
	if (_ACTION ~= "gmake") then -- project to see examples
	project "sample"
		kind "StaticLib"
		language "C++"
		--flags "WinMain"
		files { path.join(Root, "samples/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}
		--buildoptions "-std=c++11"

		DefaultConfiguration()
	end
