#pragma once

class ModelListener;

class Model {
   public:
    Model();

    void bind(ModelListener* listener) { modelListener = listener; }

    void tick();

   protected:
    ModelListener* modelListener;
};
