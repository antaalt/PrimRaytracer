
#include <Aka/Aka.h>

#include "Editor/EditorUI.h"
#include "Editor/SceneUI.h"

namespace viewer {

class Viewer : public aka::Application
{
public:
	void onCreate() override;
	void onDestroy() override;
	void onFrame() override;
	void onUpdate(aka::Time::Unit deltaTime) override;
	void onRender() override;
	void onResize(uint32_t width, uint32_t height) override;
private:
	EditorUI m_editor;
	SceneUI *m_sceneUI;
};

};