#pragma once

#include "IRenderable.h"
#include <SDL/SDL.h>
#include "Color.h"

namespace GameEngine {
	class IGraphicsRenderer
	{
	public:
		virtual ~IGraphicsRenderer() { }

		virtual SDL_Renderer* getWindowRenderer() = 0;

		virtual void AddRenderable(IRenderable *renderable) = 0;

		virtual void RemoveRenderable(IRenderable *renderable) = 0;

		virtual void RemoveRenderable(SDL_Point point) = 0;

		virtual void RenderAll(b2Vec2 cameraPos) = 0;

		virtual void SetRenderColor(Color color) = 0;
	};
}
