#include "model.h"

#include "model_listener.h"

void Model::tick() {
    modelListener->updateVisualData();
}
