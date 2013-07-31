--PremakeRoot = path.getabsolute("./")
Root = path.getabsolute("..")
ThirdRoot = path.getabsolute("../3rd")

-- Some path to customize with your config.
LLVMRoot = "D:/llvm/llvm-3.3-install/"
UnitTestPPRoot = "C:/UnitTest++-1.3"
CTemplateRoot = path.join(ThirdRoot, "ctemplate-2.2");
ExtraPath = "D:\\msys\\1.0\\local\\bin"


-- You should not modify this script below this line.

GenGetOptExe = os.pathsearch("gengetopt.exe", os.getenv("PATH"), ExtraPath)

if GenGetOptExe == nil then
	print ("executable gengetopt not found. CANCEL premake")
	GenGetOptExe = ""
	return
end
GenGetOptExe = path.join(GenGetOptExe, "gengetopt")


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
	["g++"] = { ["buildoptions"] = {"-Wextra"}},
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
				buildoptions(compilerData.buildoptions)
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
		links { "liblibclang" } -- hack for codelite which remove one prefix lib
	else
		links { "libclang" }
	end
	configuration "not *WithDLL"
	if (_ACTION == "codelite") then
		links { "liblibclang" } -- hack for codelite which remove one prefix lib
	else
		links { "libclang" }
	end
		links { "clangFrontend", "clangDriver", "clangSerialization", "clangParse", "clangSema", "clangAnalysis", "clangRewriteCore", "clangEdit", "clangAST", "clangLex", "clangBasic", "LLVMMC", "LLVMSupport" }

		links { "psapi", "imagehlp" }
		linkoptions { "$(shell " .. path.join(LLVMBinDir, "llvm-config") .. " --libs" .. ")" }
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

		links "files_generator" -- dependency

		DefaultConfiguration()

-- --------------------------------------
	project "files_generator"
		kind "ConsoleApp"
		language "C++"

		files { path.join(Root, "src/files_generator/**.*") }
		postbuildcommands { GenGetOptExe .. " -i " .. path.join(Root, "src/files_generator/parameters.ggo") }
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
