--PremakeRoot = path.getabsolute("./")
Root = path.getabsolute("..")
ThirdRoot = path.getabsolute("../3rd")

-- You should not modify this script below this line.

newoption {
	trigger = "llvm-root",
	value = "path",
	description = "path of llvm root (contains lib include bin/llvm-config)"
}

newoption {
	trigger = "ctemplate-root",
	value = "path",
	description = "path of ctemplate root (contains Release and src/windows)"
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
CTemplateRoot = _OPTIONS["ctemplate-root"] --path.join(ThirdRoot, "ctemplate-ctemplate-2.4");

if (LLVMRoot == nil or LLVMRoot == "") then
	-- assume llvm is installed in system
	LLVMConfig = "llvm-config"
else
	LLVMRoot = path.getabsolute(LLVMRoot)
	print("llvm-root: ", LLVMRoot)
	LLVMIncludeDir = path.join(LLVMRoot, "include")
	LLVMBinDir = path.join(LLVMRoot, "bin")
	LLVMLibDir = path.join(LLVMRoot, "lib")
	LLVMConfig = path.join(LLVMBinDir, "llvm-config")
end

if (CTemplateRoot ~= nil and CTemplateRoot ~= "") then
	CTemplateRoot = path.getabsolute(CTemplateRoot)
	print("ctemplate-root: ", CTemplateRoot)
end

function UseCTemplate()
	if (CTemplateRoot ~= nil and CTemplateRoot ~= "") then
		externalincludedirs { path.join(CTemplateRoot, "src/windows")}
		libdirs { path.join(CTemplateRoot, "Release") }
	end
	filter {"toolset:not msc*"}
		links { "ctemplate_nothreads" }
	filter {"toolset:msc*"}
		links { "libctemplate" }

	filter{}
end

function Llvm_config_cpp_flags()
	if (LLVMIncludeDir ~= nil and LLVMIncludeDir ~= "") then externalincludedirs(LLVMIncludeDir) end
	filter {"toolset:not msc*"}
		buildoptions { "$(shell " .. LLVMConfig .. " --cppflags" .. ")" }
	filter {}
end

function LinkToClang()
	filter "configurations:*WithDLL"
		if (LLVMBinDir ~= nil and LLVMBinDir ~= "") then libdirs(LLVMBinDir) end
		if (LLVMLibDir ~= nil and LLVMLibDir ~= "") then libdirs(LLVMLibDir) end

	filter "configurations:not *WithDLL"
		if (LLVMLibDir ~= nil and LLVMLibDir ~= "") then libdirs(LLVMLibDir) end

	filter {"toolset:msc*"}
		links {"libclang"}

	filter {"configurations:*WithDLL", "toolset:not msc*"}
		--links {"clang"} -- order issue between link and linkoptions
		linkoptions {"-lclang"}
		if not (_OPTIONS["without-tinfo"]) then
			links { "tinfo" }
		end

	filter {"configurations:*WithDLL", "toolset:not msc*"}
		linkoptions { "$(shell " .. LLVMConfig .. " --system-libs --ldflags --libs support" .. ")" }
		linkoptions { "-pthread" }

	filter "configurations:not *WithDLL"
		links { "clang", "clangIndex", "clangFormat", "clangTooling",
				"clangToolingCore", "clangFrontend", "clangDriver",
				"clangSerialization", "clangParse", "clangSema",
				"clangAnalysis", "clangRewrite", "clangEdit",
				"clangAST", "clangLex", "clangBasic"}
		if not (_OPTIONS["without-tinfo"]) then
			links { "tinfo" }
		end

	filter {"configurations:not *WithDLL", "toolset:not msc*"}
		linkoptions { "$(shell " .. LLVMConfig .. " --system-libs --ldflags --libs all support" .. ")" }
		linkoptions { "-pthread" }

	filter {}
end

solution "ccccc"
	location ( path.join(Root, "project/" .. _ACTION))
	configurations { "DebugWithDLL", "ReleaseWithDLL", "Debug", "Release" }

	objdir(path.join(Root, "obj", _ACTION)) -- premake add $(configName)/$(AppName)
	targetdir(path.join(Root, "bin", _ACTION, "%{cfg.buildcfg}"))

	cppdialect "C++17"

	externalwarnings "Off"

	filter "action:codelite"
		toolset "gcc"
	filter "action:codeblocks"
		toolset "gcc"

	filter "toolset:clang"
		defines {"__STDC_LIMIT_MACROS", "__STDC_CONSTANT_MACROS"}

	filter "configurations:Debug*"
		optimize "On"
		symbols "Off"
		defines "NDEBUG"
	filter "configurations:Release*"
		optimize "On"
		symbols "Off"
		defines "NDEBUG"
	filter {}

	startproject "ccccc_app"

-- --------------------------------------
	project "ccccc_app"
		kind "ConsoleApp"
		language "C++"
		targetname("ccccc")
		files { path.join(Root, "src/app/**.*") }
		warnings "Extra"
		flags { "FatalWarnings"}

		includedirs { path.join(Root, "src/lib/") }

		Llvm_config_cpp_flags()

		UseCTemplate()

		links { "ccccc_lib" }
		filter { "system:windows" }
			linkoptions{path.join("%{cfg.targetdir}", "ccccc.lib")} -- order issue between link and linkoptions
		filter { "system:not windows" }
			linkoptions{path.join("%{cfg.targetdir}", "libccccc.a")} -- order issue between link and linkoptions
		filter {}
		LinkToClang()

		--debugdir(Root)
		debugargs {"../../../samples/namespace.cpp"}

-- --------------------------------------
	project "ccccc_lib"
		kind "StaticLib"
		language "C++"
		targetname("ccccc_lib")
		files { path.join(Root, "src/lib/**.*") }
		warnings "Extra"
		flags { "FatalWarnings"}

		Llvm_config_cpp_flags()

-- --------------------------------------
	project "ccccc_test"
		kind "ConsoleApp"
		language "C++"
		files { path.join(Root, "test/**.*") }
		warnings "Extra"
		flags { "FatalWarnings"}

		includedirs { path.join(Root, "src/lib/") }
		externalincludedirs { path.join(ThirdRoot, "doctest/") }

		Llvm_config_cpp_flags()

		links { "ccccc_lib" }
		filter { "system:windows" }
			linkoptions{path.join("%{cfg.targetdir}", "ccccc_lib.lib")} -- order issue between link and linkoptions
		filter { "system:not windows" }
			linkoptions{path.join("%{cfg.targetdir}", "libccccc_lib.a")} -- order issue between link and linkoptions
		filter {}

		LinkToClang()

		--debugdir(Root)

-- --------------------------------------
	project "sample"
		kind "None"
		language "C++"
		files { path.join(Root, "samples/**.*") }
		warnings "Extra"
		flags { "FatalWarnings"}
