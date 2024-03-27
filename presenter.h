#include "model_listener.h"

#pragma once

class View;

class Presenter : public ModelListener {
   public:
    Presenter(View& v) : view(v) {}

    void updateVisualData() override;

    void button1Clicked();
    void button2Clicked();
    void button3Clicked();

   private:
    View& view;
};
