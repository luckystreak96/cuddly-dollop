//
// Created by yanik on 29/03/19.
//

#ifndef PROJECT_GRAPHICS_COMPONENT_H
#define PROJECT_GRAPHICS_COMPONENT_H

#include "iComponent.h"

#include "effect_buffer.h"
#include "spatial_object.h"
#include "textured_object.h"

// Abstracts coordination of drawing with textures and positions
class GraphicsComponent : public IComponent, public SpatialObject, public TexturedObject
{
public:
    GraphicsComponent(std::string modelName = "CENTERED_TILE", std::string texPath = std::string("res/sprites/tiles/mushroom.png"));
    ~GraphicsComponent();

    Transformation* GetModelMat() { return &m_modelMat; };
    void set_scale(Vector3f scale);

    virtual void Update();
    virtual void Draw(bool withTex = true);

    void UpdateMModels();
    void InsertMModels(Transformation& t);
    void InsertMModels(Transformation& t, int position);
    void InsertMModels(Mat4f& mat, int position);

    // Returns true if the position changed
    bool UpdateTranslation();

    void SetColorAll(Vector3f color = Vector3f(1, 1, 1), float alpha = 1.0f);

    bool will_not_draw();

    EffectBuffer* get_buffers();

    static float GetProjectionRotation();

public:
    bool mustDraw = true;

    static float _persRotation;
    static float _orthoRotation;

protected:
    EffectBuffer m_buffers;

    std::string m_modelName;
    Transformation m_modelMat;
};

#endif //PROJECT_GRAPHICS_COMPONENT_H
