#version 450



layout (location = 0) in  vec4 frag_position;
layout (location = 0) out vec4 fragColor;



#define LATTICE_UNIT_MAX_ATOM_COUNT 256

layout (std430, set = 0, binding = 0) buffer LatticeUnitData {
    vec4 atoms_positions[LATTICE_UNIT_MAX_ATOM_COUNT];
} latticeUnitData;



layout (std140, set = 1, binding = 0) uniform FilmConditions {
    //vec2  film_size_m;
    vec4  incident_ray;
    float lambda_angstroms;
    float film_distance_cm;
    uint  atom_count;
} filmConditions;



#define pi 3.1415926535f

#define MAX_N_ATTEMPTS 16//it doubles

void main() {

    float interference = 0.0f;

/*
    Equazione di lawe
    k \lambda = d cos(a_s) - d cos(a_i)
*/

    float diffraction_angles_x[MAX_N_ATTEMPTS];
    float diffraction_angles_y[MAX_N_ATTEMPTS];


    float incident_x = filmConditions.incident_ray.x;
    float incident_y = filmConditions.incident_ray.y;


    vec3 intensity = vec3(0.0f);

    int diffraction_angle_idx = 0; 

    for (int n = -(MAX_N_ATTEMPTS / 2); n < (MAX_N_ATTEMPTS / 2); n++) {

        for (int atom_idx = 0; atom_idx < filmConditions.atom_count; atom_idx += 2) {//prova per unità vettoriale

            vec3 d    = latticeUnitData.atoms_positions[atom_idx].xyz - latticeUnitData.atoms_positions[atom_idx+1].xyz;

            float d_x = distance(latticeUnitData.atoms_positions[atom_idx].x, latticeUnitData.atoms_positions[atom_idx+1].x);
            float d_y = distance(latticeUnitData.atoms_positions[atom_idx].y, latticeUnitData.atoms_positions[atom_idx+1].y);
            float d_z = distance(latticeUnitData.atoms_positions[atom_idx].z, latticeUnitData.atoms_positions[atom_idx+1].z);
            
            float acos_arg_x = 0.0f;
            float acos_arg_y = 0.0f;
            float acos_arg_z = 0.0f;

            int scatter_x_visible = 1;
            int scatter_y_visible = 1;
            int scatter_z_visible = 1;

#if 0
            
            //vec3 s_x = n * filmConditions.lambda_angstroms / d_x + filmConditions.incident_ray.xyz;
            //vec3 s_y = n * filmConditions.lambda_angstroms / d_y + filmConditions.incident_ray.xyz;
            vec3 s = n * filmConditions.lambda_angstroms / d + filmConditions.incident_ray.xyz;//is is normalized

            intensity.x = 0.01f / length(distance(filmConditions.film_distance_cm * s.x, (frag_position.x * 10) + 0.5 * 10));
            intensity.y = 0.01f / length(distance(filmConditions.film_distance_cm * s.y, (frag_position.y * 10) + 0.5 * 10));
#endif

#if 1
            if (d_x != 0.0f) {
                acos_arg_x = n * filmConditions.lambda_angstroms / d_x + dot(normalize(d.xyz), normalize(filmConditions.incident_ray.xyz));
            }
            else{
                scatter_y_visible = 0;//exclude result
                acos_arg_x = 1.0E9f;
            }

            if (d_y != 0.0f) {
                acos_arg_y = n * filmConditions.lambda_angstroms / d_y + dot(normalize(d.xyz), normalize(filmConditions.incident_ray.xyz));
            }   
            else {
                scatter_x_visible = 0;//exclude result
                acos_arg_y = 1.0E9f;
            }

            if (d_z != 0.0f) {
                acos_arg_z = n * filmConditions.lambda_angstroms / d_z + dot(normalize(d.xyz), normalize(filmConditions.incident_ray.xyz));
            }
            else {
                scatter_z_visible = 0;
                acos_arg_z = 1.0E9f;
            }


            float alpha_s_x  = 0.0f;
            float alpha_s_y  = 0.0f;
            float alpha_s_z  = 0.0f;

            //invert x and y, it's a diffraction cone

            if (abs(acos_arg_x) > 1) {
                scatter_y_visible = 0;//exclude result
            }
            else {
                alpha_s_y = acos(acos_arg_x);
            }

            if (abs(acos_arg_y) > 1) {
                scatter_x_visible = 0;//exclude result
            }
            else {
                alpha_s_x = acos(acos_arg_y);
            }

            if (abs(acos_arg_z) > 1) {
                scatter_z_visible = 0;//exclude result
            }
            else {
                alpha_s_z = acos(acos_arg_z);
            }

            //    intensity += 0.01f / distance((frag_position.xy * pi) + 0.5 * pi, vec2(alpha_s_y, alpha_s_x));
            //intensity += 0.01f / distance((frag_position.y * pi) + 0.5 * pi, alpha_s_x);
            //intensity += 0.01f / distance((frag_position.x * pi) + 0.5 * pi, alpha_s_y);
            
            if (scatter_x_visible == 1) {//horizontal lines
                intensity.x += 0.005f / distance((frag_position.y * pi) + 0.5 * pi, alpha_s_x);
            }
            if (scatter_y_visible == 1) {//vertical lines
                intensity.y += 0.005f / distance((frag_position.x * pi) + 0.5 * pi, alpha_s_y);
            }
            if (scatter_z_visible == 1) {//xy cone
                //intensity.z += 0.01f / distance((frag_position.x * pi) + 0.5 * pi, alpha_s_z);
                //intensity.z += 0.01f / distance((frag_position.y * pi) + 0.5 * pi, alpha_s_z);
                //intensity.z += 0.01f / distance((frag_position.z * pi) + 0.5 * pi, alpha_s_z);

                //if (frag_position.x  < abs(alpha_s_z / 100) && frag_position.y < abs(alpha_s_z / 100)) {
                //    intensity.z = 1.0f;
                //}

                //float r = distance(frag_position.xy, vec2(0.0f));
                //if (r < abs(alpha_s_z)) {
                //    fragColor = vec4(1.0f);
                //}
                
            }
#endif//0
        }
        
    }

    float intersection_threshold = 0.09f;
    if (intensity.x > intersection_threshold && intensity.y > intersection_threshold) {
        //intensity.z = intensity.x * intensity.y / 2.0;
        intensity.z = 1.0f;
    }

    fragColor = vec4(intensity.x, intensity.y, intensity.z, 1.0f);
    fragColor = vec4(intensity.y, intensity.y, intensity.y, 1.0f);
    fragColor = vec4(intensity.x, intensity.x, intensity.x, 1.0f);
    fragColor = vec4(intensity.z);

    //float r = distance(frag_position.xy, vec2(0.0f));
    //if (r < 0.5f) {
    //    fragColor = vec4(1.0f);
    //}

}
