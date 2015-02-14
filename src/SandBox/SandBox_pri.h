#include "LBS.h"

#define GET_2BYTE_INT(p) (((p)[0] << 8) + (p)[1])
#define GET_4BYTE_INT(p) (((p)[0] << 24) + ((p)[1] << 16) + ((p)[2] << 8) + (p)[3])

#define SET_2BYTE_INT(p, value) (((p)[0] = (value) >> 8), ((p)[1] = (value) & 0xff))
#define SET_4BYTE_INT(p, value) \
	(((p)[0] = (value) >> 24), \
	 ((p)[1] = (value) >> 16), \
	 ((p)[2] = (value) >> 8), \
	 ((p)[3] = (value) & 0xff))


#define GET_8BYTE_INT(p) \
	(((p)[0] << 56) + \
	 ((p)[1] << 48) + \
	 ((p)[2] << 40) + \
	 ((p)[3] << 32) + \
	 ((p)[4] << 24) + \
	 ((p)[5] << 16) + \
	 ((p)[6] << 8)  + \
	 (p)[7])
#define SET_8BYTE_INT(p, value) \
	(((p)[0] = (value) >> 56), \
	 ((p)[1] = (value) >> 48), \
	 ((p)[2] = (value) >> 40), \
	 ((p)[3] = (value) >> 32), \
	 ((p)[4] = (value) >> 24), \
	 ((p)[5] = (value) >> 16), \
	 ((p)[6] = (value) >> 8), \
	 ((p)[7] = (value) & 0xff))
#define NULL_VISUAL (L"(null)")
#define NULL_VALUE (0x0)
#define MEM_fill(obj, i) \
	(memset(&(obj), (i), sizeof((obj))))

typedef enum {
	LPR_NOTHING = 1,
	LPR_JUST_PANIC,
	LPR_ANYTHING
} WarningFlag;

typedef struct ExeEnvironment_tag {
	WarningFlag wflag;

	ExeContainer *exe;
	Loopr_Stack stack;

	/*Loopr_Int32 local_variable_count;
	LocalVariable *local_variable;*/
	LocalVariableMap *local_variable_map;

	Loopr_Int32 function_count;
	struct ExeEnvironment_tag **function;
} ExeEnvironment;

/* execute.c */
Loopr_Value *Loopr_execute(ExeEnvironment *env, Loopr_Boolean top_level);

/* value.c */
Loopr_Byte *Loopr_byte_serialize(const void *data, int length);
#define Loopr_byte_deserialize(dest, data, length) \
	(memcpy((dest), (data), (length)))

Loopr_InfoTable *Loopr_alloc_info_table(Loopr_BasicType type);
Loopr_Value *Loopr_alloc_value(Loopr_BasicType type);
Loopr_Value *Loopr_create_string(Loopr_Byte *data, int *offset);
#define Loopr_create_null() (NULL)
Loopr_Value *Loopr_create_object(Loopr_Value *orig);
Loopr_Value *Loopr_get_init_value(Loopr_BasicType type);

/* coding.c */
Loopr_Byte *Coding_alloc_byte(int length);
ByteContainer *Coding_init_coding_env(void);
int Coding_init_local_variable(ByteContainer *env, char *identifier);
int Coding_get_local_variable_index(ByteContainer *env, char *name);
void Coding_byte_cat(ByteContainer *env, Loopr_Byte *src, int count);
void Coding_push_code(ByteContainer *env, Loopr_Byte code, Loopr_Byte *args, int args_count);
ExeContainer *Coding_alloc_exe_container(ByteContainer *env);
ExeEnvironment *Coding_init_exe_env(ByteContainer *env, WarningFlag wflag);

typedef void (*Walle_Marker)(void);
/* wall-e.c */
void Walle_update_alive_period();
int Walle_get_alive_period();
void Walle_set_header(Loopr_Value *v);
Loopr_Value *Walle_get_header();
void Walle_add_alloc_size(Loopr_Int64 add);
Loopr_Int64 Walle_get_alloc_size();
void Walle_add_threshold(Loopr_Int64 add);
Loopr_Int64 Walle_get_threshold();
void Walle_set_marker(Walle_Marker marker);
Walle_Marker Walle_get_marker();

void Walle_add_object(Loopr_Value *v);
void Walle_dispose_value(Loopr_Value **target);
void Walle_gcollect();
void Walle_check_mem();

void Walle_dispose_environment(ExeEnvironment *env);
void Walle_dispose_byte_container(ByteContainer *env, Loopr_Boolean flag_clean_code);

extern ByteInfo Loopr_Byte_Info[];
extern TypeInfo Loopr_Type_Info[];
