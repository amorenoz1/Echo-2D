# Echo-2D

Tiny game engine for 2d games in c++

## Using Echo-2D 

```bash
# for macos
export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/path/to/Echo2D/lib

# for windows
set PATH=%PATH%;path\to\Echo2D\lib

# for linux
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/Echo2D/lib

```

## Examples Using Echo-2D

Here is a little exmaple using the Echo-2D.
It is the classic bouncing dvd icon, but 
instead of the dvd icon it is a square.

```c++
#include <Echo2D.h>

class MyApp : public Engine::Application {
public:
    MyApp () : Engine::Application(800, 600, "HelloThere") {}
    ~MyApp(){}

    // Square and background
    glm::vec2 velocity = {50.0f, 50.0f};
    glm::vec2 position = {400.0f, 300.0f};
    glm::vec2 dimensions = {100.0f, 100.0f};
    glm::vec4 color1 = {198.0f, 84.0f, 13.0f, 255.0f};
    glm::vec4 background = {144.0f, 233.0f, 200.0f, 255.0f};

    void Update(float dt) override {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;

        if (position.x + dimensions.x >= 800.0f) {
            position.x = 800.0f - dimensions.x;
            velocity.x *= -1;
        }

        if (position.y <= 0.0f) {
            position.y = 0.0f;
            velocity.y *= -1;
        }

        if (position.x <= 0.0f) {
            position.x = 0.0f;
            velocity.x *= -1;
        }

        if (position.y + dimensions.y >= 600.0f) {
            position.y = 600.0f - dimensions.y;
            velocity.y *= -1;
        }
    }

    void Render() const override {
        Engine::Renderer::ClearScreenColor(background);
        Engine::Renderer::DrawRect(dimensions, position, color1);
    }
};



int main (void) {
    MyApp *app = new MyApp();
    app->Run();
    delete app;
}

```
### Bouncing Square Video    
<p align="center">
  <img src="readmemeadia/bouncingsquare.gif" width="400">
</p>

