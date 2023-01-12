
#import,file "noesis_windows.jai";

pk_dll_context : Context;

main :: () {
    pk_dll_context = context;
    Noesis : Noesis_Jai;
    Noesis.HelloWorld();
}

#scope_export

#program_export noesis_load_xaml :: (filename : *u8) #c_call {
    push_context pk_dll_context {
        print("Filename: %\n", to_string(filename));
    }
}

#import "Basic";
