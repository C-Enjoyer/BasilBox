#ifndef INC_COUNTER_H_
#define INC_COUNTER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"

/* Exported types ------------------------------------------------------------*/
typedef void(*counter_cb_t)(void);

typedef struct counter
{
	uint32_t value;
	uint32_t maximum;
	counter_cb_t callback;
	bool autoReload;
} counter_t;

/* Exported constants --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/***
	* Initializes possibly uninitialized counter to default values (0, NULL, false...)
	* @param cnt Pointer to the memory to initialize.
	*/
void counter_initialize(counter_t* pCounter) __attribute__((nonnull));

/***
	* Set the callback that is to be called once the counter reaches its maximum.
	* @param pCounter Pointer to the counter to manipulate.
	* @param cb Callback to set.
	*/
void counter_setCallback(counter_t* pCounter, counter_cb_t cb) __attribute__((nonnull));

/***
	* Removes the callback that is set on the counter.
	* @param pCounter Pointer to the counter to manipulate.
	*/
void counter_clearCallback(counter_t* pCounter) __attribute__((nonnull));

/***
	* Sets the auto-reload behavior of the counter.
	* If true, the counter will restart once the counter reaches its maximum.
	* If false, the counter will be single-shot.
	* @param pCounter Pointer to the counter to manipulate.
	* @param autoReload Value to set.
	*/
void counter_setAutoReload(counter_t* pCounter, bool autoReload) __attribute__((nonnull));

/***
	* Starts the counter to fire its callback after \p maximum calls to `tick`.
	* @param pCounter Pointer to the counter to manipulate.
	* @param maximum The amount of ticks necessary to call the callback.
	*/
void counter_start(counter_t* pCounter, uint32_t maximum) __attribute__((nonnull));

/***
	* Ticks towards calling the callback. If no callback is set, no external effect can be observed.
	* If auto-reload is enabled the counter will furthermore wrap-around at its maximum and be able to start again;
	* if it is disabled the counter will stop once it reaches its maximum.
	* @param pCounter Pointer to the counter to manipulate.
	*/
void counter_tick(counter_t* pCounter) __attribute__((nonnull));

/***
	* Stops the counter. Calls to `tick` will no longer change internal state.
	* @param pCounter Pointer to the counter to manipulate.
	*/
void counter_stop(counter_t* pCounter) __attribute__((nonnull));

/**
	* Check if the counter is currently running, i.e. counting towards a target.
	* @param pCounter Pointer to the counter to check.
	* @return true if the counter is started, false otherwise.
	*/
bool counter_isActive(counter_t* pCounter) __attribute__((nonnull));

#ifdef __cplusplus
}
#endif

#endif /* INC_COUNTER_H_ */