#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
 


#include <shengine/shEngine.h>//most of engine declarations are here
#include <shengine/shExport.h>//required when building a shared library

#include "crystal-model/crystal-model.h"

#include <memory.h>



uint8_t modelLoadRecovery(
    Model* p_model
) {
    shApplicationError(p_model == NULL, "modelLoadRecovery: invalid engine extension memory", return 0);

    smdReadFile("../../smd/model-recovery.smd", &p_model->recovery_smd);
    smdParseMemory(&p_model->recovery_smd);
    smdAccessVarByName(&p_model->recovery_smd, "incident_ray",       NULL, &p_model->film_conditions.incident_ray);
    smdAccessVarByName(&p_model->recovery_smd, "lambda_ag",          NULL, &p_model->film_conditions.lambda_ag);
    smdAccessVarByName(&p_model->recovery_smd, "film_distance_cm",   NULL, &p_model->film_conditions.film_distance_cm);
    smdAccessVarByName(&p_model->recovery_smd, "atom_count",         NULL, &p_model->film_conditions.atom_count);
    smdAccessVarByName(&p_model->recovery_smd, "atoms_positions_ag", NULL,  p_model->lattice_unit_data.atoms_positions_ag);
    smdFileHandleRelease(&p_model->recovery_smd);

    return 1;
}

extern uint8_t modelUpdateInputs(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "modelUpdateInputs: invalid engine memory", return 0);

    Model*   p_model = (Model*)p_engine->p_ext;
    ShWindow window  = p_engine->window;

    if (shIsKeyDown(window, SH_KEY_Q)) {
        p_model->film_conditions.lambda_ag -= 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_W)) {
        p_model->film_conditions.lambda_ag += 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_A)) {
        p_model->film_conditions.incident_ray.x -= 1.0f * (float)p_engine->time.delta_time;
    }
    
    if (shIsKeyDown(window, SH_KEY_S)) {
        p_model->film_conditions.incident_ray.x += 1.0f * (float)p_engine->time.delta_time;
    }
    
    if (shIsKeyDown(window, SH_KEY_D)) {
        p_model->film_conditions.incident_ray.y -= 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_F)) {
        p_model->film_conditions.incident_ray.y += 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_G)) {
        p_model->film_conditions.incident_ray.z -= 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_H)) {
        p_model->film_conditions.incident_ray.z += 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_Z)) {
        p_model->film_conditions.film_distance_cm -= 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(window, SH_KEY_X)) {
        p_model->film_conditions.film_distance_cm += 1.0f * (float)p_engine->time.delta_time;
    }

    if (shIsKeyDown(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_E)) {//save interface data
        smdWriteLine(&p_model->export_smd, 4, "incident_ray",        SMD_VAR_TYPE_FLOAT32, &p_model->film_conditions.incident_ray);
        smdWriteLine(&p_model->export_smd, 1, "lambda_ag",           SMD_VAR_TYPE_FLOAT32, &p_model->film_conditions.lambda_ag);
        smdWriteLine(&p_model->export_smd, 1, "film_distance_cm",    SMD_VAR_TYPE_FLOAT32, &p_model->film_conditions.film_distance_cm);
        smdWriteLine(&p_model->export_smd, 1, "atom_count",          SMD_VAR_TYPE_UINT32,  &p_model->film_conditions.atom_count);
        smdWriteLine(&p_model->export_smd, p_model->film_conditions.atom_count * 4, "atoms_positions_ag",  SMD_VAR_TYPE_FLOAT32, p_model->lattice_unit_data.atoms_positions_ag);
        smdWriteFile(&p_model->export_smd, "../../smd/model-saved.smd");
        smdExportHandleRelease(&p_model->export_smd);
    }

    if (shIsKeyDown(p_engine->window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(p_engine->window, SH_KEY_L)) {//load saved data
        smdReadFile("../../smd/model-saved.smd", &p_model->saved_smd);
        smdParseMemory(&p_model->saved_smd);
        smdAccessVarByName(&p_model->saved_smd, "incident_ray",       NULL, &p_model->film_conditions.incident_ray);
        smdAccessVarByName(&p_model->saved_smd, "lambda_ag",          NULL, &p_model->film_conditions.lambda_ag);
        smdAccessVarByName(&p_model->saved_smd, "film_distance_cm",   NULL, &p_model->film_conditions.film_distance_cm);
        smdAccessVarByName(&p_model->saved_smd, "atom_count",         NULL, &p_model->film_conditions.atom_count);
        smdAccessVarByName(&p_model->saved_smd, "atoms_positions_ag", NULL,  p_model->lattice_unit_data.atoms_positions_ag);
        smdFileHandleRelease(&p_model->saved_smd);
    }

    return 1;
}

extern uint8_t modelUpdateGui(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "modelUpdateGui: invalid engine memory", return 0);

    Model* p_model = (Model*)p_engine->p_ext;

    ShGui* p_gui = &p_engine->gui;

    char txt[256] = { 0 };
    snprintf(
        txt, 256, "lambda: %.2f A\nincident angle: (%.2f, %.2f, %.2f) rad\nfilm distance: %.2f",
        p_model->film_conditions.lambda_ag,
        p_model->film_conditions.incident_ray.x,
        p_model->film_conditions.incident_ray.y,
        p_model->film_conditions.incident_ray.z,
        p_model->film_conditions.film_distance_cm
    );

    shGuiText(
        p_gui,
        SH_GUI_VEC2_ZERO,
        SH_GUI_COLOR_WHITE,
        20.0f,
        txt,
        SH_GUI_EDGE_LEFT | SH_GUI_EDGE_TOP
    );

    snprintf(
        txt, 256, "fps: %.2f", 1.0f / (float)p_engine->time.delta_time
    );

    shGuiText(
        p_gui,
        SH_GUI_VEC2_ZERO,
        SH_GUI_COLOR_WHITE,
        20.0f,
        txt,
        SH_GUI_EDGE_TOP | SH_GUI_CENTER_WIDTH
    );

    return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus