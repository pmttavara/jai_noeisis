#import "Basic";
#import "Compiler";
#import "BuildCpp";
#import "Bindings_Generator";
#import "Check";

PATH_SDK :: "NoesisGUI-NativeSDK-win-3.1.6-Indie";

#run {
    set_build_options_dc(.{do_output = false});
    
    success := false;
    
    w := compiler_create_workspace("Main");
    success = build_cpp_static_lib(
        "noesis_jai",
        "noesis_jai.cpp",
        extra = .[
            #run tprint("-I%/Include", PATH_SDK),
            #run tprint("-I%/Src/Packages/App/Providers/Include", PATH_SDK),
            #run tprint("-I%/Src/Packages/App/ApplicationLauncher/Include", PATH_SDK),
            #run tprint("-I%/Src/Packages/App/DisplayLauncher/Include", PATH_SDK),
            #run tprint("-I%/Src/Packages/App/Launcher/Include", PATH_SDK),
            #run tprint("-I%/Src/Packages/App/Display/Include", PATH_SDK),
            #run tprint("-I%/Src/Packages/App/Theme/Include", PATH_SDK),
        ],
        debug = true,
    );
    if !success {
        compiler_set_workspace_status(.FAILED);
        return;
    }
    opts: Generate_Bindings_Options;
    array_add(*opts.libpaths, ".");
    // array_add(*opts.libpaths, #run tprint("./%/Lib/windows_x86_64/", PATH_SDK));
    array_add(*opts.libnames, "noesis_jai");
    array_add(*opts.libnames, "Noesis");
    array_add(*opts.system_include_paths, GENERATOR_DEFAULT_SYSTEM_INCLUDE_PATH);
    array_add(*opts.source_files, "noesis_jai.hpp");
    array_add(*opts.extra_clang_arguments, "-x", "c++");
    success = generate_bindings(opts, "noesis_windows.jai");
    if !success {
        compiler_set_workspace_status(.FAILED);
        return;
    }
    
    options := get_build_options(w);
    options.output_type = .EXECUTABLE;
    options.output_executable_name = "main";
    set_build_options(options, w);

    compiler_begin_intercept(w);
    add_build_file("main.jai", w);

    while true {
        message := compiler_wait_for_message();
        if message.kind == {
          case .TYPECHECKED;
            do_error_checking(null, message);
            
          case .COMPLETE;
            break;
        }
    }

    compiler_end_intercept(w);
}
