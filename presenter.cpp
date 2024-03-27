#include "presenter.h"

#include "view.h"

void Presenter::updateVisualData() { view.updateData(); }

void Presenter::button1Clicked() {
    model->createNewDocument();
}

void Presenter::button2Clicked() {
    model->importDocument();
}

void Presenter::button3Clicked() {
    model->exportDocument();
}
