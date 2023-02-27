/**
 * \file interp_show.h
 */

#if !defined(INTERP_SHOW_H)
#define INTERP_SHOW_H

#ifdef __cplusplus
extern "C" {
#endif

/** Print the contents of \e data_t . This is currently the only walk function
 * accepted by \c interpWalk() .
 *
 * \param entry pointer to \e data_t
 * \param user \e void pointer to user data to be echoed by callbacks
 * \return \c bool indicating success

 */
bool interpShow(void * const entry, void * const user);

#ifdef __cplusplus
}
#endif

#endif
