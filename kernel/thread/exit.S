.global __exit

/**
 * @file kernel/thread/exit.S
 * @brief Exit Call of Thread
 */


.section app.text
/**
 * @fn void __exit(int exit_value)
 * @brief Exit call of thread
 */
__exit:
	mov x8, 60
	svc 0

