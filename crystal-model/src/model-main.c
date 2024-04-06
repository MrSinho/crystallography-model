#ifdef __cplusplus
extern "C" {
#endif//__cplusplus
 


#include <shengine/shEngine.h>//most of engine declarations are here
#include <shengine/shExport.h>//required when building a shared library

#include "crystal-model/crystal-model.h"

#include <memory.h>




uint8_t SH_ENGINE_EXPORT_FUNCTION model_start(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_start: invalid engine memory", return 0);

    shEngineGuiSetup(p_engine);

    p_engine->p_ext = calloc(1, sizeof(Model));

    shApplicationError(
        p_engine->p_ext == NULL,
        "model_start: invalid engine extension memory",
        return 0
    );

    Model* p_model = (Model*)p_engine->p_ext;

    modelLoadRecovery(p_model);

    ShVkPipelinePool* p_pool                = &p_engine->pipeline_pool;
    ShVkPipeline*     p_pipeline            = &p_pool->pipelines[0];
    uint32_t          swapchain_image_count = p_engine->core.swapchain_image_count;

    VkDevice          device          = p_engine->core.device;
    VkPhysicalDevice  physical_device = p_engine->core.physical_device;
    VkCommandBuffer   cmd_buffer      = p_engine->core.graphics_cmd_buffers[0];
    VkQueue           queue           = p_engine->core.graphics_queue;


    //allocate memory
    shCreateBuffer(
        device,                           //device
        MODEL_SIZE,                       //size
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, //usage
        VK_SHARING_MODE_EXCLUSIVE,        //sharing_mode
        &p_model->staging_buffer          //p_buffer
    );

    shCreateBuffer(
        device,                                                                                                     //device
        MODEL_SIZE,                                                                                                 //size
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, //usage
        VK_SHARING_MODE_EXCLUSIVE,                                                                                  //sharing_mode
        &p_model->dst_buffer                                                                                        //p_buffer
    );

    shAllocateBufferMemory(
        device,                                                                     //device
        physical_device,                                                            //physical_device
        p_model->staging_buffer,                                                    //buffer
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, //property_flags
        &p_model->staging_memory                                                    //p_memory
    );

    shAllocateBufferMemory(
        device,                              //device
        physical_device,                     //physical_device
        p_model->dst_buffer,                 //buffer
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, //property_flags
        &p_model->dst_memory                 //p_memory
    );

    shBindBufferMemory(
        device,                  //device
        p_model->staging_buffer, //buffer
        0,                       //offset
        p_model->staging_memory  //buffer_memory
    );

    shBindBufferMemory(
        device,              //device
        p_model->dst_buffer, //buffer
        0,                   //offset
        p_model->dst_memory  //buffer_memory
    );

    VkBuffer          staging_buffer = p_model->staging_buffer;
    VkDeviceMemory    staging_memory = p_model->staging_memory;
    VkBuffer          dst_buffer     = p_model->dst_buffer;
    VkDeviceMemory    dst_memory     = p_model->dst_memory;

    shPipelinePoolSetDescriptorBufferInfos(
        0,                     //first_descriptor
        swapchain_image_count, //descriptor_count
        dst_buffer,            //buffer
        LATTICE_UNIT_DATA_OFFSET,  //buffer_offset
        LATTICE_UNIT_DATA_SIZE,    //buffer_size
        p_pool                 //p_pipeline_pool
    );

    shPipelinePoolSetDescriptorBufferInfos(
        swapchain_image_count,  //first_descriptor
        swapchain_image_count,  //descriptor_count
        dst_buffer,             //buffer
        FILM_CONDITIONS_OFFSET, //buffer_offset
        FILM_CONDITIONS_SIZE,   //buffer_size
        p_pool                  //p_pipeline_pool
    );


    shPipelinePoolCreateDescriptorSetLayoutBinding(
        0,                                 //binding
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, //descriptor_type
        1,                                 //descriptor_set_count
        VK_SHADER_STAGE_FRAGMENT_BIT,      //shader_stage
        p_pool                             //p_pipeline_pool
    );

    shPipelinePoolCreateDescriptorSetLayoutBinding(
        0,                                 //binding
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, //descriptor_type
        1,                                 //descriptor_set_count
        VK_SHADER_STAGE_FRAGMENT_BIT,      //shader_stage
        p_pool                             //p_pipeline_pool
    );


    shPipelinePoolCreateDescriptorSetLayout(
        device, //device
        0,      //first_binding_idx
        1,      //binding_count
        0,      //set_layout_idx
        p_pool  //p_pipeline_pool
    );

    shPipelinePoolCreateDescriptorSetLayout(
        device,                //device
        1,                     //first_binding_idx
        1,                     //binding_count
        swapchain_image_count, //set_layout_idx
        p_pool                 //p_pipeline_pool
    );


    shPipelinePoolCopyDescriptorSetLayout(
        0,                     //src_set_layout_idx
        0,                     //first_dst_set_layout_idx
        swapchain_image_count, //dst_set_layout_count
        p_pool                 //p_pipeline_pool
    );

    shPipelinePoolCopyDescriptorSetLayout(
        swapchain_image_count, //src_set_layout_idx
        swapchain_image_count, //first_dst_set_layout_idx
        swapchain_image_count, //dst_set_layout_count
        p_pool                 //p_pipeline_pool
    );



    shPipelinePoolCreateDescriptorPool(
        device,                            //device
        0,                                 //pool_idx
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, //descriptor_type
        swapchain_image_count,             //descriptor_count
        p_pool                             //p_pipeline_pool
    );

    shPipelinePoolCreateDescriptorPool(
        device,                            //device
        1,                                 //pool_idx
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, //descriptor_type
        swapchain_image_count,             //descriptor_count
        p_pool                             //p_pipeline_pool
    );



    shPipelinePoolAllocateDescriptorSetUnits(
        device,                            //device
        0,                                 //binding
        0,                                 //pool_idx
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, //descriptor_type
        0,                                 //first_descriptor_set_unit
        swapchain_image_count,             //descriptor_set_unit_count
        p_pool                             //p_pipeline_pool
    );

    shPipelinePoolAllocateDescriptorSetUnits(
        device,                            //device
        0,                                 //binding
        1,                                 //pool_idx
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, //descriptor_type
        swapchain_image_count,             //first_descriptor_set_unit
        swapchain_image_count,             //descriptor_set_unit_count
        p_pool                             //p_pipeline_pool
    );

    shPipelinePoolUpdateDescriptorSetUnits(
        device,                    //device
        0,                         //first_descriptor_set_unit
        swapchain_image_count * 2, //descriptor_set_unit_count
        p_pool                     //p_pipeline_pool
    );

    shPipelineSetVertexInputState(p_pipeline);
    shPipelineCreateInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, SH_FALSE, p_pipeline);

    shPipelineCreateRasterizer(VK_POLYGON_MODE_FILL, SH_FALSE, p_pipeline);
    shPipelineSetMultisampleState(p_engine->core.sample_count, 0.0f, p_pipeline);
    shPipelineSetViewport(
        0, 0, p_engine->window.width, p_engine->window.height,
        0, 0, p_engine->window.width, p_engine->window.height, 
        p_pipeline
    );
    shPipelineColorBlendSettings(SH_FALSE, SH_FALSE, SH_ENGINE_SUBPASS_COLOR_ATTACHMENT_COUNT, p_pipeline);

    uint32_t shader_size = 0;
    char* shader_code = (char*)shReadCode(
        "../../shaders/bin/film.vert.spv",
        "rb",
        &shader_size
    );
    shApplicationError(shader_code == NULL, "model_start: invalid shader code memory", return 0);

    shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
    shPipelineCreateShaderStage(VK_SHADER_STAGE_VERTEX_BIT, p_pipeline);
    free(shader_code);

    shader_code = (char*)shReadCode(
        "../../shaders/bin/film.frag.spv",
        "rb",
        &shader_size
    );
    shApplicationError(shader_code == NULL, "invalid shader code memory", return 0);

    shPipelineCreateShaderModule(device, shader_size, shader_code, p_pipeline);
    shPipelineCreateShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, p_pipeline);
    free(shader_code);



    shApplicationError(
        shPipelineCreateLayout(device, 0, swapchain_image_count * 2, p_pool, p_pipeline) == 0,
        "model_start: failed creating pipeline layout",
        return 0
    );

    shApplicationError(
        shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
        "model_start: failed creating pipeline",
        return 0
    );


    shCreateFences(device, 1, 1, &p_model->copy_fence);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_update(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_update: invalid engine memory", return 0);

    Model*            p_model               = (Model*)p_engine->p_ext;

    VkBuffer          staging_buffer        = p_model->staging_buffer;
    VkDeviceMemory    staging_memory        = p_model->staging_memory;
    VkBuffer          dst_buffer            = p_model->dst_buffer;

    VkDevice          device                = p_engine->core.device;
    VkCommandBuffer   transfer_cmd_buffer   = p_engine->core.transfer_cmd_buffer;
    VkQueue           transfer_queue        = p_engine->core.transfer_queue;

    
    shWriteMemory(
        device,                 //device
        staging_memory,         //memory
        LATTICE_UNIT_DATA_OFFSET,   //offset
        LATTICE_UNIT_DATA_SIZE,     //data_size
        &p_model->lattice_unit_data //p_data
    );

    shWriteMemory(
        device,                   //device
        staging_memory,           //memory
        FILM_CONDITIONS_OFFSET,   //offset
        FILM_CONDITIONS_SIZE,     //data_size
        &p_model->film_conditions //p_data
    );

    shWaitForFences(
        device,               //device
        1,                    //fence_count
        &p_model->copy_fence, //p_fences
        1,                    //wait_for_all
        UINT64_MAX            //timeout_ns
    );

    shResetFences(
        device,              //device
        1,                   //fence_count
        &p_model->copy_fence //p_fences
    );

    shBeginCommandBuffer(transfer_cmd_buffer);

    shCopyBuffer(
        transfer_cmd_buffer,     //transfer_cmd_buffer
        p_model->staging_buffer, //src_buffer
        0,                       //src_offset
        0,                       //dst_offset
        MODEL_SIZE,              //size
        p_model->dst_buffer      //dst_buffer
    );

    shEndCommandBuffer(transfer_cmd_buffer);

    shQueueSubmit(
        1,                              //cmd_buffer_count
        &transfer_cmd_buffer,           //p_cmd_buffers
        transfer_queue,                 //queue
        p_model->copy_fence,            //fence
        0,                              //semaphores_to_wait_for_count
        NULL,                           //p_semaphores_to_wait_for
        VK_PIPELINE_STAGE_TRANSFER_BIT, //wait_stage
        0,                              //signal_semaphore_count
        NULL                            //p_signal_semaphores
    );
    
    shWaitForFences(
        device,               //device
        1,                    //fence_count
        &p_model->copy_fence, //p_fences
        1,                    //wait_for_all
        UINT64_MAX            //timeout_ns
    );

    modelUpdateInputs(p_engine);

    modelUpdateGui(p_engine);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_main_cmd_buffer(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_main_cmd_buffer: invalid engine memory", return 0);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_main_renderpass(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_main_renderpass: invalid engine memory", return 0);

    int32_t           swapchain_image_idx   = p_engine->core.swapchain_image_idx;
    int32_t           swapchain_image_count = p_engine->core.swapchain_image_count;
    VkCommandBuffer   cmd_buffer            = p_engine->core.graphics_cmd_buffers[swapchain_image_idx];
    ShVkPipelinePool* p_pool                = &p_engine->pipeline_pool;
    ShVkPipeline*     p_pipeline            = &p_pool->pipelines[0];
    Model*            p_model               = (Model*)p_engine->p_ext;

    shPipelineBindDescriptorSetUnits(
        cmd_buffer,
        0,
        swapchain_image_idx,
        1,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        0,
        NULL,
        p_pool,
        p_pipeline
    );

    shPipelineBindDescriptorSetUnits(
        cmd_buffer,
        1,
        swapchain_image_count + swapchain_image_idx,
        1,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        0,
        NULL,
        p_pool,
        p_pipeline
    );


    shBindPipeline(cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pipeline);

    shDraw(cmd_buffer, FILM_VERTEX_COUNT, 0, 1, 0);

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_frame_resize(
    ShEngine* p_engine
) {
    shApplicationError(p_engine == NULL, "model_frame_resize: invalid engine memory", return 0);

    ShVkPipeline* p_pipeline = &p_engine->pipeline_pool.pipelines[0];
    VkDevice      device     = p_engine->core.device;

    shDestroyPipeline(device, p_pipeline->pipeline);
    shPipelineSetViewport(
        0, 0, p_engine->window.width, p_engine->window.height,
        0, 0, p_engine->window.width, p_engine->window.height,
        p_pipeline
    );

    shApplicationError(
        shSetupGraphicsPipeline(device, p_engine->core.renderpass, p_pipeline) == 0,
        "noise_frame_resize: failed creating pipeline",
        return 0
    );

    return 1;
}

uint8_t SH_ENGINE_EXPORT_FUNCTION model_close(
    ShEngine* p_engine
) {
     shApplicationError(p_engine       == NULL, "model_close: invalid engine memory",           return 0);
    shApplicationError(p_engine->p_ext == NULL, "model_close: invalid engine extension memory", return 0);

    
    VkDevice          device      =  p_engine->core.device;
    ShVkPipelinePool* p_pool      = &p_engine->pipeline_pool;
    ShVkPipeline*     p_pipeline  = &p_pool->pipelines[0];
    Model*            p_model     = (Model*)p_engine->p_ext;


    shWaitDeviceIdle(device);
    
    shDestroyFences(device, 1, &p_model->copy_fence);

    shPipelinePoolDestroyDescriptorPools     (device, 0, 1, p_pool);
    shPipelinePoolDestroyDescriptorSetLayouts(device, 0, 1, p_pool);

	shPipelineDestroyShaderModules(device, 0, 2, p_pipeline);
	shPipelineDestroyLayout       (device, p_pipeline);
	shDestroyPipeline             (device, p_pipeline->pipeline);
	
    
	shClearPipeline(p_pipeline);


    if (p_engine->p_ext != NULL) {
        free(p_engine->p_ext);
    }

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