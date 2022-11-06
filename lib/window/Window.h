#pragma once

class Window {
    public:
        Window() = delete;
        virtual ~Window();

        virtual void update() = 0;
    protected:
    private:
};
