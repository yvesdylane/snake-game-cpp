#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>
#include <deque>

int main( int argc, char * argv[] )
{
    SDL_Init(SDL_INIT_EVERYTHING);
    auto window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, 0);
    auto renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event e;

    enum Direction
    {
        DOWN,
        LEFT,
        RIGHT,
        UP
    };

    bool running = true;
    int dir = 0;
    SDL_Rect head {500, 400, 10, 10};

    // Snake body container
    //
    std::deque<SDL_Rect> rq;
    int size = 1;

    // Apple container
    //
    std::vector<SDL_Rect> apples;

    // Create apples on the map
    //
    for(int i = 0; i < 100; i++)
    {
        SDL_Rect ap;
        ap.x = rand()%100*10;
        ap.y = rand()%100*10;
        ap.w = 10;
        ap.h = 10;
        apples.push_back(ap);
    }

    while(running)
    {
        // Check Input
        //
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT){ running = false; }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_DOWN) { dir = DOWN; }
                if(e.key.keysym.sym == SDLK_UP)   { dir = UP; }
                if(e.key.keysym.sym == SDLK_LEFT) { dir = LEFT; }
                if(e.key.keysym.sym == SDLK_RIGHT){ dir = RIGHT; }
            }
        }

        // Move
        //
        switch(dir)
        {
            case DOWN:
                head.y += 10; break;
            case UP:
                head.y -= 10; break;
            case LEFT:
                head.x -= 10; break;
            case RIGHT:
                head.x += 10; break;
        }

        // Collision detection with apples
        //
        std::for_each(apples.begin(), apples.end(), [&](auto& apple){
            if(head.x == apple.x && head.y == apple.y)
            {
                size += 10;
                apple.x = -10;
                apple.y = -10;
            }
        });

        // Collision detection with snake body
        //
        std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment){
            if(head.x == snake_segment.x && head.y == snake_segment.y)
            {
                size = 1;
            }
        });
        // Add newest head to the snake body
        //
        rq.push_front(head);

        while(rq.size() > size)
            rq.pop_back();

        // Clear Window
        //
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw Body
        //
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        std::for_each(rq.begin(), rq.end(), [&](auto& snake_segment){
            SDL_RenderFillRect(renderer, &snake_segment);
        });

        // Craw Apples
        //
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        std::for_each(apples.begin(), apples.end(), [&](auto& apple){
            SDL_RenderFillRect(renderer, &apple);
        });

        // Display
        //
        SDL_RenderPresent(renderer);
        SDL_Delay(25);
    }

    return 0;
}
