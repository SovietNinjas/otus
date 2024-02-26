#pragma once

#include "model.h"

class ModelListener {
   public:
    ModelListener() : model(0) {}

    virtual ~ModelListener() {}

    virtual void updateVisualData() = 0;

    void bind(Model* m) { model = m; }

   protected:
    Model* model;
};
