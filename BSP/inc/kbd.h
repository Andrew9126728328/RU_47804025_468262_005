#ifndef __KBD_H__
#define __KBD_H__

/***************************************************************************************
 * X macros for creating a list of KEY controls
 *
 *                 	        ROW_XX  	Port 	            Pin 	        
 */
#define KEY_ROW_XLIST(key_row_xmacro) \
		key_row_xmacro(	    ROW_1,      ROW_1_GPIO_PORT,	ROW_1_PIN) \
		key_row_xmacro(	    ROW_2,      ROW_2_GPIO_PORT,	ROW_2_PIN) \
		key_row_xmacro(	    ROW_3,      ROW_3_GPIO_PORT,	ROW_3_PIN) \
		key_row_xmacro(	    ROW_4,      ROW_4_GPIO_PORT,	ROW_4_PIN) \
		key_row_xmacro(	    ROW_5,      ROW_5_GPIO_PORT,	ROW_5_PIN) 
/*
 *                 	        COL_XX  	Port 	            Pin 	        
 */
#define KEY_COL_XLIST(key_column_xmacro) \
		key_column_xmacro(	COL_1,      COL_1_GPIO_PORT,	COL_1_PIN) \
		key_column_xmacro(	COL_2,      COL_2_GPIO_PORT,	COL_2_PIN) \
		key_column_xmacro(	COL_3,      COL_3_GPIO_PORT,	COL_3_PIN) \
		key_column_xmacro(	COL_4,      COL_4_GPIO_PORT,	COL_4_PIN) \
		key_column_xmacro(	COL_5,      COL_5_GPIO_PORT,	COL_5_PIN) \
		key_column_xmacro(	COL_6,      COL_6_GPIO_PORT,	COL_6_PIN)
		
/* Нумерация рядов */
enum 
{
#define key_row_xmacro(ndx, port, pin) ndx,
	KEY_ROW_XLIST(key_row_xmacro)
#undef key_row_xmacro
    ROW_TOTAL
};
/* Нумерация столбцов */
enum
{
#define key_column_xmacro(_ndx, _port, _pin) _ndx,
	KEY_COL_XLIST(key_column_xmacro)
#undef key_column_xmacro
    COL_TOTAL
};

/*******************************************************************************/ 
/**
* @brief Объект keyboard для работы с клавиатурой BSP
* @details Вся работа с клавиатурой осуществляется только через этот объект
*/ 
typedef struct kbd_s
{
	void (*set_column)(size_t colum);
	void (*reset_column)(size_t colum);
	int (*read_row)(size_t colum);
}kbd_t;
extern const kbd_t kbd;
#endif /* __KBD_H__ */