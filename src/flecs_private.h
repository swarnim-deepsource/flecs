#ifndef FLECS_PRIVATE_H
#define FLECS_PRIVATE_H

/* This file contains declarations to private flecs functions */

#include "types.h"
#include "flecs/util/entity_index.h"
#include "flecs/util/table.h"

/* Initialize builtin systems */
void ecs_init_builtins(
    ecs_world_t *world);

/* Initialize prefab builtin systems */
void ecs_init_prefab_builtins(
    ecs_world_t *world);

/* Initialize timer builtin systems */
void ecs_init_timer_builtins(
    ecs_world_t *world);

////////////////////////////////////////////////////////////////////////////////
//// Entity API
////////////////////////////////////////////////////////////////////////////////

/* Get prefab from type, even if type was introduced while in progress */
ecs_entity_t ecs_get_prefab_from_type(
    ecs_world_t *world,
    ecs_stage_t *stage,
    bool is_new_table,
    ecs_entity_t entity,
    ecs_type_t type_id);

/* Mark an entity as being watched. This is used to trigger automatic rematching
 * when entities used in system expressions change their components. */
void ecs_set_watch(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_entity_t entity);

/* Does one of the entity containers has specified component */
bool ecs_components_contains_component(
    ecs_world_t *world,
    ecs_type_t table_type,
    ecs_entity_t component,
    ecs_entity_t flags,
    ecs_entity_t *entity_out);

/* Get pointer to a component */
void* ecs_get_ptr_intern(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_entity_t entity,
    ecs_entity_t component,
    bool staged_only,
    bool search_prefab);

ecs_entity_t ecs_get_entity_for_component(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_type_t type_id,
    ecs_entity_t component);

void ecs_clear_w_filter(
    ecs_world_t *world,
    const ecs_filter_t *filter);

/* Get entity info */
bool ecs_get_info(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_entity_t entity,
    ecs_entity_info_t *info); 

/* Initialize component value(s) */
void ecs_run_init_actions(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_table_t *table,
    ecs_data_t *data,
    uint32_t row,
    uint32_t count,
    ecs_entities_t components);
    
/* Deinitialize component value(s) */
void ecs_run_deinit_actions(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_table_t *table,
    ecs_data_t *data,
    uint32_t row,
    uint32_t count,
    ecs_entities_t components);

/* Get actual row from record row */
int32_t ecs_record_to_row(
    int32_t row, 
    bool *is_watched_out);

/* Convert actual row to record row */
int32_t ecs_row_to_record(
    int32_t row, 
    bool is_watched);

////////////////////////////////////////////////////////////////////////////////
//// World API
////////////////////////////////////////////////////////////////////////////////

/* Notify systems that there is a new table, which triggers matching */
void ecs_notify_queries_of_table(
    ecs_world_t *world,
    ecs_table_t *table);

/* Activate system (move from inactive array to on_update array or vice versa) */
void ecs_world_activate_system(
    ecs_world_t *world,
    ecs_entity_t system,
    ecs_system_kind_t kind,
    bool active);

/* Get current thread-specific stage */
ecs_stage_t *ecs_get_stage(
    ecs_world_t **world_ptr);

/* Get array for system kind */
ecs_vector_t** ecs_system_array(
    ecs_world_t *world,
    ecs_system_kind_t kind);

/* Get component callbacks */
ecs_component_data_t *ecs_get_component_data(
    ecs_world_t *world,
    ecs_entity_t component);

////////////////////////////////////////////////////////////////////////////////
//// Stage API
////////////////////////////////////////////////////////////////////////////////

/* Initialize stage data structures */
void ecs_stage_init(
    ecs_world_t *world,
    ecs_stage_t *stage);

/* Deinitialize stage */
void ecs_stage_deinit(
    ecs_world_t *world,
    ecs_stage_t *stage);

/* Merge stage with main stage */
void ecs_stage_merge(
    ecs_world_t *world,
    ecs_stage_t *stage);


////////////////////////////////////////////////////////////////////////////////
//// Type API
////////////////////////////////////////////////////////////////////////////////

ecs_type_t ecs_type_find_intern(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_entity_t *buf,
    int32_t count);

/* Merge add/remove types */
ecs_type_t ecs_type_merge_intern(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_type_t cur_id,
    ecs_type_t to_add_id,
    ecs_type_t to_remove_id);

/* Test if type_id_1 contains type_id_2 */
ecs_entity_t ecs_type_contains(
    ecs_world_t *world,
    ecs_type_t type_id_1,
    ecs_type_t type_id_2,
    bool match_all,
    bool match_prefab);

/* Test if type contains component */
bool ecs_type_has_entity_intern(
    ecs_world_t *world,
    ecs_type_t type,
    ecs_entity_t component,
    bool match_prefab);

/* Add component to type */
ecs_type_t ecs_type_add_intern(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_type_t type,
    ecs_entity_t component);

/* Get index for entity in type */
int16_t ecs_type_index_of(
    ecs_type_t type,
    ecs_entity_t component);

/* Get number of containers (parents) for a type */
int32_t ecs_type_container_depth(
   ecs_world_t *world,
   ecs_type_t type,
   ecs_entity_t component);

/** Utility to iterate over prefabs in type */
int32_t ecs_type_get_prefab(
    ecs_type_t type,
    int32_t n);

/* Find entity in prefabs of type */
ecs_entity_t ecs_find_entity_in_prefabs(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_type_t type,
    ecs_entity_t component,
    ecs_entity_t previous);


////////////////////////////////////////////////////////////////////////////////
//// Table API
////////////////////////////////////////////////////////////////////////////////

/* Initialize root table */
void ecs_init_root_table(
    ecs_world_t *world,
    ecs_stage_t *stage);

/* Bootstrap first table to store component data */ 
ecs_table_t *ecs_bootstrap_component_table(
    ecs_world_t *world);

void ecs_table_register_query(
    ecs_world_t *world,
    ecs_table_t *table,
    ecs_query_t *query);

/* Deinitialize table. This invokes all matching on_remove systems */
void ecs_table_deinit_components(
    ecs_world_t *world,
    ecs_table_t *table);

/* Free table */
void ecs_table_free(
    ecs_world_t *world,
    ecs_table_t *table); 

/* Clear data in columns */
void ecs_table_replace_data(
    ecs_world_t *world,
    ecs_table_t *table,
    ecs_data_t *data);
    
/* Merge data of one table into another table */
void ecs_table_merge(
    ecs_world_t *world,
    ecs_table_t *new_table,
    ecs_table_t *old_table);

void ecs_table_swap(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_table_t *table,
    ecs_data_t *data,
    int32_t row_1,
    int32_t row_2,
    ecs_record_t *record_ptr_1,
    ecs_record_t *record_ptr_2);

void ecs_table_move_back_and_swap(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_table_t *table,
    ecs_data_t *data,
    int32_t row,
    int32_t count);

////////////////////////////////////////////////////////////////////////////////
//// Query API
////////////////////////////////////////////////////////////////////////////////

ecs_query_t*  ecs_query_new_w_sig(
    ecs_world_t *world,
    ecs_entity_t system, 
    ecs_sig_t *sig);

void ecs_query_activate_table(
    ecs_world_t *world,
    ecs_query_t *query,
    ecs_table_t *table,
    bool active);

void ecs_query_match_table(
    ecs_world_t *world,
    ecs_query_t *query,
    ecs_table_t *table);

////////////////////////////////////////////////////////////////////////////////
//// Signature API
////////////////////////////////////////////////////////////////////////////////

void ecs_sig_init(
    ecs_world_t *world,
    const char *name,
    const char *expr,
    ecs_sig_t *sig);

void ecs_sig_deinit(
    ecs_sig_t *sig);



////////////////////////////////////////////////////////////////////////////////
//// System API
////////////////////////////////////////////////////////////////////////////////

void ecs_system_init_base(
    ecs_world_t *world,
    EcsSystem *base_data);

/* Compute the AND type from the system columns */
void ecs_system_compute_and_families(
    ecs_world_t *world,
    EcsSystem *system_data);

void ecs_enable_intern(
    ecs_world_t *world,
    ecs_entity_t system,
    EcsSystem *system_data,
    bool enabled,
    bool by_user);

/* Invoked when system becomes active / inactive */
void ecs_system_activate(
    ecs_world_t *world,
    ecs_entity_t system,
    bool activate);

/* Invoke status action */
void ecs_invoke_status_action(
    ecs_world_t *world,
    ecs_entity_t system,
    EcsColSystem *system_data,
    ecs_system_status_t status);

/* Check if all non-table column constraints are met */
bool ecs_sig_check_constraints(
    ecs_world_t *world,
    ecs_sig_t *sig);

/* Create new table system */
ecs_entity_t ecs_new_col_system(
    ecs_world_t *world,
    const char *name,
    ecs_system_kind_t kind,
    ecs_sig_t *sig,
    ecs_system_action_t action);

/* Notify column system of a new table, which initiates system-table matching */
void ecs_query_notify_of_table(
    ecs_world_t *world,
    ecs_query_t *query,
    ecs_table_t *table);

/* Notify row system of a new type, which initiates system-type matching */
void ecs_row_system_notify_of_type(
    ecs_world_t *world,
    ecs_stage_t *stage,
    ecs_entity_t system,
    ecs_type_t type);

/* Activate table for system (happens if table goes from empty to not empty) */
void ecs_system_activate_table(
    ecs_world_t *world,
    ecs_entity_t system,
    ecs_table_t *table,
    bool active);

/* Internal function to run a system */
ecs_entity_t ecs_run_intern(
    ecs_world_t *world,
    ecs_world_t *real_world,
    ecs_entity_t system,
    float delta_time,
    uint32_t offset,
    uint32_t limit,
    const ecs_filter_t *filter,
    void *param);

/* Run a task (periodic system that is not matched against any tables) */
void ecs_run_task(
    ecs_world_t *world,
    ecs_entity_t system);

/* Invoke row system */
ecs_type_t ecs_run_row_system(
    ecs_world_t *world,
    ecs_entity_t system,
    ecs_table_t *table,
    ecs_data_t *data,
    int32_t offset,
    int32_t limit);

/* Callback for parse_component_expr that stores result as ecs_sig_column_t's */
int ecs_parse_signature_action(
    ecs_world_t *world,
    const char *system_id,
    const char *sig,
    int column,    
    ecs_sig_from_kind_t from_kind,
    ecs_sig_oper_kind_t oper_kind,
    ecs_sig_inout_kind_t inout_kind,
    const char *component_id,
    const char *source_id,
    void *data);

/* Trigger rematch of system */
void ecs_rematch_query(
    ecs_world_t *world,
    ecs_query_t *query);

/* Re-resolve references of system after table realloc */
void ecs_revalidate_query_refs(
    ecs_world_t *world,
    ecs_query_t *query);

void ecs_measure_frame_time(
    ecs_world_t *world,
    bool enable);

void ecs_measure_system_time(
    ecs_world_t *world,
    bool enable);


////////////////////////////////////////////////////////////////////////////////
//// Worker API
////////////////////////////////////////////////////////////////////////////////

/* Compute schedule based on current number of entities matching system */
void ecs_schedule_jobs(
    ecs_world_t *world,
    ecs_entity_t system);

/* Prepare jobs */
void ecs_prepare_jobs(
    ecs_world_t *world,
    ecs_entity_t system);

/* Run jobs */
void ecs_run_jobs(
    ecs_world_t *world);


////////////////////////////////////////////////////////////////////////////////
//// Time API
////////////////////////////////////////////////////////////////////////////////

void ecs_os_time_setup(void);

uint64_t ecs_os_time_now(void);

void ecs_os_time_sleep(
    int32_t sec, 
    int32_t nanosec);


////////////////////////////////////////////////////////////////////////////////
//// Utilities
////////////////////////////////////////////////////////////////////////////////

/* Convert 64bit value to ecs_record_t type. ecs_record_t is stored as 64bit int in the
 * entity index */
ecs_record_t ecs_to_row(
    uint64_t value);

/* Get 64bit integer from ecs_record_t */
uint64_t ecs_from_row(
    ecs_record_t record);

/* Utility that print a descriptive error string*/
//void ecs_print_error_string(const char *error_description, const char* signature, const char* system_id, const char* component_id);
//void ecs_print_error_string(const char* signature, const char *system_id, const char *error_description, const char *component_id);

/* Utility that parses system signature */
int ecs_parse_expr(
    ecs_world_t *world,
    const char *sig,
    ecs_parse_action_t action,
    const char *system_id,
    void *ctx);

/* Test whether signature has columns that must be retrieved from a table */
bool ecs_needs_tables(
    ecs_world_t *world,
    const char *signature,
    const char *system_id);

/* Count number of columns signature */
int32_t ecs_columns_count(
    const char *sig);

#define assert_func(cond) _assert_func(cond, #cond, __FILE__, __LINE__, __func__)
void _assert_func(
    bool cond,
    const char *cond_str,
    const char *file,
    int32_t line,
    const char *func);

#endif
