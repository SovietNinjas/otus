#include "model.h"

#include "model_listener.h"

Model::Model() : modelListener(0) {}

void Model::tick() { modelListener->updateVisualData(); }
