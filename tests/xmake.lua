add_requires("doctest")
set_default(false)

target("tests")
set_kind("binary")
add_deps("cpp-result")
add_files("*.cpp")
add_packages("doctest")
after_build(function(target)
    local resdir = path.join(target:targetdir(), "data")
    os.cp("$(projectdir)/tests/data/", resdir)
end)

target_end()
