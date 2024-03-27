#pragma once

template <typename PresenterT>
class ViewBase {
   public:
    ViewBase() = default;

    inline void bind(PresenterT& newPresenter) {
        presenter = &newPresenter;
    }

   protected:
    PresenterT* presenter = nullptr;
};
