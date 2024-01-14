#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
 


#include <shengine/shEngine.h>//most of engine declarations are here
#include <shengine/shExport.h>//required when building a shared library



//#include "crystallography/crystallography.h"



uint8_t SH_ENGINE_EXPORT_FUNCTION model_start(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_start: invalid engine memory", return 0);

    shEngineGuiSetup(p_engine);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_update(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_update: invalid engine memory", return 0);

    ShGui* p_gui = &p_engine->gui;

    shGuiText(
        p_gui,
        SH_GUI_VEC2_ZERO,
        SH_GUI_COLOR_WHITE,
        20.0f,
        "@mrsinho",
        SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
    );

    //compile shaders

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_main_cmd_buffer(
    ShEngine* p_engine
) {
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_main_renderpass(
    ShEngine* p_engine
) {
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_frame_resize(
    ShEngine* p_engine
) {
    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_close(
    ShEngine* p_engine
) {
    return 1;
}



#ifdef SH_APPLICATION_TARGET_TYPE_EXECUTABLE
 
#include <sheditor/shEditor.h>
 
int main() {
    ShEngine* p_engine = shAllocateEngine();
    shEngineError(
        p_engine == NULL,
        "main: invalid engine memory",
        return -1
    );
 
    p_engine->application_host.p_start           = (ShApplicationFunc*) &model_start;
    p_engine->application_host.p_update          = (ShApplicationFunc*) &model_update;
    p_engine->application_host.p_main_cmd_buffer = (ShApplicationFunc*) &model_main_cmd_buffer;
    p_engine->application_host.p_main_renderpass = (ShApplicationFunc*) &model_main_renderpass;
    p_engine->application_host.p_frame_resize    = (ShApplicationFunc*) &model_frame_resize;
    p_engine->application_host.p_close           = (ShApplicationFunc*) &model_close;
    p_engine->window.title                       = "crystal model";
    
    shEditorMain(p_engine);
    return 0;
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE
 


#ifdef __cplusplus
}
#endif//__cplusplus