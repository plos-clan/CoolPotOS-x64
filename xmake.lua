set_project("CoolPotOS")

add_rules("mode.debug", "mode.release")
add_requires("zig")

set_optimize("fastest")
set_languages("c23")
set_warnings("all", "extra", "pedantic", "error")

set_policy("run.autobuild", true)
set_policy("check.auto_ignore_flags", false)

add_cflags("-target x86_64-freestanding")
add_arflags("-target x86_64-freestanding")
add_ldflags("-target x86_64-freestanding")

add_cflags("-mno-80387", "-mno-mmx", "-mno-sse", "-mno-sse2", "-msoft-float")
add_cflags("-mno-red-zone", "-mcmodel=large", "-fno-sanitize=undefined")

target("kernel")
    set_kind("binary")
    set_toolchains("@zig")
    set_default(false)

    add_linkdirs("libs")
    add_includedirs("libs")
    add_includedirs("src/include")
    add_ldflags("-T src/linker.ld", "-e kmain")

    add_links("alloc")
    add_links("os_terminal")
    add_files("src/**.c")
    add_files("src/**/*c")

target("iso")
    set_kind("phony")
    add_deps("kernel")
    set_default(true)

    on_build(function (target)
        import("core.project.project")

        local iso_dir = "$(buildir)/iso_dir"
        os.cp("assets/limine/*", iso_dir .. "/limine/")

        local target = project.target("kernel")
        os.cp(target:targetfile(), iso_dir .. "/kernel.elf")

        local iso_file = "$(buildir)/CoolPotOS.iso"
        os.run("xorriso -as mkisofs --efi-boot limine/limine-uefi-cd.bin %s -o %s", iso_dir, iso_file)
        print("ISO image created at: %s", iso_file)
    end)

    on_run(function (target)
        import("core.project.config")

        local flags = {
            "-M", "q35",
            "-m", "256m",
            "-cpu", "qemu64,+x2apic",
            "-smp", "4",
            "-serial", "stdio",
            "-no-reboot",
            --"-d", "in_asm,int",
            "-drive", "if=pflash,format=raw,file=assets/ovmf-code.fd",
            "-cdrom", config.buildir() .. "/CoolPotOS.iso"
        }
        
        os.execv("qemu-system-x86_64" , flags)
    end)
