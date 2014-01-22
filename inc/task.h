extern void startScheduler ( void );
extern void initializeScheduler ( void );
extern void changePriority ( char *name, uint8_t priority );
extern void createTask ( void ( * function_ptr )( void ), char *taskname, uint8_t priority, uint16_t stack_size );
extern void changeStatus ( char *name, uint8_t status );
