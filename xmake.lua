set_project("cpp-result")
set_xmakever("2.5.4")

set_version("1.0.0", {build = "%Y%m%d%H%M"})

set_warnings("all", "error")

set_languages("c++17")
add_rules("mode.release", "mode.debug", "mode.profile")

target("cpp-result")
add_headerfiles("include/result/(*.h)")
add_includedirs("include", {public = true})

includes("tests")
