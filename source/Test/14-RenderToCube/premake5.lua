project "Test-14-RenderToCube"

currentSourceDir = path.join( sourceDir, "Test", "14-RenderToCube" )
currentBinaryDir = path.join( binaryDir, "Test", "14-RenderToCube" )

kind( "WindowedApp" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", sharedLibDir ) )
implibdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )
defines( "VkRenderer_EXPORTS" )
wx_config {Unicode="yes", Version="3.1", Libs="core,aui", Static="yes"}

includedirs{
	path.join( sourceDir, "Core" ),
	path.join( sourceDir, "Renderer", "Renderer", "Src" ),
	path.join( binaryDir, "Renderer", "Renderer", "Src" ),
	path.join( sourceDir, "Test", "00-Common", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Utils",
	"Renderer",
	"00-Common"
}

files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp"
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
