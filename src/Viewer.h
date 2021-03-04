
#include <Aka/Aka.h>

namespace viewer {

class Viewer : public aka::Application
{
public:
	void initialize() override;
	void destroy() override;
	void update(aka::Time::Unit deltaTime) override;
	void render() override;
	void resize(uint32_t width, uint32_t height) override;
};

};