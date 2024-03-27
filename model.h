#pragma once

#include "file_manager.h"

class ModelListener;

class Model {
   public:
    Model() {}

    void bind(ModelListener* listener) {
        modelListener = listener;
    }

    void createNewDocument() {
        fm_.createNewDocument();
    }

    void exportDocument() {
        fm_.exportAs(".docx");
    }

    void importDocument() {
        fm_.import("directory");
    }

    void tick();

   protected:
    ModelListener* modelListener = nullptr;
   private:
    FileManager fm_{};
};
