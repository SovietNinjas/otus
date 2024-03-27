#include <future>
#include <iostream>
#include <thread>

#include "model.h"
#include "model_listener.h"
#include "presenter.h"
#include "view.h"

static void guiEntry(Model& m) {
    using namespace std::chrono_literals;
    while (true) {
        m.tick();
        std::this_thread::sleep_for(1ms);
    }
}

template <typename PresenterT, typename ViewT>
std::pair<std::unique_ptr<PresenterT>, std::unique_ptr<ViewT>> makeTransition(Model& model) {
    auto view = std::make_unique<ViewT>();
    auto presenter = std::make_unique<PresenterT>(*view);

    model.bind(presenter.get());
    presenter->bind(&model);
    view->bind(*presenter);

    return std::make_pair(std::move(presenter), std::move(view));
}

int main(int, char**) {
    Model m{};

    auto gui = makeTransition<Presenter, View>(m);

    auto ret = std::async(std::launch::async, guiEntry, std::ref(m));

    // backend ..

    return 0;
}
