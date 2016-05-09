#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "vector.h"
#include <SDL.h>

void Camera_Init(Vec2d pos, int width, int height);

/**
 * @brief	Gets the Camera that is currently active.		
 */
SDL_Rect Camera_Get_Camera();

void Camera_Set_Editor_Offset(Vec2d offset);
Vec2d Camera_Get_Editor_Offset();
/**
 * @brief	Get's the position of the active camera.
 *
 * @return	Returns the position as a Vec2d.
 */
Vec2d Camera_GetPosition();

/**
 * @brief	Get's the size of the active camera.
 *
 * @return	Returns the size as a Vec2d.
 */
Vec2d Camera_GetSize();

/**
 * @brief	Set's the size of the active camera.
 *
 * @param size	The size you want to set the camera to.
 */
void Camera_SetSize(Vec2d size);

/**
 * @brief	Set's the position of the active camera.
 *
 * @param size	The position you want to set the camera to.
 */
void Camera_SetPosition(Vec2d pos);


#endif