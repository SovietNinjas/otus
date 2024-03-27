#pragma once

#include "presenter.h"
#include "view_base.h"

class View : public ViewBase<Presenter> {
   public:
    View() {}
    void updateData() {
        // text1.invalidate();
        // icon1.invalidate();
    }

    void buton1Callback() { presenter->button1Clicked(); }

    void buton2Callback() { presenter->button2Clicked(); }

    void buton3Callback() { presenter->button3Clicked(); }

   protected:
   private:
    // Text text1{};
    // Icon icon1{};
};
