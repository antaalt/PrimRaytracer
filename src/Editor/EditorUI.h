#pragma once

#include <Aka/Aka.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#if defined(AKA_USE_OPENGL)
#include <backends/imgui_impl_opengl3.h>
#endif
#if defined(AKA_USE_D3D11)
#include <backends/imgui_impl_dx11.h>
#endif

namespace viewer {

struct EditorWidget
{
public:
	virtual void initialize() {}
	virtual void destroy() {}
	virtual void update(aka::Time::Unit deltaTime) {}
	virtual void draw() {}
};

class EditorUI
{
public:
	EditorUI();
	~EditorUI();

	void initialize();
	void destroy();

	template <typename T, typename... Args>
	T* attach(Args&&... args) { m_widgets.push_back(new T(std::forward<Args>(args)...)); return reinterpret_cast<T*>(m_widgets.back()); }

	void frame();
	void update(aka::Time::Unit deltaTime);
	void draw();
	void render();
	bool focused() const;
private:
	std::vector<EditorWidget*> m_widgets;
};

};