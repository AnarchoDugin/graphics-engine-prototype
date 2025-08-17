#include <general/LightSource.h>
#include <glad/glad.h>

LightSource::LightSource(const glm::vec3& position, const glm::vec3& color, Object3D* model) : position(position), color(color),
    lightModel(model) {}

LightSource::~LightSource() {}

void LightSource::draw()
{
    lightModel->render();
}

void LightSource::setAttenuation(float constant, float linear, float quadratic)
{
    constantAttenuation = constant;
    linearAttenuation = linear;
    quadraticAttenuation = quadratic;
}

void LightSource::getAttenuation(float& constant, float& linear, float& quadratic) const
{
    constant = constantAttenuation;
    linear = linearAttenuation;
    quadratic = quadraticAttenuation;
}
