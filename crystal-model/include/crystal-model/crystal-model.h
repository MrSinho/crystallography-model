#ifndef CRYSTAL_MODEL_H
#define CRYSTAL_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



/**
 * @struct modelvec4
 * @brief Represents a 4D vector used in crystallography simulations.
 */
typedef struct modelvec4 {
    float x; /**< The x-coordinate of the vector. */
    float y; /**< The y-coordinate of the vector. */
    float z; /**< The z-coordinate of the vector. */
    float w; /**< The w-coordinate of the vector. */
} modelvec4;

/**
 * @struct modelvec2
 * @brief Represents a 2D vector used in crystallography simulations.
 */
typedef struct modelvec2 {
    float x; /**< The x-coordinate of the vector. */
    float y; /**< The y-coordinate of the vector. */
    float z; /**< The z-coordinate of the vector. */
    float w; /**< The w-coordinate of the vector. */
} modelvec2;


/**
 * @brief Offset of lattice unit data within the staging and destination vulkan buffers.
 */
#define LATTICE_UNIT_DATA_OFFSET  0

/**
 * @brief Size of lattice unit data within the staging and destination vulkan buffers.
 */
#define LATTICE_UNIT_DATA_SIZE    sizeof(LatticeUnit)

/**
 * @brief Offset of film conditions data within the staging and destination vulkan buffers.
 */
#define FILM_CONDITIONS_OFFSET    LATTICE_UNIT_DATA_SIZE

/**
 * @brief Size of film conditions data within the staging and destination vulkan buffers.
 */
#define FILM_CONDITIONS_SIZE      sizeof(FilmConditions)

/**
 * @brief Total size of the model structure, which is equal to the size of the staging and destination vulkan buffers.
 */
#define MODEL_SIZE                (LATTICE_UNIT_DATA_SIZE + FILM_CONDITIONS_SIZE)



/**
 * @brief Number of vertices of a rectangular film mesh, which is made up of two triangles.
 */
#define FILM_VERTEX_COUNT 6

/**
 * @brief Structure representing film conditions in the crystallography model.
 */
typedef struct FilmConditions {
    modelvec4 incident_ray;     /**< Incident ray vector. */
    float     lambda_ag;        /**< Wavelength of the incident ray in angstroms. */
    float     film_distance_cm; /**< Distance from the film to the crystal in centimeters. */
    uint32_t  atom_count;       /**< Number of atoms in the lattice unit. */
} FilmConditions;



/**
 * @brief Maximum number of atoms in a lattice unit.
 */
#define MODEL_LATTICE_UNIT_MAX_ATOM_COUNT 256

/**
 * @struct LatticeUnit
 * @brief  Structure representing a lattice unit in the crystallography model.
 */
typedef struct LatticeUnit {
    modelvec4 atoms_positions_ag[MODEL_LATTICE_UNIT_MAX_ATOM_COUNT]; /**< Array of atoms positions in the lattice unit. */
} LatticeUnit;



/**
 * @struct Model
 * @brief  Structure representing the crystallography model properties and information.
 */
typedef struct Model {
    LatticeUnit     lattice_unit_data; /**< Data representing lattice unit.                        */
    FilmConditions  film_conditions;   /**< Conditions and parameters of the light receiving film. */
    VkFence         copy_fence;        /**< Vulkan fence for copying data.                         */
    VkBuffer        staging_buffer;    /**< Vulkan buffer for staging data.                        */
    VkBuffer        dst_buffer;        /**< Vulkan buffer for destination data.                    */
    VkDeviceMemory  staging_memory;    /**< Vulkan device memory for staging buffer.               */
    VkDeviceMemory  dst_memory;        /**< Vulkan device memory for destination buffer.           */
    SmdFileHandle   recovery_smd;      /**< Smd file handle used to load recovery data.            */
    SmdFileHandle   saved_smd;         /**< Smd file handle used to load saved data.               */
    SmdExportHandle export_smd;        /**< Smd file handle used to export model data.             */
} Model;



/**
 * @brief Loads recovery data for the crystallography model.
 *
 * This function loads recovery data for the crystallography model,
 * allowing it to resume from a previous state.
 *
 * @param p_model Pointer to a valid @ref Model structure.
 * 
 * @return 1 if the recovery data was successfully loaded, otherwise returns zero.
 */
extern uint8_t modelLoadRecovery(
    Model* p_model
);

/**
 * @brief Updates the inputs for the crystallography model.
 *
 * This function updates the inputs required for the crystallography model,
 * such as user interactions and input data.
 *
 * @param p_engine Valid pointer to a <a href="https://mrsinho.github.io/shengine-docs/struct_sh_engine.html">ShEngine</a> structure.
 * 
 * @return 1 if the inputs were successfully updated, otherwise returns zero.
 */
extern uint8_t modelUpdateInputs(
    ShEngine* p_engine
);

/**
 * @brief Updates the graphical user interface (GUI) for the crystallography model.
 *
 * This function updates the graphical user interface (GUI) elements
 * for the crystallography model, such as displaying simulation parameters
 * and receiving user input.
 *
 * @param p_engine Valid pointer to a <a href="https://mrsinho.github.io/shengine-docs/struct_sh_engine.html">ShEngine</a> structure.
 * 
 * @return 1 if the GUI was successfully updated, otherwise returns zero.
 */
extern uint8_t modelUpdateGui(
    ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//_CRYSTAL_MODEL_H