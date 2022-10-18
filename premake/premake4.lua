--PremakeRoot = path.getabsolute("./")
Root = path.getabsolute("..")
ThirdRoot = path.getabsolute("../3rd")

-- Some path to customize with your config.
--UnitTestPPRoot = "C:/UnitTest++-1.3"
--CTemplateRoot = path.join(ThirdRoot, "ctemplate-2.4");

-- You should not modify this script below this line.

newoption {
	trigger = "llvm-root",
	value = "path",
	description = "path of llvm root (contains lib include bin/llvm-config)"
}

-- tinfo is not part of msys
newoption {
	trigger = "without-tinfo",
	description = "don't add tinfo library dependency"
}

if (_ACTION == nil) then
	return
end

LLVMRoot = _OPTIONS["llvm-root"]

if (LLVMRoot == nil or LLVMRoot == "") then
	-- assume llvm is installed in system
	LLVMConfig = "llvm-config"
else
	LLVMIncludeDir = path.join(LLVMRoot, "include")
	LLVMBinDir = path.join(LLVMRoot, "bin")
	LLVMLibDir = path.join(LLVMRoot, "lib")
	LLVMConfig = path.join(LLVMBinDir, "llvm-config")
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
	["g++"] = { ["buildoptions"] = {"-Wextra", "-Wno-unused-parameter", "-std=c++20"}},
	["clang++"] = {
		["buildoptions"] = {"-Wextra", "-Wno-unused-parameter", "-std=c++20"},
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
	--includedirs { path.join(CTemplateRoot, "src")}
	--libdirs { path.join(CTemplateRoot, ".libs") }
	links { "ctemplate_nothreads" }
end

function Llvm_config_cpp_flags()
	buildoptions { "$(shell " .. LLVMConfig .. " --cppflags" .. ")" }
end

function LinkToClang()
	configuration "*WithDLL"
		links { "clang", "LLVMSupport"}

		if not (_OPTIONS["without-tinfo"]) then
			links { "tinfo" }
		end

		if (ActionsData[_ACTION].Compiler ~= "vc") then
			linkoptions { "$(shell " .. LLVMConfig .. " --system-libs --ldflags --libs support" .. ")" }
		end
		linkoptions { "-pthread" }

	configuration "not *WithDLL"
		links { "clang", "clangIndex", "clangFormat", "clangTooling",
				"clangToolingCore", "clangFrontend", "clangDriver",
				"clangSerialization", "clangParse", "clangSema",
				"clangAnalysis", "clangRewrite", "clangEdit",
				"clangAST", "clangLex", "clangBasic", "LLVMSupport"}
		if not (_OPTIONS["without-tinfo"]) then
			links { "tinfo" }
		end

		if (ActionsData[_ACTION].Compiler ~= "vc") then
			linkoptions { "$(shell " .. LLVMConfig .. " --system-libs --ldflags --libs all support" .. ")" }
		end
		linkoptions { "-pthread" }
	configuration "*"
end

--UnitTestPPIncludeDir = path.join(UnitTestPPRoot, "src")
--UnitTestPPLibDir = "/usr/local/lib"
function UseUnitTestPP()
	--includedirs {UnitTestPPIncludeDir}
	--libdirs {UnitTestPPLibDir}
	links { "UnitTest++" }
end


solution "ccccc"
	location ( LocationDir )
	configurations { "DebugWithDLL", "ReleaseWithDLL", "Debug", "Release" }

	if (LLVMIncludeDir ~= nil and LLVMIncludeDir ~= "") then includedirs(LLVMIncludeDir) end

	configuration "*WithDLL"
		if (LLVMBinDir ~= nil and LLVMBinDir ~= "") then libdirs(LLVMBinDir) end
		if (LLVMLibDir ~= nil and LLVMLibDir ~= "") then libdirs(LLVMLibDir) end
	configuration "not *WithDLL"
		if (LLVMLibDir ~= nil and LLVMLibDir ~= "") then libdirs(LLVMLibDir) end

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

		Llvm_config_cpp_flags()

		links { "ccccc_lib" }
		LinkToClang()
		UseUnitTestPP()

		if (_PREMAKE_VERSION >= "4.4") then
			--debugdir(Root)
		end

		DefaultConfiguration()

-- --------------------------------------
	if (_ACTION ~= "gmake") then -- project to see examples
	project "sample"
		kind "StaticLib" -- "None"
		language "C++"
		files { path.join(Root, "samples/**.*") }
		flags { "ExtraWarnings", "FatalWarnings"}

		DefaultConfiguration()
	end
