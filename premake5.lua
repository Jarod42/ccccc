Root = path.getabsolute(".")
ThirdRoot = path.getabsolute("3rd")

newoption {
	trigger = "llvm-root",
	value = "path",
	description = "path of llvm root (contains lib include bin/llvm-config)"
}

newoption {
	trigger = "expand-llvm-config",
	description = "run llvm-config from premake directly"
}

newoption {
	trigger = "no-3rd",
	description = "don't create 3rd party project"
}

-- tinfo is not part of msys
newoption {
	trigger = "without-tinfo",
	description = "don't add tinfo library dependency"
}

if (_ACTION == nil) then
	return
end

local LLVMRoot = _OPTIONS["llvm-root"]
local ExpandLLVMConfig = _OPTIONS["expand-llvm-config"]
local No3rd = _OPTIONS["no-3rd"]

if (LLVMRoot == nil or LLVMRoot == "") then
	-- assume llvm is installed in system
	LLVMConfig = "llvm-config"
else
	LLVMIncludeDir = path.join(LLVMRoot, "include")
	LLVMBinDir = path.join(LLVMRoot, "bin")
	LLVMLibDir = path.join(LLVMRoot, "lib")
	LLVMConfig = path.join(LLVMBinDir, "llvm-config")
end

function UseMstch()
	externalincludedirs { "%{ThirdRoot}/mstch/include" }
	links {"mstch"}
end

function Llvm_config_cpp_flags()
	if (LLVMIncludeDir ~= nil and LLVMIncludeDir ~= "") then externalincludedirs(LLVMIncludeDir) end
	if ExpandLLVMConfig then
		local output, e = os.outputof(LLVMConfig .. " --cppflags") -- -I$(LLVMIncludeDir) -D_FILE_OFFSET_BITS=64 -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
		buildoptions { output }
	else
		buildoptions { "$(shell %{LLVMConfig} --cppflags)" }
	end
end

function LinkToClang()
	if (LLVMLibDir ~= nil and LLVMLibDir ~= "") then libdirs(LLVMLibDir) end

	links { "clang"}

	if not (_OPTIONS["without-tinfo"]) then
		links { "tinfo" }
	end

	if ExpandLLVMConfig then
		local output, e = os.outputof(LLVMConfig .. " --system-libs --ldflags --libs support") -- -L$(LLVMLibDir) -lLLVM-18
		linkoptions { output }
	else
		linkoptions { "$(shell %{LLVMConfig} --system-libs --ldflags --libs support)" }
	end

	linkoptions { "-pthread" }
end

solution "ccccc"
	location "%{Root}/project/%{_ACTION}"
	configurations { "Debug", "Release" }

	objdir "%{Root}/obj/%{_ACTION}" -- premake add $(configName)/$(AppName)

	language "C++"
	cppdialect "C++17"

	filter "toolset:clang"
		defines {"__STDC_LIMIT_MACROS", "__STDC_CONSTANT_MACROS"}

	filter "configurations:Debug*"
		optimize "Off"
		symbols "On"
		defines "DEBUG"
	filter "configurations:Release*"
		optimize "On"
		symbols "Off"
		defines "NDEBUG"

	filter {}
	startproject "ccccc_app"
if not No3rd then
-- --------------------------------------
	group "3rd"
-- --------------------------------------
	project "mstch"
		kind "StaticLib"
		targetdir "%{Root}/lib/%{_ACTION}/%{cfg.buildcfg}"
		warnings "Extra"
		flags { "FatalWarnings"}

		files { "%{ThirdRoot}/mstch/src/**.*", "%{ThirdRoot}/mstch/include/**.*" }
		includedirs { "%{ThirdRoot}/mstch/src", "%{ThirdRoot}/mstch/include" }
end
-- --------------------------------------
	group "ccccc"
-- --------------------------------------
	project "ccccc_app"
		kind "ConsoleApp"
		targetdir "%{Root}/bin/%{_ACTION}/%{cfg.buildcfg}"
		targetname("ccccc")
		warnings "Extra"
		flags { "FatalWarnings"}

		files { "%{Root}/src/app/**.*" }
		includedirs { "%{Root}/src/lib/" }

		Llvm_config_cpp_flags()

		UseMstch()

		links { "ccccc_lib" }
		filter { "system:windows" }
			linkoptions { "%{Root}/lib/%{_ACTION}/%{cfg.buildcfg}/ccccc_lib.lib" } -- order issue between link and linkoptions
		filter { "system:not windows" }
			linkoptions { "%{Root}/lib/%{_ACTION}/%{cfg.buildcfg}/libccccc_lib.a" } -- order issue between link and linkoptions
		filter {}
		LinkToClang()

		--debugdir(Root)
		debugargs {"../../../samples/namespace.cpp"}

-- --------------------------------------
	project "ccccc_lib"
		kind "StaticLib"
		targetdir "%{Root}/lib/%{_ACTION}/%{cfg.buildcfg}"
		targetname("ccccc_lib")
		warnings "Extra"
		flags { "FatalWarnings"}
		files { "%{Root}/src/lib/**.*" }

		Llvm_config_cpp_flags()

-- --------------------------------------
	project "ccccc_test"
		kind "ConsoleApp"
		targetdir "%{Root}/bin/%{_ACTION}/%{cfg.buildcfg}"
		warnings "Extra"
		flags { "FatalWarnings"}

		files { "%{Root}/test/**.*" }
		includedirs { "%{Root}/src/lib/" }
		externalincludedirs { "%{ThirdRoot}/doctest/doctest/" }

		Llvm_config_cpp_flags()

		links { "ccccc_lib" }
		filter { "system:windows" }
			linkoptions { "%{Root}/lib/%{_ACTION}/%{cfg.buildcfg}/ccccc_lib.lib" } -- order issue between link and linkoptions
		filter { "system:not windows" }
			linkoptions { "%{Root}/lib/%{_ACTION}/%{cfg.buildcfg}/libccccc_lib.a" } -- order issue between link and linkoptions
		filter {}

		LinkToClang()

		debugdir("%{cfg.targetdir}")

-- --------------------------------------
	project "sample"
		kind "None"
		warnings "Extra"
		flags { "FatalWarnings" }

		files { "%{Root}/samples/**.*" }
