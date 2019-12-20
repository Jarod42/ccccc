--PremakeRoot = path.getabsolute("./")
Root = path.getabsolute("..")
ThirdRoot = path.getabsolute("../3rd")

-- Some path to customize with your config.
UnitTestPPRoot = "C:/UnitTest++-1.3"
CTemplateRoot = path.join(ThirdRoot, "ctemplate-2.4");

-- You should not modify this script below this line.

newoption {
   trigger = "llvm-root",
   value = "path",
   description = "path of llvm root (contains lib include bin/llvm-config)"
}

if (_ACTION == nil) then
	return
end

LLVMRoot = _OPTIONS["llvm-root"]

if (LLVMRoot == nil or LLVMRoot == "") then
	print("No llvm-root provided")
	print("Generation aborted")
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
	["g++"] = { ["buildoptions"] = {"-Wextra", "-Wno-unused-parameter", "-std=c++17"}},
	["clang++"] = {
		["buildoptions"] = {"-Wextra", "-Wno-unused-parameter", "-std=c++17"},
		["defines"] = {"__STDC_LIMIT_MACROS", "__STDC_CONSTANT_MACROS"}
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
UnitTestPPLibDir = "/usr/local/lib"

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

function Llvm_config_cpp_flags()
	buildoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --cppflags" .. ")" }
end

function LinkToClang()
	configuration "*WithDLL"
		links { "clang", "LLVMSupport", "tinfo" }
		linkoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --system-libs --ldflags --libs support" .. ")" }
		linkoptions { "-pthread" }

	configuration "not *WithDLL"
		links { "clang", "clangIndex", "clangFormat", "clangTooling",
				"clangToolingCore", "clangFrontend", "clangDriver",
				"clangSerialization", "clangParse", "clangSema",
				"clangAnalysis", "clangRewrite", "clangEdit",
				"clangAST", "clangLex", "clangBasic", "LLVMSupport", "tinfo"}

		linkoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --system-libs --ldflags --libs all support" .. ")" }
		linkoptions { "-pthread" }

end


solution "ccccc"
	location ( LocationDir )
	configurations { "DebugWithDLL", "ReleaseWithDLL", "Debug", "Release" }

	includedirs(LLVMIncludeDir)

	configuration "*WithDLL"
		libdirs(LLVMBinDir)
		libdirs(LLVMLibDir)
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

		Llvm_config_cpp_flags()

		UseCTemplate()

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

		Llvm_config_cpp_flags()
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

		Llvm_config_cpp_flags()

		links { "ccccc_lib" }
		links { "UnitTest++" }
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
		--buildoptions "-std=c++14"

		DefaultConfiguration()
	end
